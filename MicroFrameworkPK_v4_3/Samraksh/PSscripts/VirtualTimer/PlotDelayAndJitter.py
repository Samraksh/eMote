import matplotlib.pyplot as plt		
from pylab import * 				#For displaying grid
import math
import numpy as np					#For mean, stdDev



def CalcVarianceStdDev(data, meanOfData):	
	variance=[];
	
	for dat in data:
		res = dat - meanOfData;
		variance.append(res * res)
	
	finalVariance = mean(variance)	
	stdDev = math.sqrt(finalVariance)
	return (finalVariance, stdDev)
	


def CalcTimerDelay(delayFile, timerValue):
	global ListStartValue
	global ListEndValue
	global HistogramBinCount
	#global DelayValueFig
	timerDelay = []
	timerFinalValue = 'Timer' + str(timerValue)
	
	with open(delayFile,'r') as fName:
		content = fName.readlines()
		for line in content:
			line = line.strip()
			timerDelay.append(double(line))
	
	#w, h = figaspect(2.)
	#fig = Figure(figsize=(w,h))
	#plt.figure(1, figsize=(w,h))
	plt.figure(1, figsize=(23.8,13.35))
	plt.subplot(311)
	print ("--- %s : %s ---" %(timerFinalValue, DelayValue) )
		
	#Below if...else is because, if ListEndValue is 0, then python list cannot be sliced. 
	if ListEndValue == 0:
		meanOfDelay = np.mean(timerDelay[ListStartValue:])
		(finalVariance, stdDev) = CalcVarianceStdDev(timerDelay[ListStartValue:], meanOfDelay)
		PrintValues( 'delay', str(finalVariance), str(stdDev), str(meanOfDelay), str(min(timerDelay[ListStartValue:])), str(max(timerDelay[ListStartValue:])) )
		ylim( ( min(timerDelay[ListStartValue:])-stdDev, max(timerDelay[ListStartValue:])+stdDev ) )
		# A comma has to be added at the end of legend name, otherwise, below error is displayed
		# UserWarning: Legend does not support [<matplotlib.lines.Line2D object at 0x...
		# http://stackoverflow.com/questions/11983024/matplotlib-legends-not-working+&cd=2&hl=en&ct=clnk&gl=us&client=firefox-a
		timer1DelayLegend, = plt.plot(range(1,len(timerDelay[ListStartValue:])+1), timerDelay[ListStartValue:], 'r*-')
		yVal = [meanOfDelay] * len(timerDelay[ListStartValue:])
		timer1MeanDelayLegend, = plt.plot(range(1,len(timerDelay[ListStartValue:])+1), yVal, 'b+-')
	else:
		meanOfDelay = np.mean(timerDelay[ListStartValue:ListEndValue])
		(finalVariance, stdDev) = CalcVarianceStdDev(timerDelay[ListStartValue:ListEndValue], meanOfDelay)
		PrintValues( 'delay', str(finalVariance), str(stdDev), str(meanOfDelay), str(min(timerDelay[ListStartValue:ListEndValue])), str(max(timerDelay[ListStartValue:ListEndValue])) )
		ylim( ( min(timerDelay[ListStartValue:ListEndValue])-stdDev, max(timerDelay[ListStartValue:ListEndValue])+stdDev ) )
		# A comma has to be added at the end of legend name, otherwise, below error is displayed
		# UserWarning: Legend does not support [<matplotlib.lines.Line2D object at 0x...
		# http://stackoverflow.com/questions/11983024/matplotlib-legends-not-working+&cd=2&hl=en&ct=clnk&gl=us&client=firefox-a
		timer1DelayLegend, = plt.plot(range(1,len(timerDelay[ListStartValue:ListEndValue])+1), timerDelay[ListStartValue:ListEndValue], 'r*-')
		yVal = [meanOfDelay] * len(timerDelay[ListStartValue:ListEndValue])
		timer1MeanDelayLegend, = plt.plot(range(1,len(timerDelay[ListStartValue:ListEndValue])+1), yVal, 'b+-')
		
	plt.xlabel('Samples')
	plt.ylabel('Delay (microseconds)')
	plt.title( '%s - %s Delay' %(DelayValue, timerFinalValue) )
	plt.grid('on')
	plt.hold(True)
	timer1MeanDelayLegendValue = timerFinalValue + ' mean delay ' + str(meanOfDelay) + ' microseconds'
	figlegend( (timer1DelayLegend, timer1MeanDelayLegend),
			   (timerFinalValue + ' delay', timer1MeanDelayLegendValue),
				'upper right')
		
