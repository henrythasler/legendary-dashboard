# Legendary Dashboard - Tutorial

## Open project

![open](docs/tutorial/open-project.png)

## Explorer view

![files](docs/tutorial/files.png)

## PlatformIO Tasks

![open](docs/tutorial/platformio-tasks.png)

## (2) Build Project

![build](docs/tutorial/build.png)

## (3) Remote Devices

![device-list](docs/tutorial/device-list.png)

## (4) Remote Upload

![remote-upload](docs/tutorial/remote-upload.png)

## (5) Remote Monitor

![remote-monitor](docs/tutorial/remote-monitor.png)

## native Unit-tests

These tests will be run locally on your PC

![test-files](docs/tutorial/test-files.png)

![test-native](docs/tutorial/test-native.png)

![test-result](docs/tutorial/test-result.png)

## Image conversion

This is how it works with [GIMP](https://www.gimp.org/). 

1. Create an image with the size of your display (e.g. 400x300).
2. Adjust brightness and contrast to almost extreme values.
3. Create a palette that represents the colors available on your display and convert the image:

![test-native](docs/tutorial/image-palette.png)

![test-native](docs/tutorial/image-mode.png)

![test-native](docs/tutorial/image-convert.png)

4. separate both colors (black, yellow) and save each into a separate file. Convert each file with [image2cpp](http://javl.github.io/image2cpp/) 
