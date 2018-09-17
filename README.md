# The Wordclock
A simple Wordclock project for a mode controlled Wordclock with four buttons.

## What is the project about?
In gerneral a Wordclock is a clock that displays the time using words and letter.
However, with this project the time can just be displayed in five minute intervals.

The display consists out off a RGB LED matrix behind a translucent plate with a plastik foil that masks the letters. In addition, the LEDs are seperated by a grid so that every LED has its own lightning chamber. The LEDs are controlled by an [Arduino Nano](https://store.arduino.cc/arduino-nano). All componnents are held toghether by picture frame which has five outlets for the four buttons and the power supply.

Already supported languages are
- German

## Modes and effects
As already mentioned, the Wordclock can be controlled by four buttons. Two of them choose the next or previous one of the so called _modes_ and the purpose of the two remaining ones is determined by said mode.
The other thing are _effects_. As the name suggests, they add some effects to the data displayed by the mode. It can be something to show hidden time data such like seconds or the weekday or they simply improve the appearance of the clock.
There is also a mode to choose the next or previous effect.
So modes are meant for configurating the clock and effects mainly add some grahical effect to it.

## Special features of this clock
- showing the time in words.
- an optional RTC module is supported since the clock of the Arduino Nano isn't the most precise one.
- a storage for colors, called color presets, that can be manipuled by some modes and be used for many effects.
- an alarm that can be set for a certain time plus a weekday.
- approximatelly 20 modes.
- approximatelly 10 effects.

## Configurability
Since the flash memory and RAM of the Arduino Nano cannot grasp all modes and effects you can choose which one you want to upload and which one you want to omit. Additionally, the most modes and effects also allow you to specify more specific settings. Look into their header files to see what you can or you have to set.

You can set all this in the [Config.hpp file](Firmware/Wordclock/Config.hpp). Besides, you can also set some hardware settings, some \"manners\" and default values for the case of a EEPROM reset there. Said EEPROM reset can also be activated there.

### Generators
When congigurating the effects you will encounter something that's called _generators_. These are classes that generated colors for diverse purposes. Note that while many other settings are template parameters they are constructor parameters since they are objects.

## Uploading
To upload the project to the Arduino Nano simply download it and open the [Wordclock.ino file](Firmware/Wordclock/Wordclock.ino) with the Arduino IDE. Make sure you have already installed the [FastLED library](http://fastled.io/) e.g. via the update manager. Then set the board to Arduino Nano and the port to the one you connected your Arduino to and press upload.
I relinquished a file structure ordered by folders and rather organized them by their name so you can upload it that way.

Optionally, you can synchronize your project with Visual Studio with the [Visual Micro plugin](https://www.visualmicro.com/) installed and upload it from there. I highly recommend this option when you want to extend the program. Otherwise the Arduino IDE is just fine.

Note, however, that you should reset the EEPROM when you upload the program the first time or when you change the color presets, alarms, modes or effects in the Config.hpp file.
To reset the EEPROM simply activate _RESET\_EPPROM_ or _RESET\_EEPROM\_SAFELY_ in the Config.hpp file.
