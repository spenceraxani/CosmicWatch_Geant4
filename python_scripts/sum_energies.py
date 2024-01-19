import numpy as np

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

data_frames[0] = data_frames[0].eval('Sum = Cerenkov + CoupledTransportation + eIon + msc + phot + compt') 

data_frames[0] = data_frames[0].eval('Diff = Tot - Sum') 

for e in range(Events):
    if data_frames[0]["Diff"][e] != 0:
        print(e, data_frames[0]["Diff"][e])