#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		
def CalcTimerJitter(jitterFile, timerValue):
	global ListStartValue
	global ListEndValue
	global HistogramBinCount
	#global DelayValueFig
	MaxJitter = 0
	timer1Jitter = []
	timerFinalValue = 'Timer' + str(timerValue)
	
	with open(jitterFile,'r') as fName:
		content = fName.readlines()
		for line in content:
			line = line.strip()
			timer1Jitter.append(double(line))
	
	plt.figure(1, figsize=(23.8,13.35))
	plt.subplot(312)
		
	if ListEndValue == 0:
		meanOfJitter = np.mean(timer1Jitter[ListStartValue:])
		(finalVariance, stdDev) = CalcVarianceStdDev(timer1Jitter[ListStartValue:], meanOfJitter)
		MaxJitter = max(timer1Jitter[ListStartValue:])
		PrintValues( 'jitter', str(finalVariance), str(stdDev), str(meanOfJitter), str(min(timer1Jitter[ListStartValue:])), str(max(timer1Jitter[ListStartValue:])) )
		ylim( ( min(timer1Jitter[ListStartValue:])-stdDev, max(timer1Jitter[ListStartValue:])+stdDev ) )
		timer1JitterLegend, = plt.plot(range(1,len(timer1Jitter[ListStartValue:])+1), timer1Jitter[ListStartValue:], 'r>-')
		yVal = [meanOfJitter] * len(timer1Jitter[ListStartValue:])
		timer1MeanJitterLegend, = plt.plot(range(1,len(timer1Jitter[ListStartValue:])+1), yVal, 'b+-')
	else:
		meanOfJitter = np.mean(timer1Jitter[ListStartValue:ListEndValue])
		(finalVariance, stdDev) = CalcVarianceStdDev(timer1Jitter[ListStartValue:ListEndValue], meanOfJitter)
		MaxJitter = max(timer1Jitter[ListStartValue:ListEndValue])
		PrintValues( 'jitter', str(finalVariance), str(stdDev), str(meanOfJitter), str(min(timer1Jitter[ListStartValue:ListEndValue])), str(max(timer1Jitter[ListStartValue:ListEndValue])) )
		ylim( ( min(timer1Jitter[ListStartValue:ListEndValue])-stdDev, max(timer1Jitter[ListStartValue:ListEndValue])+stdDev ) )
		timer1JitterLegend, = plt.plot(range(1,len(timer1Jitter[ListStartValue:ListEndValue])+1), timer1Jitter[ListStartValue:ListEndValue], 'r>-')
		yVal = [meanOfJitter] * len(timer1Jitter[ListStartValue:ListEndValue])
		timer1MeanJitterLegend, = plt.plot(range(1,len(timer1Jitter[ListStartValue:ListEndValue])+1), yVal, 'b+-')
		
	plt.xlabel('Samples')
	plt.ylabel('Jitter (microseconds)')
	plt.title('%s - %s Jitter' %(DelayValue, timerFinalValue) )
	plt.grid('on')
	plt.hold(True)
	timer1MeanJitterLegendValue = timerFinalValue + ' mean jitter ' + str(meanOfJitter) + ' microseconds' 
	figlegend( (timer1JitterLegend, timer1MeanJitterLegend),
			   (timerFinalValue + ' jitter', timer1MeanJitterLegendValue),
				'upper left')
	
	
	plt.subplot(313)
	if ListEndValue == 0:
		plt.hist(timer1Jitter[ListStartValue:], HistogramBinCount, range=[0, MaxJitter])
	else:
		plt.hist(timer1Jitter[ListStartValue:ListEndValue], HistogramBinCount, range=[0, MaxJitter])
		
	plt.xlabel('Jitter (microseconds)')
	plt.ylabel('Frequency')
	plt.title('%s - %s Jitter histogram' %(DelayValue, timerFinalValue) )
	plt.grid('on')
	figName = DelayValue + '-' + timerFinalValue + '.png'
	figLocation = BaseDir + DelayValue + '\\'
	finalFig = figLocation + figName
	plt.savefig(finalFig, bbox_inches = 'tight')
	#plt.show()
	plt.close()
	 
	#DelayValueFig = int(DelayValueFig) + 1 
	#DelayValueFig = str(DelayValueFig)

