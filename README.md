# 2019-SikLights

Siklights are lights of some kind on an FRC robot.

## What does this do?

This Arduino program recieves commands over i2c and then controls NeoPixels strips based on the commands recieved.
The commands are send from the RoboRio code over i2c.

## What for?

Our goal was 2 fold.
1. To provide some interest (sick lights) on the robot
2. To provide some visual feedback to operators

## How does the Arduino code work?

This code uses 2 libraries, Wire, to communicate over i2c, and Adafruit_NeoPixel to control a strand of led lights.

This is just the reciever.

The code is using a few pins.
- 4 & 5 are the i2c pins on an Ardunino UNO. Check your board.
- 14 & 15 are being used to control the NeoPixels (we had 2 strands)
- 13 is the built in Arduino UNO status light.

The code initializes both the Wire library and the NeoPixels in setup. The real magic is in the handleEvent method.

The handleEvent method is called when ever a command is recieved by i2c. The commands are simply numbers. This requires the sender program and this reciever program to jointly decide what these command numbers mean. There are more descriptive ways to do this, but this was done for simplicity. Only one byte needed.

When a command is recieved, methods to change the colors on the lights are called. There is one call per strand. This is code that should change the state of a strip of lights and end. This system can not be used to run animations.

## How does the RoboRio code work?

This is not the most elegant code, but it did work. Keep in mind this was 2019 code, and some locations you might put this code for 2020 are different.

In 2019's Robot class you would need to define some variables like this:
```
	// I2C Variables
  private I2C.Port i2cPort;
  private byte[] dataSend;
  private byte[] dataRecieve;
  private I2C i2c;
  private int deviceNo;
```
Then you would need a setup method that could be called from robotInit() like this:
```
private void doI2cSetup() {
    i2cPort = I2C.Port.kMXP; // when using the MXP expansion Port
    // i2cPort = I2C.Port.kOnboard; // when using the RoboRio built in port
    deviceNo = 4; // needs to match Arduino
    i2c = new I2C(i2cPort, deviceNo);
    dataSend = new byte[1];
    dataRecieve = new byte[1];
    System.out.println("Init I2C");

    dataSend[0] = 2;
    i2c.transaction(dataSend, 1, dataRecieve, 1);
    System.out.println("Turn purple lights on");
  }
```
Then you can send a command to the Arduino. These commands are just a single byte. We only create a few of these. You can see more of what the commands did if you look at the Arduino code.

We used this code in teleopPeriodic() to turn warning lights on at 30sec to go:

```
public void teleopPeriodic() {
    Scheduler.getInstance().run();
    double timeLeft = DriverStation.getInstance().getMatchTime();
    // if less than 30 sec run once
    if (timeLeft <= 30 && is30Sec == false && timeLeft != -1) {
      dataSend[0] = 3;
      i2c.transaction(dataSend, 1, dataRecieve, 1);
      System.out.println("Turn on 30sec animation");
      is30Sec = true;
    }
  }
```
The key part is just setting the command you want `dataSend[0] = 3` and then sending it.

## How did the lights get hooked up?

We were using a RioDuino http://www.revrobotics.com/rev-11-1104/ that is now discontinued that plugged into the MPX port. You can use the I2C pins on the MPX port or the seperate I2C port on the RoboRio. Just make sure to specify which one you are using in the Arduino code. You can then pick any two output pins on the RioDuino or other microcontroler to use as the NeoPixel output pins. The NeoPixels also need power and ground. BUT, the output from the RoboRio, or a microcontroller is not enough to power a hundred or more leds. You will need to connect another 5v power source on the robot that will have enough juice. Don't power your microcontroller with this, just the leds, and tie the grounds together.
