import matplotlib as mpl

fsize = 16
mpl.rcParams['legend.fontsize'] = fsize-2
mpl.rcParams["figure.figsize"] = (10,4)
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
import json
import re

from merge_threads import merge

def mean(bins, freq):
    dx = bins[1]-bins[0]

    events = sum(freq)

    f_b_sum = 0
    for i, f in enumerate(freq):
        f_b_sum += bins[i]*f
    
    v = f_b_sum/events
    #print(events, "in histogram, mean =", f_b_sum/events)

    return v

def err(row):
    return np.abs(row.Tot-row.Sum)/row.Tot

data_folder = "PScint/scint_size/"

with open("../data/"+data_folder+"config.json", "r") as infile:
    config = json.load(infile)

materials = config["materials"]
#print(materials)
size_dict = config["size_dict"]
#print(size_dict)

means_dic = {"4x4x22": 0, "3x3x20": 0, "10x10x20": 0}

#fig1, ax1 = plt.subplots()
num_plots = 2
fig1, ax1 = plt.subplots(nrows=num_plots, ncols=1, figsize=(10, 10))
plt.tight_layout(h_pad=3.0)
ax2 = ax1[1].twinx()
#fig1, ax1 = plt.subplots(nrows=len(config["size_dict"]["PScint"]), ncols=1, sharex='col', figsize=(10, 12), squeeze=True)
#fig2, ax2 = plt.subplots(nrows=len(config["size_dict"]["PScint"]), ncols=1, sharex='col', figsize=(10, 12), squeeze=True)
#fig3, ax3 = plt.subplots(nrows=len(config["size_dict"]["PScint"]), ncols=1, sharex='col', figsize=(10, 12), squeeze=True)