#-------------------------------------------------------------------------------------------------------------
'''
def CalcTimer2Delay(delayFile):
	global ListStartValue
	global ListEndValue
	global HistogramBinCount
	timer2Delay = []
	
	with open(delayFile,'r') as fName:
		content = fName.readlines()
		for line in content:
			line = line.strip()
			timer2Delay.append(double(line))
	
	plt.figure(2, figsize=(23.8,13.35))
	plt.subplot(311)
	print '--- Timer2 ---'
	
	if ListEndValue == 0:
		meanOfDelay = np.mean(timer2Delay[ListStartValue:])
		(finalVariance, stdDev) = CalcVarianceStdDev(timer2Delay[ListStartValue:], meanOfDelay)
		PrintValues( 'delay', str(finalVariance), str(stdDev), str(meanOfDelay), str(min(timer2Delay[ListStartValue:])), str(max(timer2Delay[ListStartValue:])) )
		ylim( ( min(timer2Delay[ListStartValue:])-stdDev, max(timer2Delay[ListStartValue:])+stdDev ) )
		timer2DelayLegend, = plt.plot(range(1,len(timer2Delay[ListStartValue:])+1), timer2Delay[ListStartValue:], 'r*-')
		yVal = [meanOfDelay] * len(timer2Delay[ListStartValue:])
		timer2MeanDelayLegend, = plt.plot(range(1,len(timer2Delay[ListStartValue:])+1), yVal, 'b+-')
	else:
		meanOfDelay = np.mean(timer2Delay[ListStartValue:ListEndValue])
		(finalVariance, stdDev) = CalcVarianceStdDev(timer2Delay[ListStartValue:ListEndValue], meanOfDelay)
		PrintValues( 'delay', str(finalVariance), str(stdDev), str(meanOfDelay), str(min(timer2Delay[ListStartValue:ListEndValue])), str(max(timer2Delay[ListStartValue:ListEndValue])) )
		ylim( ( min(timer2Delay[ListStartValue:ListEndValue])-stdDev, max(timer2Delay[ListStartValue:ListEndValue])+stdDev ) )
		timer2DelayLegend, = plt.plot(range(1,len(timer2Delay[ListStartValue:ListEndValue])+1), timer2Delay[ListStartValue:ListEndValue], 'r*-')
		yVal = [meanOfDelay] * len(timer2Delay[ListStartValue:ListEndValue])
		timer2MeanDelayLegend, = plt.plot(range(1,len(timer2Delay[ListStartValue:ListEndValue])+1), yVal, 'b+-')
		
	plt.xlabel('Samples')
	plt.ylabel('Delay (microseconds)')
	plt.title('%s - Timer2 Delay' %DelayValue)
	plt.grid('on')
	plt.hold(True)
	timer2MeanDelayLegendValue = 'Timer2 mean delay ' + str(meanOfDelay) + ' microseconds' 
	figlegend( (timer2DelayLegend, timer2MeanDelayLegend),
			   ('Timer2 delay', timer2MeanDelayLegendValue),
				'upper right')

#^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	
def CalcTimer2Jitter(jitterFile):
	global ListStartValue
	global ListEndValue
	global HistogramBinCount
	MaxJitter = 0
	timer2Jitter = []
	
	with open(jitterFile,'r') as fName:
		content = fName.readlines()
		for line in content:
			line = line.strip()
			timer2Jitter.append(double(line))
	
	plt.figure(2, figsize=(23.8,13.35))
	plt.subplot(312)
	
	if ListEndValue == 0:
		meanOfJitter = np.mean(timer2Jitter[ListStartValue:])
		(finalVariance, stdDev) = CalcVarianceStdDev(timer2Jitter[ListStartValue:], meanOfJitter)
		MaxJitter = max(timer2Jitter[ListStartValue:])
		PrintValues( 'jitter', str(finalVariance), str(stdDev), str(meanOfJitter), str(min(timer2Jitter[ListStartValue:])), str(max(timer2Jitter[ListStartValue:])) )
		ylim( ( min(timer2Jitter[ListStartValue:])-stdDev, max(timer2Jitter[ListStartValue:])+stdDev ) )
		timer2JitterLegend, = plt.plot(range(1,len(timer2Jitter[ListStartValue:])+1), timer2Jitter[ListStartValue:], 'r>-')
		yVal = [meanOfJitter] * len(timer2Jitter[ListStartValue:])
		timer2MeanJitterLegend, = plt.plot(range(1,len(timer2Jitter[ListStartValue:])+1), yVal, 'b+-')
	else:
		meanOfJitter = np.mean(timer2Jitter[ListStartValue:ListEndValue])
		(finalVariance, stdDev) = CalcVarianceStdDev(timer2Jitter[ListStartValue:ListEndValue], meanOfJitter)
		MaxJitter = max(timer2Jitter[ListStartValue:ListEndValue])
		PrintValues( 'jitter', str(finalVariance), str(stdDev), str(meanOfJitter), str(min(timer2Jitter[ListStartValue:ListEndValue])), str(max(timer2Jitter[ListStartValue:ListEndValue])) )
		ylim( ( min(timer2Jitter[ListStartValue:ListEndValue])-stdDev, max(timer2Jitter[ListStartValue:ListEndValue])+stdDev ) )
		timer2JitterLegend, = plt.plot(range(1,len(timer2Jitter[ListStartValue:ListEndValue])+1), timer2Jitter[ListStartValue:ListEndValue], 'r>-')
		yVal = [meanOfJitter] * len(timer2Jitter[ListStartValue:ListEndValue])
		timer2MeanJitterLegend, = plt.plot(range(1,len(timer2Jitter[ListStartValue:ListEndValue])+1), yVal, 'b+-')
		
	plt.xlabel('Samples')
	plt.ylabel('Jitter (microseconds)')
	plt.title('%s - Timer2 Jitter' %DelayValue)
	plt.grid('on')
	plt.hold(True)
	timer2MeanJitterLegendValue = 'Timer2 mean jitter ' + str(meanOfJitter) + ' microseconds' 
	figlegend( (timer2JitterLegend, timer2MeanJitterLegend),
			   ('Timer2 jitter', timer2MeanJitterLegendValue),
				'upper left')
	
	plt.subplot(313)
	if ListEndValue == 0:
		plt.hist(timer2Jitter[ListStartValue:], bins=HistogramBinCount, range=[0, MaxJitter])
	else:
		plt.hist(timer2Jitter[ListStartValue:ListEndValue], bins=HistogramBinCount, range=[0, MaxJitter])
		
	plt.xlabel('Jitter (microseconds)')
	plt.ylabel('Frequency')
	plt.title('%s - Timer2 Jitter histogram' %DelayValue)
	plt.grid('on')
	figName = DelayValue + '-Timer2.png'
	figLocation = BaseDir + DelayValue + '\\'
	finalFig = figLocation + figName
	plt.savefig(finalFig, bbox_inches = 'tight')
	plt.show()
	plt.close()
	
'''
#*****************************************************************************************************

