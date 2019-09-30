
## About
A Wordclock is a type of clock that uses a matrix of letters to display the current time instead of using hands or a monitor. Each letter has an RGB LED with a masking foil used to \"form\" the light into the actual letter. It has no higher functions than showing the time and being a decorative object but it is way cooler than a regular clock. Besides, it supports a couple of neat special effects and a highly configurable set of modes.

I wrote this program for an Arduino Nano. Other Arduino boards are neither tested nor officially supported which doesn't necessary mean that it does not work with them though. However, they should not have less than the 2KiB of RAM and 30KiB of flash memory for the program.

The current supported languages to be displayed by the letter matrix are
- German

## Special features of this clock
This project is not the first implementation of a Wordclock. Therefore I am going to list the features of this Wordclock implementation that extend the
basic "showing the time in words" function.
- An optional RTC module is supported since the clock of the Arduino Nano isn't the most precise one.
- 4 buttons can be used to control the clock while operating.
- There are a couple of modes that allow you to configure the clock while operating or that display the time often combined with some interest special effects.
- The Clock is highly configurability since you can decide which mode to use and how they should be configured.
- Multiple modes can run at the same time.
- There is an alarm mode that can change modes automatically at a certain time.
- There is a list of colors, called color presets, that can be used and changed by all modes. You can initialize them with the colors you want giving you effectively a way to specify color themes.
- You can configure a lot of other factors such as wiring, too. All those settings are combined in the file Config.hpp.
- configurations done at runtime are saved in the EEPROM. So you don't need to set anything again after the clock was powered off. Note that the time is stored in the RTC. When you don't use one you will still need to set the time anyway. Storing this in the EEPROM is senseless as time changes.

## Controlling at runtime

You can control the clock with 4 buttons at runtime. Normally, you can use two of them to select the next or previous mode. These two are called *mode buttons*. The remaining two allow you interact with the mode. Those two are called *action buttons*. Normally, they allow you to increment or decrement a value. Note that some modes such as the *ModeAlarm* even utilize the mode buttons partially to interact with the mode.

How a specific mode is supposed to be operated is still documented in code at its class definition.

## Configurability

You can freely select and combine all available modes in the file Config.hpp. Add the modes you want to the array at the bottom of the file. Note that each mode start with the word Mode... You can also let the section the way you found it, since the configuration of the release version is valid always. When selecting modes don't exaggerate it since RAM and the flash memory for the program are limited. So you won't be able to upload all modes at least on an Arduino Nano.

All available modes come from the import section above the mode array. Below the import section is a section for custom defined modes. I took the liberty to define some already. Feel free to use them (as examples). Some modes take additional objects such as other modes, color generators or selectors. Color generators allow you to specify the colors your mode uses at runtime. Analogously, selectors are used to select something at runtime. The documentation of the all these can still be found in the code of the modes at the class definition.

## Parallelity

You can have multiple modes running at a time. To be exact: you have the same number of modes running always or at least between two uploadings. The count is called *layer count* as each of this modes runs on one *layer*. Layers with a higher number appear behind all modes with a higher number i.e. in the background. Just the first layer receives any signals from the buttons. You can limit the count of modes selectable with the mode buttons to the 1 to n-th in the Config.hpp file.

Besides, you can specify the count of layers as well as the default modes for a layer in the Config.hpp file as well. Note that all layers can run any mode from the array but one mode can just be active on one mode. Thus when allowing the user to select a mode on a layer, it might be a good idea to just allow to select a certain set of modes for exactly one layer without overlap between the layers.

## Upload

1. First you need to make sure all dependencies are avaialable. Those are:
	- [FastLED library](http://fastled.io/) - when using the official Arduino IDE you can install it using the update manager.
2. Download a release via the release button. Don't download the current content of the repository. This is work in progress content.
3. Unzip the content and extract the Wordclock directory from the Firmware directory.
4. Configure the Config.hpp file within the latter directory. When you have an old version of it, don't copy-and-paste its content into the new file. It might have changed, so you will need to do the changes in the new file manually. Of course you can also do it automatically if you know the right tools. Note that you also need to set the the option *INTERNAL_TIME* when you upload the program the first time on the Arduino or when you changed anything connected to the modes. After having it uploaded, you must upload the program again with *INTERNAL_TIME* disabled provided you want your program to use the EEPROM.
5. Compile and upload the content of the directory. I left it in a flat format which is a little messy but you can upload it using the official Arduino IDE that way.
6. When you run into any errors, check whether your configuration was legal first please, before reporting a not existent bug.

## Hardware Details

I used a WS2812B RGB LED stripe wired in a wooden casing which ensures that one LED just lights one letter. A black acrylic glass plate covers both casing and LEDs. It ensures that the slots of the LEDs cannot be seen from the outside. This all is contained in a picture frame with the 4 buttons, the Arduino and the RTC module at one side of the clock plus a socket for the power supply. The clock is powered by an external 5V power adapter. The RTC module is enhanced by a button cell that allows it to keep track of the time even when the clock is not powered.

## Changelog

### v1.1
- Convert _effects_ to _modes_
- Cache color presets and alarms

### v1.2
- lots of bugfixes
- new modes

## v2.1
- You can run multiple modes parallelly now.
- lots of bugfixes
- change of the entire alarm system
- add selectors
- modes take no parameters anymore. You can configure them using template parameters know.
- improve almost any mode existing to that point.
- new modes, especially masking modes.