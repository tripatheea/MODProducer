from subprocess import call
import os
from time import time
import sys
from collections import defaultdict

path = sys.argv[1]
registry_file_path = sys.argv[2]

def create_maps(path):
	files_to_process = []
	for f1 in os.listdir(path):
		deeper_path = path + f1
		for f2 in os.listdir(deeper_path):	
			if (f2.endswith("root")):
				root_file = "file://" + deeper_path + "/" + f2
				files_to_process.append(root_file)
	
	for root_file in sorted(files_to_process):			
		call(["cmsRun", "filenameRun.py", root_file, registry_file_path])


def create_numbers_registry(registry_file_path):
	lines = [line.rstrip('\n') for line in open(registry_file_path)]
	
	events_in_root_files = defaultdict(int)
	last_root_filename = lines[0].split(" ")[1]
	for line in lines:
		root_filename = line.split(" ")[1]
		if root_filename != last_root_filename:
			last_root_filename = root_filename
	
		events_in_root_files[last_root_filename] += 1
			
	
	registry_path_list = registry_file_path.split("/")
	numbers_path = ""
	for i in range(0, len(registry_path_list) - 1):
		segment = registry_path_list[i]
		if segment == "":
			segment = "/"
		numbers_path += segment + "/"
		
	numbers_path += registry_path_list[len(registry_path_list) - 1].split(".")[0] + "_numbers.txt"
	
	f = open(numbers_path,'w')
	for root_filename in events_in_root_files:
		f.write(str(events_in_root_files[root_filename]) + "\n")
	f.close() 


start = time()

create_maps(path)

# Next, take the registry you just created and create another file with info on how many events per file there are going to be.
create_numbers_registry(registry_file_path)

end = time()

print "Everything done in " + str(end - start) + " seconds!"