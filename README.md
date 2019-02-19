
## About
As the name suggests, a Wordclock is a clock that displays the time using words instead of hands or digits.

The languages supported by this project are currently
- German

## Special features of this clock
- showing the time in words.
- an optional RTC module is supported since the clock of the Arduino Nano isn't the most precise one.
- an alarm that can be set for a certain time plus a weekday.
- flexibility by using selectable modes.
- a storage for colors that can be used and changed by all modes.
- approximately 20 modes.

## Modes

The controls are four buttons that allow to control the clock using _modes_. Two buttons set the mode (choose next / previous). The two remaining ones allow to configure the mode. What this means exactly depends on the mode. However, one is supposed to increment something whereas the other is supposed to decrement it.

## Configurability
Since the flash memory and RAM of the _Arduino Nano_ cannot grasp all modes  you can choose which one you want to upload and which not. Additionally, the most modes also allow you to specify more specific settings. Look into their header files to see what you must / can set.
Note that you have to wipe the EEPROM after changing any of these.
If you uploaded to many modes to the Arduino, it might be possible that the program doesn't work. This can be indicated by a black-screen, frozen effects or other glitches. In this case reset the EEPROM and upload less modes or other memory taking resources such as color themes. When not using more than 75% of the RAM or more than 60% of the flash memory for the program you should be fine.

When working around with colorful modes you will probably use objects that allow you to generated colors dynamically. These are called _generators_. Note that while many other settings are template parameters they are constructor parameters since they are objects.

You can set all this in the [Config.hpp file](Firmware/Wordclock/Config.hpp). Besides, you can also set some hardware settings, some \"manners\" and default values for the case of a EEPROM reset there. Said EEPROM reset can also be activated there.

## Setup
The display consists out off a RGB LED matrix behind a translucent plate and a plastic foil that masks the letters. In addition, the LEDs are separated by a grid so that every LED has its own lightning chamber. The LEDs are controlled by an [Arduino Nano](https://store.arduino.cc/arduino-nano). To keep better track of the time there is also a real time clock. All components are held together by a picture frame with five outlets. Four of them for the buttons and on for the power supply. All are attached at one side next to the electronic within the clock.

## Uploading
To upload the project to the Arduino Nano simply download it and open the _Wordclock_ folder within the _Firmware_ folder with the Arduino IDE. Make sure you have already installed the [FastLED library](http://fastled.io/) e.g. via the update manager and upload the program.

Note that you should reset the EEPROM when you upload the program the first time or when you change the color presets, alarms, modes in the Config.hpp file. It can also be reset in the Config.hpp file.
