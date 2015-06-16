from subprocess import call
import os


def download_root_files():
	path = "../../file_paths/Jet/remote/"
	
	for file in os.listdir(path):
		filename = path + file
		print filename
		f = open(filename, 'r')
		links = []
		links.extend(f.read().split("\n"))
		
		f2 = open(file,'w')
		for root_link in links:
			http_link = root_link.replace("root://eospublic.cern.ch//", "http://opendata.cern.ch/")
			root_file_name = http_link[78:len(http_link)]
			root_file_name = "file://data/ROOT/" + root_file_name
			print root_file_name
			f2.write(root_file_name + "\n")
		
		f2.close()
			

		
download_root_files()