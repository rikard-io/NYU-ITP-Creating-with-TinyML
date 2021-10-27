# Training a voice command model with Edge Impulse
In this experiment we're going to train a Convolutional Neural Network (CNN) to classify voice commands based on the speech command dataset – a collection of more than 65,000 one second recordings of 34 different words published by Google.

There's two parts to this assignment. 
1. Generate a dataset
1. Train the model on Edge Impulse

## Alternative 1. Generate the Dataset with Colab
1. [![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/rikard-io/NYU-ITP-Creating-with-TinyML/blob/main/04%20-%20Voice/04a_create_voice_dataset/Creating_with_TinyML_04a_Generate_a_Voice_Dataset.ipynb)

1. Step through the cells. The dataset we're basing our (smaller) dataset on is about 2GB, so it will take a while to download – about 15min in Colab.
1. Unzip the resulting zip file.
1. You're now ready to move on to training

## Alternative 2. (Advanced) Generate the Dataset locally.
Might be faster to download and you don't risk losing the downloaded data in case Colab disconnects.

I highly recommend using a virtual environment when working with python locally. [Pipenv](https://github.com/pypa/pipenv) will help you setting one up. 

1. Make sure you have python3 installed. Sometimes both python and python3 are installed on the system and python 3 will be aliased to python3 (and pip to pip3).
1. cd into 04_create_voice_dataset: `% cd 04_create_voice_dataset`
1. Install requirements `% pip -r requirements.txt`
1. Run `% python3 generate.py --words=cat,dog` (or whatever word you want to try)
    1. You can run `% python3 generate.py --help` to see some other options
1. Wait for everything to be downloaded and files to be generated.
1. You're now ready to move on to training.

## Step 2. Training
For this we're going to use [Edge Impulse](https://www.edgeimpulse.com/). There's plenty of ways to use Edge Impulse – not just for Audio – and it's great for capturing and segmenting your own Audio. There's a cli you can install and even firmware for the Arduino to integrate more tightly with the whole ecosystem. We're going to take a more manual approach, but definitely explore other options when working on your own projects!

A good tutorial on how to use Edge Impulse for Voice can be found [here](https://docs.edgeimpulse.com/docs/responding-to-your-voice) and general Sound / Audio [here](https://docs.edgeimpulse.com/docs/audio-classification)

Instructions:
1. Sign up if you haven't already
1. Create a new project
1. In the splash screen, select Audio
1. Select `Go to the uploader` under Import Existing Data
1. For each folder created in the previous step:
    1. Click `Choose Files`
    1. Select all files in the folder
    1. Click open
    1. Mark `Enter Label` under Label and type in the corresponding label (folder name)
    1. Click `Begin Upload` and wait for it to complete
1. Once everything is uploaded, click `Create Impulse` in the sidebar
1. Add processing block `Audio (MFCC)`
1. Add learning block `Classification (Keras)`
1. Click `Save Impulse`
1. Click `MFCC` in the sidebar
1. Click `Save parameters` (default are pretty good here but feel free to experiment)
1. Click `Generate features`
1. Once done, take note of the nice 3D visualization of your classes. If you can visually separate the classes, it's a good indication that the model will be able to do the same. You can also click on the nodes and listen to the files, so if two nodes of different classes are very close to each other, you can 
explore why.
1. Click `NN Classifier` in the sidebar
1. Click `Start training`, defaults are ok here. Number of epochs could be lowered to ~60, depending on how much data and how many classes you have.
1. Wait for it to complete
1. Click `Deployment` in the sidebar
1. Select `Arduino Library`
1. Click `Build`
1. A zip with an Arduino library will be downloaded. Don't unzip it
1. In the Arduino IDE, go to `Sketch->Include Library->Add .zip file` and navigate to / select your downloaded zip file.
1. In the Arduino IDE under `File->Examples` find your project and select `nano_33ble_sense_microphone_continuous`
1. Build and upload!
1. Open the Serial Monitor and test out your model.

