import pandas as pd
import numpy as np

df = pd.read_csv("RefractiveIndexNaI.csv", header=0)

#E=hc/lambda
#hc=1.23984197x10^-6 eVm
df["E"] = [round(1.23984197/float(l),5) for l in df["w.um"]]

df.to_csv("RefractiveIndexNaI.csv", index=False)