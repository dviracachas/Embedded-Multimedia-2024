#!/bin/bash

echo "Welcome to the Hey Pola installer"

# Requirements
echo "Installing non-Qt system packages (speech-dispatcher)"
sudo apt-get install speech-dispatcher
sudo apt-get install speech-dispatcher-audio-plugins

echo "Installing Python requirements (whisper, faster-whisper)"
sudo pip install whisper faster-whisper --break-system-packages

echo "Cloning git into /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/ ..."
cd /home/pi
mkdir .HeyPolaProject
cd .HeyPolaProject
git clone https://github.com/dviracachas/Embedded-Multimedia-2024.git

echo "Copying porcupine shared library for Raspberry Pi 4b"
cd /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/resources
sudo cp libpv_porcupine.so /usr/local/lib

# Compilation of the Qt Projects
echo "Compiling source code ..."
echo "HeyPola Wake-Word-Detection"
cd /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/HeyPola
qmake HeyPola.pro
make

echo "fw_Record"
cd /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/fw_Record
qmake fw_Record.pro
make

echo "qt_llama"
cd /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/qt_llama
qmake qt_llama.pro
make

echo "TextToSpeech"
cd /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/TextToSpeech
qmake TextToSpeech.pro
make

echo "HeyPolaStateMachine"
cd /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/HeyPolaStateMachine
qmake HeyPolaStateMachine.pro
make

cd /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/resources
touch AccessKey.txt

echo "Installation done :)"

# Warnings
echo -e "\e[1;43mPlease remember to put your Porcupine Access Key in /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/resources/AccessKey.txt to make sure everything works correctly!\e[0m"
echo -e "\e[1;43mAlso edit the IP Address in /home/pi/.HeyPolaProject/Embedded-Multimedia-2024/qt_llama/AskPola.py to the IP running the Ollama API in your local network\e[0m"
