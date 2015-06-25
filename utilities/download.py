import os
from subprocess import call
import sys

list_of_downloads_file = sys.argv[1]
destination_dir = sys.argv[2]

def download_root_files(list_of_downloads_file, destination_dir):
	f = open(list_of_downloads_file, 'r')
	files_to_download = f.read().split("\n")
		
	i = 1
	for root_link in files_to_download:
		print "Downloading file #" + str(i)
		http_link = root_link.replace("root://eospublic.cern.ch//", "http://opendata.cern.ch/")
		root_file_name = http_link[78:len(http_link)]
		destination_dir_path = destination_dir + root_link[26:79]
		
		call(["wget", "--continue", http_link, "-N", "-P", destination_dir_path])
		
		i += 1
		print "\n"*5
		
		
		
download_root_files(list_of_downloads_file, destination_dir)
