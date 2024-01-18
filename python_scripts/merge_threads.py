from pathlib import Path

import pandas as pd
import re

def merge(runs=1, threads=1, columns=1, f_template="../data0/output0_nt_Hits_t0.csv"):

	data_frames = [None]*runs

	for run in range(runs):

	    #list all data-file names
	    fnames = []

	    f_template = re.sub("\d+/", str(run)+"/", f_template)
	    for thread in range(threads):
	    	f_template = re.sub("\d+[.]", str(thread)+".", f_template)

	    	#check if file exists
	    	path = Path(f_template)
	    	if path.is_file():
	    		print("reading: "+f_template)
	    		fnames.append(f_template)
	    	else:
	    		print("No such file or directory: "+f_template)

	    if len(fnames) == 0:
	    	print("run "+str(run)+" not executed, skipping")
	    	continue

	    #reading column names
	    headers = [None]*columns
	    with open(fnames[0]) as file:
	        #read rows of metadata
	        meta = [next(file) for line in range(4+columns)]
	        headers = [info.strip().split()[2] for info in meta[4:]]

	    #read all csv files per thread
	    dfs = [pd.read_csv(f, names=headers, comment="#") for f in fnames]

	    #convine data frames
	    df = pd.concat(dfs, ignore_index=True)

	    data_frames[run] = df

	return data_frames