import os
from subprocess import call


def download_root_files():
	path = "../../file_paths/Jet/"
	
	links = []
	for file in os.listdir(path):
		filename = path + file
		f = open(filename, 'r')
		links.extend(f.read().split("\n"))
		
	i = 1
	for root_link in links:
		print "Downloading file #" + str(i)
		print
		http_link = root_link.replace("root://eospublic.cern.ch//", "http://opendata.cern.ch/")
		root_file_name = http_link[78:len(http_link)]
		call(["wget", http_link, "-N"])
		i += 1
		print "\n"*5
		
		
download_root_files()