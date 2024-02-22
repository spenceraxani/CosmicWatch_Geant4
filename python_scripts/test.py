import re

f_template="../data/100000events_run0-side_nt_Event_t0.csv"

save_file = re.sub("_t.*?\.", ".", f_template)
save_file = re.sub("/data/", "/figures/", save_file)
save_file = re.sub("-side_|-base_", "_", save_file)
save_file = re.sub("\.csv", "_photon_count.pdf", save_file)

t_type = None
if "base" in f_template:
    t_type = "base"
elif "side" in f_template:
    t_type = "side"

print(t_type)

print(save_file)