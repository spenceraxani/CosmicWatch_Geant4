import numpy as np
import matplotlib.pyplot as plt

#read data
runs = int(input("enter number of runs: "))
threads = int(input("enter number of threads: "))
columns = int(input("enter number of columns: "))

#merge data in imput files
f_template = "../data/100000events_run0_nt_Event_t0.csv"

from merge_threads import merge
data_frames = merge(runs, threads, columns, f_template)

Events = len(data_frames[0].index)

print("Tot events =", Events)

data_frames[0] = data_frames[0].eval('Sum = Cerenkov+compt+CoupledTransportation+eBrem+eIon+msc+phot') 

def err(row):
    return np.abs(row.Tot-row.Sum)/row.Tot

diff_list  = [(t-s)/t if t>0 else 0 for t, s in zip(data_frames[0]["Tot"], data_frames[0]["Sum"])]

data_frames[0]["Diff"] = diff_list
#data_frames[0]["Diff"] = data_frames[0].apply(err, axis=1) 
#data_frames[0] = data_frames[0].eval('Diff = (Tot - Sum)/Tot') 

for e in range(Events):
    if np.abs(data_frames[0]["Diff"][e]) >= 0.01:
        print(e, data_frames[0]["Tot"][e], data_frames[0]["Sum"][e], data_frames[0]["Diff"][e])

weird_events = data_frames[0].index[data_frames[0]['Tot'] >= 662].tolist()

tot_energy = [t for t in data_frames[0]['Tot'] if t>0]
print(max(tot_energy))

bins = np.arange(0, 671, 10)
counts, _ = np.histogram(tot_energy, bins=bins)

plt.step(bins[:-1], counts, where="mid")
plt.axvline(x=477, label="borde Compton", color="orange")
plt.savefig("../figures/100000events_run0_nt_Event_t0.pdf", bbox_inches="tight")