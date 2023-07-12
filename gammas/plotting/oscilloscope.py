import pandas as pd
import matplotlib.pyplot as plt

import re

data_files = ['../data/Na22-20min.Wfm.csv', '../data/Na22-20min-background.Wfm.csv']
setup_files = ['../data/Na22-20min.csv', '../data/Na22-20min-background.csv']

dFrames = list()

XStart = list()
XStop = list()
Resolution = list()
NBins = list()

for i, file in enumerate(setup_files):
	with open(file, 'r', encoding="utf-8") as fhandle:
		for line in fhandle:
			read_in = re.search('(?<=XStart:)(.*?)(?=:)', line)
			if read_in:
				XStart.append(float(read_in.group(0))*1e9)
				continue

			read_in = re.search('(?<=XStop:)(.*?)(?=:)', line)
			if read_in:
				XStop.append(float(read_in.group(0))*1e9)
				continue

			read_in = re.search('(?<=Resolution:)(.*?)(?=:)', line)
			if read_in:
				Resolution.append(float(read_in.group(0))*1e9)
				continue

			read_in = re.search('(?<=NumberOfBins:)(.*?)(?=:)', line)
			if read_in:
				NBins.append(int(read_in.group(0)))

	dFrames.append(pd.read_csv(data_files[i], names=['counts']))

x = [(i+0.5)*Resolution[0]+XStart[0] for i in range(NBins[0])]

if XStart[0]!=XStart[1]:
	print('XStart difference')
if XStop[0]!=XStop[1]:
	print('XStart difference')
if Resolution[0]!=Resolution[1]:
	print('XStart difference')
if NBins[0]!=NBins[1]:
	print('XStart difference')

index = dFrames[0]['counts'].idxmax()
print(index)

vmax = (index*Resolution[0]+XStart[0])
print(vmax)

data = (dFrames[0]['counts']-dFrames[1]['counts']).to_frame()

#----------ploting----------#
fig, ax = plt.subplots()

labels = ['Na22', 'bgnd']
for i, dframe in enumerate(dFrames):
	plt.step(x, dframe['counts'], label=labels[i])

#vertical line at mean
plt.axvline(x=vmax, linestyle=':', label="vmax$=$"+str(vmax))

plt.grid()
plt.xlabel('SiPM pulse area [nV/s]')
plt.ylabel('events / ('+str( round(Resolution[0], 3) )+' nV/s)' )

plt.savefig("../figures/raw_LYSO_Na22_20min.pdf", bbox_inches="tight")
plt.clf()

fig, ax = plt.subplots()

plt.step(x, data['counts'], label='Na22-bgnd')

plt.grid()
plt.xlabel('SiPM pulse area [nV/s]')
plt.ylabel('events / ('+str( round(Resolution[0], 3) )+' nV/s)' )

plt.savefig("../figures/LYSO_Na22_20min.pdf", bbox_inches="tight")