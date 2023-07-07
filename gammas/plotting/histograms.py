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

#parameters
vmin = 13
vmax = 100
nbins = 50
dv = round((vmax-vmin)/nbins, 2)

#list of isotopes used
isotopes = ['Co60', 'Cs137']
isotope_labels = {'Co60': "$^{60}$Co", 'Cs137': "$^{137}$Cs"}
#list of source-detector distances used [cm]
distances = [15, 25]
#list of measuring times [min]
times = [10, 30, 60, 120, 180, 840]

#template of data file names
file_template = "../data/Co60-25cm-10min.txt"

#list of matplotlib colors
color_tab = list(mpl.colors.TABLEAU_COLORS.keys())

#----------mean calculation----------#
def Mean(bin_centers, histogram):
	#total events recorded
	total_events = sum(histogram)

	#voltage measured * it's frecuency
	rel_heights = sum(f*v for f, v in zip(hist, bin_centers))

	return rel_heights/total_events

#----------sdev calculation----------#
def Sigma(bin_centers, mean):
	#sum of deviations from the mean
	differences = sum((v-mean)**2 for v in bin_centers)

	return np.sqrt(differences/nbins)

for isotope in isotopes:

	#using regex to change isotope name
	file_template = re.sub("[/]+\w+\-+", "/"+isotope+"-", file_template)	

	for d in distances:

		fig, ax = plt.subplots()

		#using regex to change distance to source
		file_template = re.sub("\d+cm", str(d)+"cm", file_template)

		i = -1 #index to keep track of colors in plotting
		for t in times:

			#using regex to change measuring time
			file_template = re.sub("\d+min", str(t)+"min", file_template)

			#check file exists
			path = Path(file_template)
			if path.is_file():
				print("reading: "+file_template)
				i += 1 #increase color index if file exists
			else:
				print("No such file or directory: "+file_template)
				continue

			#----------extracting data----------#
			#read only lines that start and end with numbers
			pattern = re.compile(r'^\d.+\d$')
			events = list()

			with open(file_template, 'r', encoding="utf-8") as fhandle:
				#try:
				for line in fhandle:
					if re.match(pattern, line):
						line = line.strip('\n')
						events.append(list(map(float, line.split()[2:-1])))
				'''except:
					#utf-8 decoding problem in some files
					print(file_template, "could not be read")'''
			
			#turn data into dataframe
			data = pd.DataFrame(events, columns=['Event', 'Ardn_time[ms]', 'ADC[0-1023]', 'SiPM[mV]', 'Deadtime[ms]'])
			data = data.set_index(['Event'])

			#build the histogram
			hist, bin_edges = np.histogram(data['SiPM[mV]'], bins=nbins, range=(vmin, vmax), density=None, weights=None)

			#----------statistics----------#
			hmax = max(hist) #max value in the histogram

			norm_hist = hist/hmax #histogram normalized to hmax

			hmax_rel_err = np.sqrt(hmax)/hmax #relative error at hmax bin

			hist_rel_err = [np.sqrt(h)/h if h else 0 for h in hist] #relative error at each bin

			norm_hist_err = [0]*nbins #error of event counts in the normalized histogram
			for j in range(nbins):
				norm_hist_err[j] = (hist[j]/hmax)*np.sqrt(hist_rel_err[j]**2 + hmax_rel_err**2)

			bin_centers = 0.5*(bin_edges[:-1] + bin_edges[1:])

			mean = Mean(bin_centers, hist)

			sigma = Sigma(bin_centers, mean)

			mean_err = sigma/np.sqrt(nbins)

			#----------plotting----------#
			#histogram
			plt.step(bin_centers, norm_hist, where='mid', label=str(t)+" min", color=color_tab[i])

			#error bars in event count
			plt.fill_between(bin_centers, norm_hist-norm_hist_err, norm_hist+norm_hist_err, step='mid', alpha=0.5, color=color_tab[i])

			#vertical line at mean
			plt.axvline(x=mean, linestyle=':', label='mean', color=color_tab[i])

			#shaded area between mean standard errors
			ax.axvspan(mean-mean_err, mean+mean_err, alpha=0.2, color=color_tab[i])

		#----------ploting----------#
		plt.title(isotope_labels[isotope]+" "+str(d)+" cm")
		plt.xlabel("SiPM peak voltage [mV]")
		plt.ylabel("Events/"+str(dv)+" mV")

		plt.grid(zorder=-3)

		plt.legend()

		plt.savefig("../figures/pscint_"+isotope+"_"+str(d)+"cm.pdf", bbox_inches="tight")
		plt.clf()