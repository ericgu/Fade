separate directory.
2. Copy the [.bin](https://github.com/ericgu/Fade/tree/master/InstallFiles) file to your local machine.
3. Run the flash_download_tool_[version].exe file
4. Choose SPIDownload Tab
5. Add the downloaded firmware file to the first line.
6. At the bottom, choose the COM port and the serial speed.
7. Choose "Start"

### Provision the ESP32 to connect to your local network. 

1, Start the ESP
2. It will create a wireless network named "SequenceController" (old name) and password "12345678"
3. Connect to it. 
4. If your browser doesn't auto-navigate, go to http://192.168.4.1
5. Choose the wireless network you want to connect to.

### Connector the controller

1. Navigate to the controller IP address with your web browser. You may have to hit your router to figure that out, or you can connect a USB cable to the ESP and look to see what IP address it is using.
2. That should show the Fade web-based authoring interface. 

## Fade language details

[Tutorial](https://rawcdn.githack.com/ericgu/Fade/26681499a88c3c0935cea77f8de56b3fef22759a/docs/FadeLanguageTutorial.html)
[Language Reference](https://rawcdn.githack.com/ericgu/Fade/26681499a88c3c0935cea77f8de56b3fef22759a/docs/FadeLanguageReference.html)







