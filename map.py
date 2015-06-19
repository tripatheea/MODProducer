from subprocess import call
import os
from time import time

target = open("map.mod", "w")

def create_maps(local_or_remote):
	path = "file_paths/Jet/" + local_or_remote + "/"
	
	for file in os.listdir(path):
		if file.endswith("txt"):
			filename = path + file
			f = open(filename, 'r')
	
			links = []
			links.extend(f.read().split("\n"))
			
			for root_link in links:
				call(["cmsRun", "filenameRun.py", root_link, local_or_remote])


start = time()
create_maps("local")
end = time()

print "Everything done in " + str(end - start) + " seconds!"