max_En = 0
for material in materials:

    for s in range(num_plots):
        ax1[s].grid(which='both', axis='both')
        ax1[s].grid(which='minor', axis='both', alpha=0.3)
        ax1[s].set_ylabel("Counts")

        #ax2[s].grid(which='both', axis='both')
        #ax2[s].grid(which='minor', axis='both', alpha=0.3)
        #ax2[s].set_ylabel("Counts")

        #ax3[s].grid(which='both', axis='both')
        #ax3[s].grid(which='minor', axis='both', alpha=0.3)
        #ax3[s].set_ylabel("Counts")

        #if s==0:
            #ax1[s].set_title(label="Energy deposition")
            #ax2[s].set_title("SiPM photon count")
            #ax3[s].set_title("Photon production")
        #if s==len(config["size_dict"]["PScint"])-1:
            #ax1[s].set_xlabel("Energy/event [keV]")
            #ax2[s].set_xlabel("No of photons/event")
            #ax3[s].set_xlabel("No of photons/event")

        for size in size_dict[material]:

            f_template = "../data/"+data_folder+material+"_"+size+config["dtype"]+"_run0_nt_Event_t0.csv"

            #merge data in input files
            data_frames = merge(config["runs"], config["threads"], config["columns"], f_template)
            Events = len(data_frames[0].index)

            if config["dtype"]=="gammas":

                #find events where photons were produced but not detected
                zero_production = len(data_frames[0].loc[data_frames[0]["NOfScintPhotons"] == 0].index)
                zero_detection = len(data_frames[0].loc[(data_frames[0]["NOfOptPhotons"] == 0) & (data_frames[0]["NOfScintPhotons"] > 0)]["NOfOptPhotons"].index)
                detection_events = data_frames[0].loc[(data_frames[0]["NOfOptPhotons"] > 0) & (data_frames[0]["NOfScintPhotons"] > 0)]
                detection_event_count = len(detection_events.index)
                #get detected/produced ratio
                percentages = detection_events["NOfOptPhotons"]/detection_events["NOfScintPhotons"]
                percentage_mean = percentages.mean()
                percentage_std = percentages.std()
                #mean_perecentage = percentages.sum()/detection_event_count
                print("zero production events:", zero_production)
                print("zero detection events:", zero_detection)
                print("normal events:", detection_event_count)
                print("mean percentage:", percentage_mean)
                print("std percentage:", percentage_std)

                #print(len(data_frames), "runs read")
                #print("Tot events/run =", Events)

                photoelectric_events = data_frames[0].loc[(data_frames[0]["Tot-OpAbs"] > 661)]
                photoelectric_events_ScintPhot_mean = photoelectric_events.loc[:,"NOfScintPhotons"].mean()
                photoelectric_events_ScintPhot_std = photoelectric_events.loc[:,"NOfScintPhotons"].std()
                photoelectric_events_SiPMPhot_mean = photoelectric_events.loc[:,"NOfOptPhotons"].mean()
                photoelectric_events_SiPMPhot_std = photoelectric_events.loc[:,"NOfOptPhotons"].std()
                print("photoelectric events:", len(photoelectric_events.index))
                print("photoelectric events production mean:", photoelectric_events_ScintPhot_mean)
                print("photoelectric events production std:", photoelectric_events_ScintPhot_std)
                print("photoelectric events SiPM mean:", photoelectric_events_SiPMPhot_mean)
                print("photoelectric events SiPM std:", photoelectric_events_SiPMPhot_std)

            #------------------Energy histogram------------------#
            #adding energy deposition per process to compare with total data from simulation
            data_frames[0] = data_frames[0].eval('Sum = Cerenkov+compt+CoupledTransportation+eBrem+eIon+msc+muIoni+phot')

            diff_list  = [(t-s)/t if t>0 else 0 for t, s in zip(data_frames[0]["Tot-OpAbs"], data_frames[0]["Sum"])]

            data_frames[0]["Diff"] = diff_list
            #data_frames[0]["Diff"] = data_frames[0].apply(err, axis=1) 
            #data_frames[0] = data_frames[0].eval('Diff = (Tot - Sum)/Tot') 

            #print(data_frames[0]["Tot-OpAbs"])

            counter = 0
            threshold = 0.001
            for e in range(Events):
                if np.abs(data_frames[0]["Diff"][e]) >= threshold:
                    counter += 1
                    #print(e, data_frames[0]["Tot-OpAbs"][e], data_frames[0]["Sum"][e], data_frames[0]["Diff"][e])

            weird_events = data_frames[0].index[data_frames[0]["Tot-OpAbs"] > 662].tolist()

            print(counter, "events have an error greater than", threshold)
            if len(weird_events):
                print(len(weird_events), "events above photopeak", weird_events[0])

            max_En = max(data_frames[0]["Tot-OpAbs"])
            min_En = min(data_frames[0]["Tot-OpAbs"])

            bin_size = (max_En-min_En)/100

            bins = np.arange(min_En, max_En+bin_size, bin_size)
            counts, _ = np.histogram(data_frames[0]["Tot-OpAbs"], bins=bins)

            if s==0:
                major_x = np.arange(1000, 7001, 500)
                minor_x = np.arange(1000, 7001, 100)

                ax1[s].set_xlim(left=1000, right=7000)
                ax1[s].set_xticks(major_x)
                ax1[s].set_xticks(minor_x, minor=True)

                ax1[s].set_title(label="Energy deposition")
                ax1[s].set_xlabel("Energy/event [keV]")
                ax1[s].step(bins[:-1]+bin_size/2, counts, where="mid", label=size+r" mm$^3$", color=config["color_dict"][size], ls=config["line_style"][material], alpha=config["alpha"][material], lw=2)

                ax1[s].legend(loc="upper right")

            #------------------counting SiPM photons------------------#
            SiPMphotons = data_frames[0]['NOfOptPhotons']
            max_SiPMphotons = max(SiPMphotons)
            min_SiPMphotons = min(SiPMphotons)

            bin_size = (max_SiPMphotons-min_SiPMphotons)/100
            #bin_size = 5

            bins = np.arange(min_SiPMphotons, max_SiPMphotons+(bin_size/2.), bin_size)
            counts, _ = np.histogram(SiPMphotons, bins=bins)

            means_dic[size] = mean(bins, counts)

            if s==1:
                #major_x = np.arange(1000, 7001, 500)
                #minor_x = np.arange(1000, 7001, 100)

                #ax1[s].set_xlim(left=1000, right=7000)
                #ax1[s].set_xticks(major_x)
                #ax1[s].set_xticks(minor_x, minor=True)

                #ax1[s].set_title("SiPM photon count")
                #ax1[s].set_xlabel("No of photons/event")
                ax2.step(bins[:-1]+bin_size/2, counts, where="mid", label=size+r" mm$^3$", color=config["color_dict"][size], ls="--", alpha=config["alpha"][material], lw=2)
                ax2.set_ylim(top=70)
            #ax2[s].step(bins[:-1]+bin_size/2, counts, where="mid", label=material, color=config["color_dict"][size], ls=config["line_style"][material], alpha=config["alpha"][material], lw=3)

            #------------------counting Scint photons------------------#

            ScintPhotons = data_frames[0]['NOfScintPhotons']
            max_ScintPhotons = max(ScintPhotons)
            min_ScintPhotons = min(ScintPhotons)

            bin_size = (max_ScintPhotons-min_ScintPhotons)/100
            #bin_size = 5

            bins = np.arange(min_ScintPhotons, max_ScintPhotons+(bin_size/2.), bin_size)
            counts, _ = np.histogram(ScintPhotons, bins=bins)

            means_dic[size] = mean(bins, counts)

            if s==1:
                #major_x = np.arange(10000, 70001, 5000)
                #minor_x = np.arange(10000, 70001, 1000)

                #ax1[s].set_xlim(left=10000, right=70000)
                #ax1[s].set_xticks(major_x)
                #ax1[s].set_xticks(minor_x, minor=True)

                ax1[s].set_title("Photon detection and production")
                ax1[s].set_xlabel("No of photons/event")
                ax1[s].step(bins[:-1]+bin_size/2, counts, where="mid", label=size+r" mm$^3$", color=config["color_dict"][size], ls="-", alpha=config["alpha"][material], lw=2)
                ax1[s].set_ylim(top=70)

                ax1[s].set_xscale(value="log")
                ax1[s].legend(loc="upper right", title="Photons produced")
            #ax3[s].step(bins[:-1]+bin_size/2, counts, where="mid", label=material, color=config["color_dict"][size], ls=config["line_style"][material], alpha=config["alpha"][material], lw=3)

            f_template = "../data/"+data_folder+size+"_run0_nt_Event_t0.csv"

            if config["dtype"] == "gammas":
                print("--------------------------------------------------------")
                E0 = 662
                Compt_edge = E0*(2*(E0/511))/(1+2*(E0/511))
                ax1[s].axvline(x=Compt_edge, label=r"$E_C$", ls="--", color=config["color_dict"][size], lw=1)
                #ax1[s].axvline(x=E0, ls="--", color=config["color_dict"][size], lw=1)
                #ax1.text(s=r"$E_C$", x=Compt_edge+10, y=max_En)

                ax1[s].set_yscale(value="log")
                #ax2[s].set_yscale(value="log")
                #ax3[s].set_yscale(value="log")

            #ax1[s].legend(loc="upper right")

            #preface = material+config["dtype"]+"_"

            save_file = re.sub("_nt_.*?\.", ".", f_template)
            save_file = re.sub("/data/", "/figures/", save_file)
            save_file = re.sub(".csv", "energy_spectra.pdf", save_file)

            major_x = np.arange(0, 22501, 2500)
            minor_x = np.arange(0, 22501, 500)

            #ax2[s].set_xlim(left=-500, right=23000)
            #ax2[s].set_xticks(major_x)
            #ax2[s].set_xticks(minor_x, minor=True)

            #ax2[s].legend(loc="upper right", title=size+r" mm$^3$")

            #major_x = np.arange(0, 22501, 2500)
            #minor_x = np.arange(0, 22501, 500)

            #ax3[s].set_xlim(left=-500, right=23000)
            #ax3[s].set_xticks(major_x)
            #ax3[s].set_xticks(minor_x, minor=True)

            #ax3[s].legend(loc="upper right", title=size+r" mm$^3$")

    ax2.legend(loc="upper left", title="SiPM counts")

#ax1[1].text(s=r"$E_C$", x=Compt_edge+10, y=max_En)

f_template = "../data/"+data_folder+"run0_nt_Event_t0.csv"

save_file = re.sub("_nt_.*?\.", ".", f_template)
save_file = re.sub("/data/", "/figures/", save_file)
#save_file = re.sub(".csv", "energy_spectra.pdf", save_file)
save_file = re.sub(".csv", "PScint.pdf", save_file)

print("saving to:", save_file)
fig1.savefig(save_file, bbox_inches="tight")

save_file = re.sub("energy_spectra", "SiPMphotons", save_file)
#save_file = re.sub("\.pdf", "_photon_count-SiPM-placement.pdf", save_file)

print("saving to:", save_file)
#fig2.savefig(save_file, bbox_inches="tight")

save_file = re.sub("SiPMphotons", "ScintPhotons", save_file)
#save_file = re.sub("\.pdf", "_photon_count-SiPM-placement.pdf", save_file)

print("saving to:", save_file)
#fig3.savefig(save_file, bbox_inches="tight")