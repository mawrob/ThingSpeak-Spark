This is a sample of code to demonstrate how to use ThingSpeak to monitor, control, and log data from a Spark Core.

The ThingSpeak channel interface includes the ability to create javascript plugins (or widgets) than can be added to a channel page.  Three Plugin examples are included in the code:

1. A button to control a LED connected to the Spark ' SparkLEDControl.html'
2. A button to display the state of a switch connected to a digital input on a Spark core ' SparkButtonState.html'
3. A gauge to display the value of an analog input on the Spark connected to a potentiometer SparkAnalogGauge.html'

The schematic and the breadboard layout are included above.

A screen shot of the ThingSpeak channel interface is also shown.

The code example for the Spark Core includes the following three files: iodemo.ino, newThingSpeak.cpp and newThingSpeak.h.

The newThingSpeak libraries are copies of the following github libraries: https://github.com/dareid/sparkcore.thingspeak

ISSUE:

At present this code is unreliable. It appears that the spark.function() and Spark TCPClient functions are interacting badly.  Symptoms include the Spark rebooting randomly, failing to write to ThingSpeak, and not responding to api calls.  It may be something to do with http buffers or with timing and feedback on what the issue may be is appreciated.