#Library Import
from skidl import * 
import sys
import json

settings = json.load(open('../../../local/settings.json'))
for path in settings["kicad_lib_search_paths"]:
	lib_search_paths_kicad.append(path)

############
######Global Nets
############

gnd = Net('GND')
vcc = Net('VCC')

#### BOM
# ATSAMD11D
# Decoupling Caps (2)
# Crystal
# Crystal Caps(2)
# 

############
######Central Microprocessor
############

#define
atsamd11d = Part('CSL_uP','ATSAMD11D')
atsamd11d.footprint = 'Housings_SOIC:SOIC-20W_7.5x12.8mm_Pitch1.27'
#connect global nets
atsamd11d[17]+=vcc
atsamd11d[16]+=gnd

############
######Ancillary Components
############

#Decoupling capacitor between VDDIN and GND
cin1,cin2 = 2*Part('Fab','CAP',footprint = '_Fab_Footprint:CAP_1206_FAB')

cin1.value = '4.7uF'
cin1[0]+=vcc
cin1[1]+=gnd

cin2.value = '100nF'
cin2[0]+=vcc
cin2[1]+=gnd

############
###### Oscillator Circuit
############
xin = Net('XIN')
xout = Net('XOUT')
atsamd11d[3] += xin
atsamd11d[4] += xout

#Crystal
x1 = Part('Fab','ABS25-4-SOJ',footprint='_Fab_Footprint:4-SOJ')
x1[0] += xin
x1[3] += xout

#Crystal Caps
cxin,cxout = 2*Part('Fab','CAP', value='22pf',footprint='_Fab_Footprint:CAP_1206_FAB')

cxin[0]+=xin
cxin[1]+=gnd

cxout[0]+=xout
cxout[1]+=gnd









