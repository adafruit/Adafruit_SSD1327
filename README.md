# SSD1322 for Adafruit GFX Library

A quick implementation for SSD1322-based 256x64 OLED displays.

Based upon the [Adafruit SD1327 driver](https://github.com/adafruit/Adafruit_SSD1327).

⚠️ Until merged into main library, requires a
[forked version](https://github.com/jordanh/Adafruit-GFX-Library)
of the Adafruit GFX Library (see below).

These displays use I2C or SPI to communicate.

Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

# Installation
Checkout this repository to your Arduino library directory.

## Dependencies
 * [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

# Contributing

Contributions are welcome! Please read our [Code of Conduct](https://github.com/adafruit/Adafruit_SSD1327/blob/master/CODE_OF_CONDUCT.md>)
before contributing to help this project stay welcoming.

## Documentation and doxygen
Documentation is produced by doxygen. Contributions should include documentation for any new code added.

Some examples of how to use doxygen can be found in these guide pages:

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen-tips

## Formatting and clang-format
This library uses [`clang-format`](https://releases.llvm.org/download.html) to standardize the formatting of `.cpp` and `.h` files. 
Contributions should be formatted using `clang-format`:

The `-i` flag will make the changes to the file.
```bash
clang-format -i *.cpp *.h
```
If you prefer to make the changes yourself, running `clang-format` without the `-i` flag will print out a formatted version of the file. You can save this to a file and diff it against the original to see the changes.

Note that the formatting output by `clang-format` is what the automated formatting checker will expect. Any diffs from this formatting will result in a failed build until they are addressed. Using the `-i` flag is highly recommended.

### clang-format resources
  * [Binary builds and source available on the LLVM downloads page](https://releases.llvm.org/download.html)
  * [Documentation and IDE integration](https://clang.llvm.org/docs/ClangFormat.html)

## About this Driver

Written by Jordan Husney for personal use.

Based upon code by Limor Fried for Adafruit Industries.

BSD license, check license.txt for more information
All text above must be included in any redistribution

### Why the fork of Adafruit GFX?
The original implementation of [Adafruit_GrayOLED::oled_commandList(...)](https://github.com/adafruit/Adafruit-GFX-Library/blob/d9220ba7d3cb49915f6aca0feef680e3fed0c400/Adafruit_GrayOLED.cpp#L186)
does not handle toggling the D/C line when display arguments are used. This driver depends on a new method
called `oled_commandAndArgsList(...)`. If you do not desire using a forked version of Adafruit GFX, you may
add this method to the `Adafruit_SSD1322` class.