def PrintValues(strValue, finalVariance, stdDev, meanValue, minValue, maxValue):
	print 'Values for ' + strValue + ':'
	print 'Variance is: ' + finalVariance
	print 'stdDev is: ' + stdDev
	print 'Mean is: ' + meanValue
	print 'min is: ' + minValue
	print 'max is: ' + maxValue
	print ''

#-----------------------------------------

BaseDir = 'D:\\AnanthAtSamraksh\\VirtualTimerChanges\\Baseline\\AdvancedTimerBaseLine\\'

#DelayValue = '250us'
#DelayValue = '500us'
DelayValue = '1ms'
#DelayValue = '500ms'
#DelayValue = '250ms'

#DelayValueTo = '-8ms'
#DelayValueFig = 1

ListStartValue = 0
ListEndValue = 0
HistogramBinCount = 0


def Initialize():
	global ListStartValue
	global ListEndValue
	global HistogramBinCount
	
	if DelayValue == '500ms':
		ListStartValue = 0
		ListEndValue = 0
		HistogramBinCount = 100
	elif DelayValue == '250ms':
		ListStartValue = 0
		ListEndValue = 0
		HistogramBinCount = 100
	elif DelayValue == '100ms':
		ListStartValue = 10
		ListEndValue = -10
		HistogramBinCount = 100
	elif DelayValue == '50ms':
		ListStartValue = 0
		ListEndValue = 0
		HistogramBinCount = 100
	elif DelayValue == '10ms':
		ListStartValue = 15
		ListEndValue = 0
		HistogramBinCount = 100
	elif DelayValue == '1ms':
		ListStartValue = 15
		ListEndValue = 0
		HistogramBinCount = 100
	elif DelayValue == '500us':
		ListStartValue = 50
		ListEndValue = 0
		HistogramBinCount = 100
	elif DelayValue == '250us':
		ListStartValue = 50
		ListEndValue = 0
		HistogramBinCount = 100

	
