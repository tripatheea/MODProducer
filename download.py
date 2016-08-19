import os
from subprocess import call
import sys

'''
This script downloads ROOT files from the CMS server. This script takes two arguments:

1) a path to a file which contains a list of links to files to download (one link per line)
2) a destination path to write the files to. For a sample file that contains links to file, see file_paths/Jet/small_list.txt.

The download script will skip any ROOT file that you have already downloaded and will resume any broken downloads. So you don't have to download all the files at once as long as you are downloading all of them to the same directory.

Run this script as follows:

python ./utilities/download.py ./file_paths/Jet/small_list.txt ~/MITOpenDataProject/

'''


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
