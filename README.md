# Goods Sense device (Final Project)

This project is generating musical tones 440 Hz (an A4); 587 Hz (a D5); 659 Hz (an E5); and 880 Hz (an A5) on KL25Z.

This project contains following implementations:

1. Configuring DAC (Sampling Rate: 48 KHz, Resolution: 12 bits per sample)
2. Configuring DMA
3. Configuring ADC (Sampling Rate: 96 KHz, Resolution: 16 bits per sample)
4. GPIO configuration from previous project


## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
For this project, you would require MCUXpresso installed on your PC. You can use Linux, Windows or Mac system to run this program. After installation SDK for KL25Z (relevant chip) needs to be installed.

### Installing
MCUXpresso 11.2 is used for this project. Import this project in MCUXpresso by clicking on File->Import->Existing Project.

## Running the tests
Run or Debug Configuration can be used to run this project. 
1. Debug configuration prints debug data over serial emulator.
2. Run configuration is a release configuration and is same as production release

## Tones
Following is an image to showing different tones generated using fixed point sin function.
![Tones](screenshots/tones.JPG)
Data points were copied into excel sheet and waveform was plotted.

## Extra Credit
ADC calibration implemented


## Built With
MCUXpresso 11.2
arm-none-eabi-gcc compiler

## Authors
* Rajat Chaple

## Acknowledgments
Code development was done with the help of Dean's book and KL25 reference manual.

Saket Penurkar helped me resolving couple of critical bugs.

Readme.md was written using template at
https://gist.github.com/PurpleBooth/109311bb0361f32d87a2



