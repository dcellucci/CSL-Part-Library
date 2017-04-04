# RTC Library

_Coded structures Lab_

This RTC library is an modification of Arduino's RTCZero library. While the RTCZero library requires an external 32k oscillator and focuses on Mode 2 for the RTC (the calendar functionality), this library allows flexible clock selection (internal and external) as well as use of Modes 0 and 1, the 32- and 16- bit counters, respectively.

The end goal of this library is to provide stable timestamping and synchronization functionality for an array of distributed microcontrollers. 

Settings:
16 or 32 bit counter
internal or external clock
setinterval (via prescale)

Methods:
setcounter()
readcounter()
calibrateCrystal() -> should it save the values in the flash

Extras:
Attaching an interrupt to the syncready bit in the INTFLAG register rather than doing the stupid while loop thing.

== License ==

Copyright (c) Arduino LLC. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
