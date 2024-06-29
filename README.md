# Got to Kromek Driver

```bash
cd ~/Projects/Kromek/Driver

ls
.  
├── CMakeLists.txt  
├── ConsoleExample  
├── KromekDriver  
├── libs  
├── LinuxModule  
├── Readme.txt  
├── SpectrometerDriver  
├── SpectrometerExample  
└── StandardSetup.cmake
```

# Install Linux Driver

The Kromek driver for Linux is provided in two parts. A kernel module (.ko file) and a dynamic library (.so). To use the library, you will need to compile and install the kernel module on your Linux system. You will need to perform these steps each time your kernel changes

## Pre-requisties

To build the module you will need to install the current kernel headers.

On Ubuntu this can achieved by installing the appropriate package:

```bash
sudo apt-get install linux-headers-generic
```

On Raspberry Pi

```bash
sudo apt-get install raspberrypi-kernel-headers
```
## Installation

An installation script (install.sh) has been provided to compile and install all the appropriate files. This will  install the driver on most forms of Linux that use udev and systemd (Tested on Ubuntu + Raspbian)

For a more detailed installation guide see below

## Building and installing the kromek usb driver

To interface reliably with a kromek device a driver module has been provided. Due to the varied nature of the Linux kernel you will need to re-compile the driver for your platform / distribution.

Navigate to the LinuxModule folder and build the module using _make_

The module can then be installed by using _make install_

```bash
make
sudo make install
```
## Loading the driver module

Once installed the driver module can be loaded using modprobe or insmod. It is a good idea however to load it automatically on boot. To do this copy the kromekusb.conf file from the LinuxModule folder to /usr/lib/modules-load.d/ and run depmod to update module dependencies

```bash
sudo cp kromekusb.conf /usr/lib/modules-load.d
sudo depmod
```

## Assigning the driver when connecting a device (udev)

For the device to be recognised when it is plugged in a new udev rule needs to be added. A standard configuration file is included in the LinuxModule folder (99-kromek.rules). Copy this into your udev rules folder and reload the rules

```bash
sudo cp 99-kromek.rules /etc/udev/rules.d/99-kromek.rules
sudo udevadm control --reload-rules
```

- Plug in a Kromek device
- Reboot
- Check that a new device node has been added in /dev/kromek*

## Using the USBSpectrometer library

In order for your application to find the shared library file (libUSBSpectrometer.so) you will either need to copy it into your distributions lib directory e.g. (/usr/lib/) or add the containing directory to your LD_LIBRARY path.

Further documentation on use of the shared library is provided in a separate document.
# Install Driver

## Install dependencies

```bash
sudo apt install gcc g++ cmake
sudo apt install libudev-dev
sudo apt install qt4-default
```
## Compile on Driver directory

```bash
cmake .
make
```
## Execute

```bash
cd output/linux64/release
```

# Compile ARC's Spectormeter

My directory stucture

```bash
cd ~/Projects/Kromek
.
├── Documents     # PDFs
├── Driver        # Kromek Driver 
└── Spectrometer  # FIU's Spectrometer
```

## Link Kromek library 

Set Kromek's directory driver path in CMakeLists.txt

```bash
cmake_minimum_required(VERSION 3.5)

project (Spectrometer)

###############################################
# Set Kromek Driver directory

set(DRIVER "~/Projects/Kromek/Driver") # <===

###############################################

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "-pthread -Wno-enum-compare")

include_directories(
	${DRIVER}/KromekDriver
	${DRIVER}/SpectrometerDriver
)

add_executable( ${PROJECT_NAME}
	main.cpp
	Kromek.hpp
	Kromek.cpp
	Helpers.hpp
	Helpers.cpp
)

target_link_libraries( ${PROJECT_NAME}
	${DRIVER}/output/linux64/release/libSpectrometerDriver.so
)
```

## Build Spectrometer

```bash
cd Spectrometer

mkdir build
cd build
cmake ..
make
```

## Run application

```bash
./Spectrometer
```
