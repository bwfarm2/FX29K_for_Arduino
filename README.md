# FX29K_for_Arduino
An Arduino library for the FX29K digital load cells.

## Compatibility
Supported models:
* `FX29Kx-xxxx-xxxx-L` (Tested on an FX29K0-040B-0010-L)
* `FX29Jx-xxxx-xxxx-L` (Not tested)

FX292 and FX293 analog load cells are *NOT* supported.

## Quick start
### Minimal setup
```
   RED ---------- VCC (2.7-5.5V)
YELLOW ---------- SCL
 WHITE ---------- SDA
 BLACK ---------- GND
```
### Installation
* Download this repository as a `.zip` file.
* In Arduino IDE, navigate to `Sketch` > `Include library` > `Add .zip library`.
* Select the downloaded `.zip` file.
### Main Program
#### Include
* In your `.ino` file, include [`FX29K.h`](./FX29K.h):
```c++
#include "FX29K.h"
```
#### Declaration
* Declare your `FX29K` object globally, with constructors:
```c++  
FX29K::FX29K(uint8_t addr, uint8_t range);
FX29K::FX29K(uint8_t addr, uint8_t range, TwoWire* i2cPtr);
```
* For example, for `FX29K0-040B-0010-L`:
```c++
FX29K fx29k(FX29K0, 0010, &Wire);
```

#### Initialization
* Tare load cell with:
```c++
void FX29K::tare(void);
void FX29K::tare(uint16_t samples);
```

#### Measurement
* Read raw wheatstone bridge data with:
```c++
uint16_t FX29K::getRawBridgeData(void);
```
* Get one measurement:
```c++
float FX29K::getPounds(void);
float FX29K::getKilograms(void);
float FX29K::getGrams(void);
```

## Simple weight measurement sketch
* [`/examples/Simple/Simple.ino`](./examples/Simple/Simple.ino)
## Changing I2C address (untested)

**DISCLAIMER**:<br>
By running this script you will alter the sensor's interal data registers. This is **potentially dangerous and will void your sensor's warranty**.<br>
Author will not take responsibilty for any potential damage that may be done to your sensors.<br>
Do **NOT** proceed unless you agree with the following quality assurance clause from the manufacturer and fully understand the potential risks.<br>
> **Caution**:<br>For the digital sensor. User can change the address along with the guide as below link.<br>Please notice, any data changed by users means users give up the quality assurance terms. TE will not guarantee the sensor performance any more.
* [`/examples/ChangeI2CAddress/ChangeI2CAddress.ino`](./examples/ChangeI2CAddress/ChangeI2CAddress.ino)
## Datasheets
* [Datasheets](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=showdoc&DocId=Data+Sheet%7FFX29%7FA5%7Fpdf%7FEnglish%7FENG_DS_FX29_A5.pdf%7FCAT-FSE0006)
