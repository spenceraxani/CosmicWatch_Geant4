import re

'''pattern = re.compile(r'^\d.+\d$')

strings = ['1kbsfdkbsadfasb\n', 'dlfhsahf', '6saldflhfgls2\n', 'ksjfsajh3']

for s in strings:
	if re.match(pattern, s):
		print(s, 'match')'''

file_template = "../data/Co60-25cm-10min.txt"

isotopes = ['Co60', 'Cs137']

for isotope in isotopes:
	'''search = re.search("[/]+?\w+\-+", file_template)
	if search:
		print(search.group(0))
	else:
		print('no match')'''
	file_template = re.sub("[/]+\w+\-+", "/"+isotope+"-", file_template)
	print(file_template)