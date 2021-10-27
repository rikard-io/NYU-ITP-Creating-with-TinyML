import os
import sys
import tarfile
import urllib.request

# From: https://github.com/tensorflow/tensorflow/blob/de034a2911bdf0547a92e79b0c9858f8c4625fe0/tensorflow/examples/speech_commands/input_data.py
def maybe_download_and_extract_dataset(data_url, dest_directory):
  """Download and extract data set tar file.
  If the data set we're using doesn't already exist, this function
  downloads it from the TensorFlow.org website and unpacks it into a
  directory.
  If the data_url is none, don't download anything and expect the data
  directory to contain the correct files already.
  Args:
    data_url: Web location of the tar file containing the data set.
    dest_directory: File path to extract data to.
  """
  if not data_url:
    return
  if not os.path.isdir(dest_directory):
    os.makedirs(dest_directory)
  file_name = data_url.split('/')[-1]
  file_path = os.path.join(dest_directory, file_name)

  if not os.path.exists(file_path):
    def _progress(count, block_size, total_size):
      sys.stdout.write(
        '\r>> Downloading %s %.1f%%' %
        (file_name, float(count * block_size) / float(total_size) * 100.0))
      sys.stdout.flush()

    try:
      file_path, _ = urllib.request.urlretrieve(data_url, file_path, _progress)
    except:
      print(
        'Failed to download URL: {0} to folder: {1}. Please make sure you '
        'have enough free space and an internet connection'.format(
          data_url, file_path))
      raise
    print()
    statinfo = os.stat(file_path)
    print('Successfully downloaded {0} ({1} bytes)'.format(
      file_name, statinfo.st_size))
    tarfile.open(file_path, 'r:gz').extractall(dest_directory)

if __name__ == "__main__":
  maybe_download_and_extract_dataset('https://storage.googleapis.com/download.tensorflow.org/data/speech_commands_v0.02.tar.gz', 'download/')