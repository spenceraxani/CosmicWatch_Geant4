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
#f_templates = ["../data/run0-side_nt_Event_t0.csv", "../data/run0-base_nt_Event_t0.csv"]
#f_templates = ["../data/run0_5x5x1-nt_Event_t0.csv", "../data/run0_10x5x2-nt_Event_t0.csv"]
#f_templates = ["../data/LYSO_run0_3x3x20-base_nt_Event_t0.csv", "../data/LYSO_run0_4x4x22-base_nt_Event_t0.csv", "../data/LYSO_run0_10x10x20-base_nt_Event_t0.csv"]
#f_templates = ["../data/LYSO_0,5doping_run0_3x3x20-base_nt_Event_t0.csv", "../data/LYSO_0,5doping_run0_4x4x22-base_nt_Event_t0.csv", "../data/LYSO_0,5doping_run0_10x10x20-base_nt_Event_t0.csv"]
f_templates = ["../data/PScint_scintPhot_run0_3x3x20-base_nt_Event_t0.csv", "../data/PScint_scintPhot_run0_4x4x22-base_nt_Event_t0.csv", "../data/PScint_scintPhot_run0_10x10x20-base_nt_Event_t0.csv"]

#material = "Plastic_Scint"
#means_dic = {"5x5x1": 0, "10x5x2": 0}
#color_dic = {"5x5x1": color_tab[0], "10x5x2": color_tab[1]}
material = "LYSO"
means_dic = {"4x4x22": 0, "3x3x20": 0, "10x10x20": 0}
color_dic = {"3x3x20": color_tab[0], "4x4x22": color_tab[1], "10x10x20": color_tab[2]}

config = {"LYSO/scintPhot/": {"runs": 1, "threads": 4, "columns": 16},
		"LYSO/0,5doping/": {"runs": 1, "threads": 4, "columns": 15},
		"LYSO/0.19doping/": {"runs": 1, "threads": 4, "columns": 15},
		"PScint/ang_dist/": {"runs": 1, "threads": 4, "columns": 15},
        "PScint/crystal_size/": {"runs": 1, "threads": 4, "columns": 15},
        "PScint/SiPM_Placement/": {"runs": 1, "threads": 4, "columns": 15}}
material =  {"LYSO/scintPhot/": "LYSO",
            "LYSO/0,5doping/": "LYSO",
            "LYSO/0.19doping/": "LYSO",
            "PScint/ang_dist/": "G4_PLASTIC_SC_VINYLTOLUENE",
            "PScint/crystal_size/": "G4_PLASTIC_SC_VINYLTOLUENE",
            "PScint/SiPM_Placement/": "G4_PLASTIC_SC_VINYLTOLUENE"}
data_type = {"LYSO/scintPhot/": "LYSO",
            "LYSO/0,5doping/": "LYSO",
            "LYSO/0.19doping/": "LYSO",
            "PScint/ang_dist/": "G4_PLASTIC_SC_VINYLTOLUENE",
            "PScint/crystal_size/": "G4_PLASTIC_SC_VINYLTOLUENE",
            "PScint/SiPM_Placement/": "G4_PLASTIC_SC_VINYLTOLUENE"}

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
ax2.set_title("Photon count")
ax2.set_xlabel("No of photons")
ax2.set_ylabel("Counts")

