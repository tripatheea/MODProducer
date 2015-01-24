import os

indices_file = 'CMS_Run2010B_MultiJet_AOD_Apr21ReReco-v1_0002_file_index.txt'


a = []
for line in open(indices_file, 'r'):
	a.append(line.strip())

print a