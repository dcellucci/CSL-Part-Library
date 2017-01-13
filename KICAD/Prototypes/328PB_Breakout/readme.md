### Notes

 1) The fab resonator does not work. Tried both 8 MHz and 20 MHz - possible cause is that the capacitance of the resonator is too high. Trying replacement with lower capacitance (15 pF vs 22 pF)
 2) Programmable with Atmel Studio + Atmel ICE ISP after changing fuses to use internal RC oscillator
 3) Need to check if this fix works for AVRDUDE, etc.