greatest_en = 0
for f_template in f_templates:
    
    dimensions = None
    if "4x4x22" in f_template:
        dimensions = "4x4x22"
    elif "3x3x20" in f_template:
        dimensions = "3x3x20"
    elif "10x10x20" in f_template:
        dimensions = "10x10x20"

    data_frames = merge(runs, threads, columns, f_template)
    print(len(data_frames))

    Events = len(data_frames[0].index)

    print("Tot events =", Events)

    data_frames[0] = data_frames[0].eval('Sum = Cerenkov+compt+CoupledTransportation+eBrem+eIon+msc+muIoni+phot')

    def err(row):
        return np.abs(row.Tot-row.Sum)/row.Tot

    diff_list  = [(t-s)/t if t>0 else 0 for t, s in zip(data_frames[0]["Tot-OpAbs"], data_frames[0]["Sum"])]

    data_frames[0]["Diff"] = diff_list
    #data_frames[0]["Diff"] = data_frames[0].apply(err, axis=1) 
    #data_frames[0] = data_frames[0].eval('Diff = (Tot - Sum)/Tot') 

    print(data_frames[0]["Tot-OpAbs"])

    counter = 0
    threshold = 0.001
    for e in range(Events):
        if np.abs(data_frames[0]["Diff"][e]) >= threshold:
            counter += 1
            print(e, data_frames[0]["Tot-OpAbs"][e], data_frames[0]["Sum"][e], data_frames[0]["Diff"][e])

    weird_events = data_frames[0].index[data_frames[0]["Tot-OpAbs"] >= 662].tolist()

    tot_energy = [t for t in data_frames[0]["Tot-OpAbs"] if t>0]

    print(counter, "events have an error greater than", threshold)
    if len(weird_events):
        print(len(weird_events), "events above photopeak", weird_events[0])

    #greatest = max(tot_energy)
    #least = min(tot_energy)
    greatest_en = max(data_frames[0]["Tot-OpAbs"])
    least = min(data_frames[0]["Tot-OpAbs"])

    bin_size = (greatest_en-least)/500

    bins = np.arange(least, greatest_en+bin_size, bin_size)
    #counts, _ = np.histogram(tot_energy, bins=bins)
    counts, _ = np.histogram(data_frames[0]["Tot-OpAbs"], bins=bins)

    ax1.step(bins[:-1], counts, where="mid", label=dimensions, color=color_dic[dimensions])

    #------------------counting photons------------------#
    photons = data_frames[0]['NOfOptPhotons']
    #print(photons)
    greatest = max(photons)
    #print(greatest)
    least = min(photons)
    #print(least)

    bin_size = (greatest-least)/500
    bin_size = 2

    bins = np.arange(least, greatest+(bin_size/2.), bin_size)
    counts, _ = np.histogram(data_frames[0]['NOfOptPhotons'], bins=bins)

    means_dic[dimensions] = mean(bins, counts)

    label = dimensions+"-mean = "+str(means_dic[dimensions])
    #ax2.axvline(means_dic[dimensions], label="mean at "+dimensions+" mm", color=color_dic[dimensions], ls=':')
    ax2.step(bins[:-1], counts, where="mid", label=dimensions, color=color_dic[dimensions])

E0 = 662
Compt_edge = E0*(2*(E0/511))/(1+2*(E0/511))
ax1.axvline(x=Compt_edge, ls=":")
ax1.text(s=r"$E_C$", x=Compt_edge+10, y=greatest_en)
ax1.set_yscale(value="log")
ax1.legend()

ax2.set_yscale(value="log")

preface = "_crystal-size_"

#save_file = re.sub("_t.*?\.", ".", f_templates[0])
save_file = re.sub("_nt_.*?\.", ".", f_templates[0])
save_file = re.sub("/data/", "/figures/", save_file)
#save_file = re.sub("-side_|-base_", "_", save_file)
save_file = re.sub("_4x4x22-|_3x3x20-|_10x10x20-", "_", save_file)
save_file = re.sub(".csv", preface+"energy_spectra.pdf", save_file)

print("saving to:", save_file)
fig1.savefig(save_file, bbox_inches="tight")

ax2.legend()
ax2.set_yscale(value="log")

save_file = re.sub(preface+"energy_spectra.pdf", preface+"photon_count.pdf", save_file)
#save_file = re.sub("\.pdf", "_photon_count-SiPM-placement.pdf", save_file)

print("saving to:", save_file)
fig2.savefig(save_file, bbox_inches="tight")