# Fade - a ESP32 Animation System

Fade is an ESP32-based animation system. It can drive WS2812/Neopixel addressable LEDs, PWM-based devices (LEDs or anything else that supports PWM), and hobby servo motors. 

## Introduction

See the introduction video [here](https://www.youtube.com/watch?v=-CXQz1x2k9o).

## Getting Started (Windows)

### Copy the installation files to a local directory

The installation files can be found [here](https://github.com/ericgu/Fade/tree/master/InstallFiles).

### Install WinFade
Run WinFadeInstaller.msi

### Install python and esptool.py

These tools will be used to copy the Fade system onto the ESP32

[How to Install Esptool on Windows 10](https://cyberblogspot.com/how-to-install-esptool-on-windows-10/)

### Copy/Flash the software onto the ESP32

It's good to have experience copying programs to the ESP32 from the Arduino IDE or another IDE before you try this. 

1. Connect the ESP to the computer with a USB cable (or whatever programmer you use).
2. Determine what COM port you are connected to
3. Edit FlashToEsp32.bat to specify the proper COM port
4. Run FlashToEsp32.bat
5. Put the ESP board into download board (press and hold the program button or press and hold it and then press and release the reset button)
6. Watch the download progress. 

### Verify the installation

1. Connect to the COM port with a terminal program such as CoolTerm.
2. Reset the ESP32
3. You should see debug text that says "Configuring Access Point"

### Connect the ESP to your wireless network

1. Using a wireless device (laptop or phone), connect to a network with the SSID named "FadeController_<number", password "12345678"
2. Your browser may hit the page directly; if not, navigate to [http://192.168.4.1](http://192.168.4.1)
3. Choose the appropriate wireless network. 
4. Reset the ESP

### Connect to the Fade web page

1. Determine the IP address of the ESP. You can get that either from your router or from the debug messages written when the ESP boots.
2. Enter that IP address into your web browser.
3. That should give you the web-based IDE for the ESP. 

### Connect WinFade to the ESP

1. Enter the IP address of the ESP into WinFade.
2. Enjoy.

## Fade language details

[Tutorial](https://rawcdn.githack.com/ericgu/Fade/26681499a88c3c0935cea77f8de56b3fef22759a/docs/FadeLanguageTutorial.html)
[Language Reference](https://rawcdn.githack.com/ericgu/Fade/26681499a88c3c0935cea77f8de56b3fef22759a/docs/FadeLanguageReference.html)



