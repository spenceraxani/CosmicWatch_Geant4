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
f_templates = ["../data/run0-side_nt_Event_t0.csv", "../data/run0-base_nt_Event_t0.csv"]

means_dic = {"base": 0, "side": 0}
color_dic = {"base": color_tab[0], "side": color_tab[1]}

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

ax1.grid()
ax1.set_title(label="Energy deposition")
ax1.set_xlabel("Energy [keV]")
ax1.set_ylabel("Counts")

ax2.grid()
ax2.legend()
ax2.set_title("Photon count")
ax2.set_xlabel("No of photons")
ax2.set_ylabel("Counts")

for f_template in f_templates:
    
    placement = None
    if "base" in f_template:
        placement = "base"
    elif "side" in f_template:
        placement = "side"

    data_frames = merge(runs, threads, columns, f_template)

    Events = len(data_frames[0].index)

    print("Tot events =", Events)

    data_frames[0] = data_frames[0].eval('Sum = Cerenkov+compt+CoupledTransportation+eBrem+eIon+msc+muIoni+phot')

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
            print(e, data_frames[0]["Tot"][e], data_frames[0]["Sum"][e], data_frames[0]["Diff"][e])

    print(counter, "events have an error greater than", threshold)

    weird_events = data_frames[0].index[data_frames[0]['Tot'] >= 662].tolist()

    tot_energy = [t for t in data_frames[0]['Tot'] if t>0]

    greatest = max(tot_energy)
    least = min(tot_energy)

    bin_size = (greatest-least)/500

    bins = np.arange(least, greatest+bin_size, bin_size)
    counts, _ = np.histogram(tot_energy, bins=bins)

    ax1.step(bins[:-1], counts, where="mid", label=placement)

    #------------------counting photons------------------#
    photons = data_frames[0]['NOfOptPhotons']
    greatest = max(photons)
    least = min(photons)

    bin_size = (greatest-least)/500
    bin_size = 1

    bins = np.arange(least, greatest+(bin_size/2.), bin_size)
    counts, _ = np.histogram(data_frames[0]['NOfOptPhotons'], bins=bins)

    means_dic[placement] = mean(bins, counts)

    label = placement+"-mean = "+str(means_dic[placement])
    ax2.axvline(means_dic[placement], label="mean at "+placement, color=color_dic[placement], ls=':')
    ax2.step(bins[:-1], counts, where="mid", label=placement, color=color_dic[placement])

ax1.legend()

save_file = re.sub("_t.*?\.", ".", f_templates[0])
save_file = re.sub("/data/", "/figures/", save_file)
save_file = re.sub("-side_|-base_", "_", save_file)
save_file = re.sub(".csv", "_energy_spectra.pdf", save_file)

print("saving to:", save_file)
fig1.savefig(save_file, bbox_inches="tight")

ax2.legend()

save_file = re.sub("_energy_spectra.pdf", ".pdf", save_file)
save_file = re.sub("\.pdf", "_photon_count.pdf", save_file)
    
print("saving to:", save_file)
fig2.savefig(save_file, bbox_inches="tight")