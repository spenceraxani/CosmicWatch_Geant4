import re

lines = ["XStop:-0.136364:", "XStart:-1.721e-09:\n"]

XStart = None


for line in lines:
	m = re.search('(?<=XStart:)(.*?)(?=:)', line)
	if m:
		XStart = float(m.group(0))
	print(m)
	print(XStart)