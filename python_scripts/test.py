import re

f_template="../data/100000events_run0_nt_Event_t0.csv"

save_file = re.sub("_t.*?\.", ".", f_template)
save_file = re.sub("/data/", "/figures/", save_file)
save_file = re.sub("\.csv", "_photon_count.pdf", save_file)

print(save_file)