import os
from subprocess import call

downloaded_files = []

for f in os.listdir("./"):
	if (f.endswith("root")):
		downloaded_files.append(f)


def download_root_files():
	path = "../../file_paths/Jet/remote/"
	
	links = []
	for file in os.listdir(path):
		filename = path + file
		f = open(filename, 'r')
		file_to_download = f.read().split("\n")
		links.extend(file_to_download)
		
	i = 1
	for root_link in links:
		print "Downloading file #" + str(i)
		print
		http_link = root_link.replace("root://eospublic.cern.ch//", "http://opendata.cern.ch/")
		root_file_name = http_link[78:len(http_link)]
		if (root_file_name not in downloaded_files):
			call(["wget", http_link, "-N"])
		i += 1
		print "\n"*5
		
		
download_root_files()
