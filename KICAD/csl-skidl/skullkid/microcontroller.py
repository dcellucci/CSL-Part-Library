from skidl import * 
import json

class Microcontroller:
	def __init__(self):
		print("initializing")
		#Unless specified otherwise, 
		#clock pins are 0 and 1
		self.clockPins = [0,1]


	#We want to attach a clock
	#There are three types of clocks (roughly)
	# 1) a crystal, which is a two pin part with
	#    two external capacitors connected to a
	#    ground
	# 2) an oscillator, which has integrated caps
	# 3) an external clock reference 
	def attachClock(self,clockType='oscillator',part=None,caps=None):
		print("attaching clock")
		if(clockType == 'external'):
			self.part[self.clockPins[0]]+=clock['clockLine']
		elif(clockType == 'oscillator'):
			if(part == None):
				self.clock['part'] = Part(Fab,'OSC',)
			else:
				self.clock['part'] = clock['part']
			self.part[self.clockPins[0]] += self.clock['part'][0]
			self.part[self.clockPins[1]] += self.clock['part'][2]
		elif(clockType == 'crystal'):
			if(part == None):
				self.clock['part'] = Part(Fab,'XTAL',)
			else:
				self.clock['part'] = clock['part']
			self.part[self.clockPins[0]] += part[0]
			self.part[self.clockPins[1]] += part[2]
			self.xInNet = Net('xin')
			self.xOutNet = Net('xout')
			if(caps == None):
				self.xInCap = Part('Fab','CAP',footprint = '_Fab_Footprint:CAP_1206_FAB')
				self.xOutCap = Part('Fab','CAP',footprint = '_Fab_Footprint:CAP_1206_FAB')
			else:
				self.xInCap = caps[0]
				self.xOutCap = caps[1]



# A chip is a subclass of microcontroller 
class ATMEGA328PB(Microcontroller):
	pinMap = json.load(open('pinMaps/ATMEGA328PB.json'))
	def __init__(self):
		print("Creating a ATMEGA328PB uC")
		Microcontroller.__init__()
		self.clockPins = pinMap["clockPins"]
		Microcontroller.attachClock()
	def attachClock(self):
		




