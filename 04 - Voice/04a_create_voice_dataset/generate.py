import download
import argparse
import os
import numpy as np
import shutil
import math
import soundfile as sf

def _generate_silence(file, num):
  data, sr = sf.read(file)
  split = []
  duration = int(np.ceil(len(data) / sr))

  out_path = os.path.join(args.out_dir, 'silence')
  if not os.path.isdir(out_path):
    os.mkdir(out_path)
    
  for i in range(num):
    o = math.floor(np.random.uniform(0, duration-1))
    temp = data[o * sr: o * sr + sr]
    temp *= np.random.uniform(0, 1)
    if '_noise_' in file:
      temp *= 0.1
    split.append(temp)

  base_name = os.path.splitext(os.path.basename(file))[0]
  for i in range(num):
    filename = os.path.join(out_path, f'{base_name}_{i}.wav')
    sf.write(filename, split[i], sr)


parser = argparse.ArgumentParser()
parser.add_argument(
  '--data_url',
  type=str,
  # pylint: disable=line-too-long
  default='https://storage.googleapis.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz',
  # pylint: enable=line-too-long
  help='Location of speech training data archive on the web.')

parser.add_argument(
  '--data_dir',
  type=str,
  default='download/',
  help="""\
    Where to download the speech training data to.
    """)

parser.add_argument(
  '--out_dir',
  type=str,
  default='dataset/',
  help="""\
    Where to write the dataset to.
    """)
parser.add_argument(
  '--words',
  type=str,
  default='cat,dog',
  help="""\
    Voice command words to use for labels. Separated by a comma.
    """)
parser.add_argument(
  '--max_files',
  type=int,
  default=0,
  help="""\
    Max files to generate
    """)

args = parser.parse_args()

#"yes", "no", "up", "down", "left", "right", "on", "off", "stop", and "go".
ALL_WORDS = ["bed", "bird", "cat", "dog", "down", "eight", "five", "follow", "forward", "four", "go", "happy", "house", "learn", "left", "marvin", "nine", "no", "off", "on", "one", "right", "seven", "sheila", "six", "stop", "three", "tree", "two", "up", "visual", "wow", "yes", "zero"]

SELECTED_WORDS = args.words.split(',')

def make_dataset(labeled_files, unknown_files, noise_files):
  if os.path.isdir(args.out_dir):
    shutil.rmtree(args.out_dir)
  os.mkdir(args.out_dir)
  
  for index, files in enumerate(labeled_files):
    label = SELECTED_WORDS[index]
    label_path = os.path.join(args.out_dir, label)
    os.mkdir(label_path)
    for file_path in files:
      shutil.copy2(file_path, f'{label_path}/')
  
  unknown_path = os.path.join(args.out_dir, 'unknown')
  os.mkdir(unknown_path)

  for file_path in unknown_files:
    shutil.copy2(file_path, unknown_path)

  num_slices_per_noise_file = math.floor(len(unknown_files) / len(noise_files))
  print('num_slices_per_noise_file',num_slices_per_noise_file)
  for noise_file in noise_files:
    _generate_silence(noise_file, num_slices_per_noise_file)

  
def _get_wav_files(label):
  dir = os.path.join(args.data_dir, label)
  files = []
  for file in os.listdir(dir):
    if file.endswith(".wav"):
        files.append(os.path.join(args.data_dir, label, file))
  return files

def _check_label(label):
  if not label in ALL_WORDS:
    raise Exception(f"{label} is not a word in the dataset")

def generate():
  for label in SELECTED_WORDS:
    _check_label(label)
  
  np.random.seed(0)
  
  list_of_files = [_get_wav_files(label) for label in SELECTED_WORDS]
  if args.max_files > 0:
    max_per_label = math.floor(args.max_files / (len(SELECTED_WORDS)+2))
    list_of_files = [np.random.choice(l, max_per_label, replace=False) for l in list_of_files]
    
  num_word_files = len(list_of_files[0])

  list_of_non_words = [word for word in ALL_WORDS if not word in SELECTED_WORDS]
  list_of_non_word_files = np.hstack([_get_wav_files(label) for label in list_of_non_words])

  random_selection = np.random.choice(list_of_non_word_files, min(num_word_files, len(list_of_non_word_files)), replace=False)  

  noise_files = _get_wav_files('_background_noise_')

  make_dataset(list_of_files, random_selection, noise_files)

if __name__ == "__main__":
  download.maybe_download_and_extract_dataset(args.data_url, args.data_dir)
  generate()
  