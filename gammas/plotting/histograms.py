import matplotlib as mpl

fsize = 18
mpl.rcParams['legend.fontsize'] = fsize
mpl.rcParams["figure.figsize"] = (6,5)
mpl.rcParams['axes.labelsize'] = fsize
mpl.rcParams['xtick.labelsize'] = fsize
mpl.rcParams['ytick.labelsize'] = fsize
mpl.rcParams['text.usetex'] = True
mpl.rcParams['font.family'] = 'sans-serif'
mpl.rcParams['mathtext.fontset'] = 'dejavusans'
mpl.rcParams.update({'font.size': fsize})

import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

import re
from pathlib import Path

#list of isotopes used
isotopes = ['Co60', 'Cs137']
isotope_labels = {'Co60': "$^{60}$Co", 'Cs137': "$^{137}$Cs"}
#list of source-detector distances used [cm]
distances = [25]
#list of measuring times [min]
times = [10, 30, 60, 120]

#template of data file names
file_template = "../data/Co60-25cm-10min.txt"

for isotope in isotopes:

	#using regex to change isotope name
	file_template = re.sub("[/]+\w+\-+", "/"+isotope+"-", file_template)	

	fig, ax = plt.subplots()

	for d in distances:

		#using regex to change distance to source
		file_template = re.sub("\d+cm", str(d)+"cm", file_template)

		for t in times:

			#using regex to change measuring time
			file_template = re.sub("\d+min", str(t)+"min", file_template)

			#check file exists
			path = Path(file_template)
			if path.is_file():
				print("reading: "+file_template)
			else:
				print("No such file or directory: "+file_template)
				continue

			#extracting data
			pattern = re.compile(r'^\d.+\d$')
			events = list()

			with open(file_template, 'r', encoding="utf-8") as fhandle:
				try:
					for line in fhandle:
						if re.match(pattern, line):
							line = line.strip('\n')
							events.append(list(map(float, line.split()[2:-1])))
				except:
					#utf-8 decoding problem in some files
					print(file_template, "could not be read")
			
			data = pd.DataFrame(events, columns=['Event', 'Ardn_time[ms]', 'ADC[0-1023]', 'SiPM[mV]', 'Deadtime[ms]'])
			data = data.set_index(['Event'])

			hist = np.histogram(data['SiPM[mV]'], bins=100, range=(13, 100), density=None, weights=None)

			plt.step(hist[1][:-1], hist[0], where='mid', label=str(t)+" min")

	#----------ploting----------#
	plt.title(isotope_labels[isotope]+" "+str(d)+" cm")
	plt.xlabel("SiPM [mV]")
	plt.ylabel("Cuentas")

	plt.grid()

	plt.legend()

	plt.savefig("../figures/pscint_"+isotope+"_"+str(d)+"cm.pdf", bbox_inches="tight")
	plt.clf()