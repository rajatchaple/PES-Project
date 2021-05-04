# Getting in Tune (Assignment 7)

In this project, the developed device is targeted to sense condition of goods while transporting, which is its orientation. Also, Since this device needs to be standalone product and portable, it is required to be battery operated. This requires low power operation of device.

This project contains following implementations:

1. Accelerometer interface over I2C
2. LPTIMER (low power timer)
3. Deep sleep into low leakage stop
   


## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites
For this project, you would require MCUXpresso installed on your PC. You can use Linux, Windows or Mac system to run this program. After installation SDK for KL25Z (relevant chip) needs to be installed.

### Installing
MCUXpresso 11.2 is used for this project. Import this project in MCUXpresso by clicking on File->Import->Existing Project.

## Running the tests
Run or Debug Configuration can be used to run this project. 
Tests are manual as almost every function call is someway or the other connected to external hardware.
# TEST 1 : Software integration and Hardware test
Testing sequence as below:
1) Press and hold TEST_SWITCH (pin details in gpio_pins.h) followed by reset
2) This will enter device into Test Mode also ensured by External LED 1. (pin details in gpio.h)
3) Firstly it checks all LEDs
   sequence : RED, GREEN, BLUE, EXTERNAL_OFF, EXTERNAL_ON
4) It then calibrates accelerometer into current device's pose
5) Device needs to be tilted for angle greater than ~40 degrees in any direction. Tilt is indicated by CYAN led. Perform for mulriple angles.
6) Device remains in test mode. TO returnd device into App mode, reset device without pressing TEST_SWITCH

# TEST 2 : Energy profiling for low power mode
1) Simplicity studio and Blue geckos AEM was used for energy measurement
2) KL25Z acted as a peripheral device

Since on average this device uses current of 49uA. Estimated battery life for device is 7 days for 2400mAh





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



