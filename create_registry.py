from subprocess import call
import os
from time import time
import sys
from collections import defaultdict

path = sys.argv[1]
registry_file_path = sys.argv[2]

def create_registry(path):
	files_to_process = []
	for f1 in os.listdir(path):
		if (f1.endswith("root")):
			root_file = "file://" + path + f1
			files_to_process.append(root_file)
	
	for root_file in sorted(files_to_process):			
		call(["cmsRun", "filenameRun.py", root_file, registry_file_path])


start = time()

create_registry(path)

end = time()

print "Everything done in " + str(end - start) + " seconds!"