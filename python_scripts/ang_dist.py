import matplotlib as mpl

fsize = 18
mpl.rcParams['legend.fontsize'] = 14
mpl.rcParams["figure.figsize"] = (6,5)
mpl.rcParams['axes.labelsize'] = fsize
mpl.rcParams['xtick.labelsize'] = fsize
mpl.rcParams['ytick.labelsize'] = fsize
mpl.rcParams['text.usetex'] = True
mpl.rcParams['font.family'] = 'sans-serif'
mpl.rcParams['mathtext.fontset'] = 'dejavusans'
mpl.rcParams.update({'font.size': fsize})

#list of matplotlib colors
color_tab = list(mpl.colors.TABLEAU_COLORS.keys())

import matplotlib.pyplot as plt
import numpy as np
import re

#read data
runs = int(input("enter number of runs: "))
threads = int(input("enter number of threads: "))
columns = int(input("enter number of columns: "))

from merge_threads import merge

#merge data in imput files
f_templates = ["../data/run0-ang_dis_nt_Event_t0.csv"]

#means_dic = {"5x5x1": 0, "10x5x2": 0}
#color_dic = {"5x5x1": color_tab[0], "10x5x2": color_tab[1]}

def mean(bins, freq):
    dx = bins[1]-bins[0]

    events = sum(freq)

    f_b_sum = 0
    for i, f in enumerate(freq):
        f_b_sum += bins[i]*f
    
    v = f_b_sum/events
    print(events, "in histogram, mean =", f_b_sum/events)

    return v

fig1, ax1 = plt.subplots()
fig2, ax2 = plt.subplots()

major_ticks = np.arange(0, 181, 30)

ax1.grid()
ax1.set_xticks(major_ticks)
ax1.set_title("Energy deposition")
ax1.set_xlabel(r"zenith angle $\theta$ [$^\circ$]")
ax1.set_ylabel("Average energy deposit per event [keV]")

ax2.grid()
ax2.set_xticks(major_ticks)
ax2.legend()
ax2.set_title("Photon count")
ax2.set_xlabel(r"zenith angle $\theta$ [$^\circ$]")
ax2.set_ylabel("Average photon counts per event")

for f_template in f_templates:
    
    dimensions = None
    if "5x5x1" in f_template:
        dimensions = "5x5x1"
    elif "10x5x2" in f_template:
        dimensions = "10x5x2"

    data_frames = merge(runs, threads, columns, f_template)

    Events = len(data_frames[0].index)

    print("Tot events =", Events)

    data_frames[0] = data_frames[0].eval('Sum = Cerenkov+compt+CoupledTransportation+eBrem+eIon+msc+muIoni+phot')

    data_frames[0]["theta"] = [np.degrees(np.arccos(z/(x**2+y**2+z**2))) for x,y,z in zip(data_frames[0]["Px"], data_frames[0]["Py"], data_frames[0]["Pz"])]

    def err(row):
        return np.abs(row.Tot-row.Sum)/row.Tot

    diff_list  = [(t-s)/t if t>0 else 0 for t, s in zip(data_frames[0]["Tot"], data_frames[0]["Sum"])]

    data_frames[0]["Diff"] = diff_list
    #data_frames[0]["Diff"] = data_frames[0].apply(err, axis=1) 
    #data_frames[0] = data_frames[0].eval('Diff = (Tot - Sum)/Tot') 

    print(data_frames[0]["Diff"])

    counter = 0
    threshold = 0.001
    for e in range(Events):
        if np.abs(data_frames[0]["Diff"][e]) >= threshold:
            counter += 1
            print(e, data_frame1[0]["Tot"][e], data_frames[0]["Sum"][e], data_frames[0]["Diff"][e])

    print(counter, "events have an error greater than", threshold)

    weird_events = data_frames[0].index[data_frames[0]['Tot'] >= 662].tolist()

    tot_energy = [t for t in data_frames[0]['Tot'] if t>0]

    greatest = max(tot_energy)
    least = min(tot_energy)

    bin_size = (greatest-least)/500

    bins = np.arange(least, greatest+bin_size, bin_size)
    counts, _ = np.histogram(tot_energy, bins=bins)

    #ax1.step(bins[:-1], counts, where="mid", label=dimensions)

    #------------------counting photons------------------#
    t_bin_size = 2
    t_bins = np.arange(0, 180, t_bin_size)
    
    photons = [[] for _ in range(len(t_bins))]
    energies = [[] for _ in range(len(t_bins))]
    print(photons)
    print(len(t_bins))

    for t, t_min in enumerate(t_bins):
        t_events = data_frames[0].index[(t_min <= data_frames[0]['theta']) & (data_frames[0]['theta'] < t_min+t_bin_size)].tolist()
        #t_events = data_frames[0][t_min <= data_frames[0]['theta'] & data_frames[0]['theta'] < t+t_bin_size].tolist()

        photons[t] = data_frames[0].loc[t_events, 'NOfOptPhotons'].tolist()
        if len(photons[t]) > 0:
            photons[t] = sum(photons[t])/len(photons[t])
        else:
            photons[t] = 0

        energies[t] = data_frames[0].loc[t_events, 'Tot'].tolist()
        if len(energies[t]) > 0:
            energies[t] = sum(energies[t])/len(energies[t])
        else:
            energies[t] = 0

    #photons = data_frames[0]['NOfOptPhotons']
    #greatest = max(photons)
    #least = min(photons)

    #bin_size = (greatest-least)/500
    #bin_size = 1

    #bins = np.arange(least, greatest+(bin_size/2.), bin_size)
    #counts, _ = np.histogram(data_frames[0]['NOfOptPhotons'], bins=bins)

    #means_dic[dimensions] = mean(bins, counts)

    #label = dimensions+"-mean = "+str(means_dic[dimensions])
    #ax2.axvline(means_dic[dimensions], label="mean at "+dimensions, color=color_dic[dimensions], ls=':')

    photons2 = [p*(np.cos(np.radians(t_p))**2) for p, t_p in zip(photons, t_bins+t_bin_size/2)]
    energies2 = [e*(np.cos(np.radians(t_p))**2) for e, t_p in zip(energies, t_bins+t_bin_size/2)]

    ax1.plot(t_bins+t_bin_size/2, energies, label="energy deposit")
    ax1.plot(t_bins+t_bin_size/2, energies2, label=r"energy deposit$\cdot\cos^2$")

    ax2.plot(t_bins+t_bin_size/2, photons, label="Photon count")
    ax2.plot(t_bins+t_bin_size/2, photons2, label=r"Photon count$\cdot\cos^2$")

ax1.legend()

save_file = re.sub("_t.*?\.", ".", f_templates[0])
save_file = re.sub("/data/", "/figures/", save_file)
#save_file = re.sub("-ang_dis_", "_", save_file)
save_file = re.sub(".csv", "_energy_spectra.pdf", save_file)

print("saving to:", save_file)
fig1.savefig(save_file, bbox_inches="tight")

ax2.legend()

save_file = re.sub("_energy_spectra.pdf", "_photon_count.pdf", save_file)
#save_file = re.sub("\.pdf", "_photon_count-SiPM-placement.pdf", save_file)

print("saving to:", save_file)
fig2.savefig(save_file, bbox_inches="tight")