def main():
	Timer0DelayFile = '\\' + DelayValue + '-Delay_Timer0.txt'
	Timer0JitterFile = '\\' + DelayValue + '-Jitter_Timer0.txt'
	'''
	Timer1DelayFile = '\\' + '\\Timer0-7\\Timer0-7-Delay_Timer1.txt'
	Timer1JitterFile = '\\' + '\\Timer0-7\\Timer0-7-Jitter_Timer1.txt'
	Timer2DelayFile = '\\' + '\\Timer0-7\\Timer0-7-Delay_Timer2.txt'
	Timer2JitterFile = '\\' + '\\Timer0-7\\Timer0-7-Jitter_Timer2.txt'
	Timer3DelayFile = '\\' + '\\Timer0-7\\Timer0-7-Delay_Timer3.txt'
	Timer3JitterFile = '\\' + '\\Timer0-7\\Timer0-7-Jitter_Timer3.txt'
	Timer4DelayFile = '\\' + '\\Timer0-7\\Timer0-7-Delay_Timer4.txt'
	Timer4JitterFile = '\\' + '\\Timer0-7\\Timer0-7-Jitter_Timer4.txt'
	Timer5DelayFile = '\\' + '\\Timer0-7\\Timer0-7-Delay_Timer5.txt'
	Timer5JitterFile = '\\' + '\\Timer0-7\\Timer0-7-Jitter_Timer5.txt'
	Timer6DelayFile = '\\' + '\\Timer0-7\\Timer0-7-Delay_Timer6.txt'
	Timer6JitterFile = '\\' + '\\Timer0-7\\Timer0-7-Jitter_Timer6.txt'
	Timer7DelayFile = '\\' + '\\Timer0-7\\Timer0-7-Delay_Timer7.txt'
	Timer7JitterFile = '\\' + '\\Timer0-7\\Timer0-7-Jitter_Timer7.txt'
	'''
	
	'''
	CalcTimer1Delay(BaseDir + DelayValue + Timer1DelayFile)
	CalcTimer1Jitter(BaseDir + DelayValue + Timer1JitterFile)
	CalcTimer2Delay(BaseDir + DelayValue + Timer2DelayFile)
	CalcTimer2Jitter(BaseDir + DelayValue + Timer2JitterFile)
	'''
	
	#CalcTimerDelay 'Timer/Jitter file' 'Timer'
	CalcTimerDelay(BaseDir + DelayValue + Timer0DelayFile, 0)
	CalcTimerJitter(BaseDir + DelayValue + Timer0JitterFile, 0)
	'''
	CalcTimerDelay(BaseDir + DelayValue + Timer1DelayFile, 1)
	CalcTimerJitter(BaseDir + DelayValue + Timer1JitterFile, 1)
	CalcTimerDelay(BaseDir + DelayValue + Timer2DelayFile, 2)
	CalcTimerJitter(BaseDir + DelayValue + Timer2JitterFile, 2)
	CalcTimerDelay(BaseDir + DelayValue + Timer3DelayFile, 3)
	CalcTimerJitter(BaseDir + DelayValue + Timer3JitterFile, 3)
	CalcTimerDelay(BaseDir + DelayValue + Timer4DelayFile, 4)
	CalcTimerJitter(BaseDir + DelayValue + Timer4JitterFile, 4)
	CalcTimerDelay(BaseDir + DelayValue + Timer5DelayFile, 5)
	CalcTimerJitter(BaseDir + DelayValue + Timer5JitterFile, 5)
	CalcTimerDelay(BaseDir + DelayValue + Timer6DelayFile, 6)
	CalcTimerJitter(BaseDir + DelayValue + Timer6JitterFile, 6)
	CalcTimerDelay(BaseDir + DelayValue + Timer7DelayFile, 7)
	CalcTimerJitter(BaseDir + DelayValue + Timer7JitterFile, 7)
	'''
	

if __name__ == "__main__":
	Initialize()
	main()
	
	
	