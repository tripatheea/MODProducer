import subprocess
import os
from time import time
import sys

path = sys.argv[1]
registry_file_path = sys.argv[2]
log_file_path = str(registry_file_path) + "_log.log"


def get_files_already_in_registry(registry_file):

	f = open(registry_file, "wb")
	f.close()

	files = set()
	with open(registry_file) as f:
		for line in f:
			files.add( line.split(" ")[3].split("\n")[0] )

	return list(files)

def create_registry(path, log_file_path):
	
	files_already_processed = get_files_already_in_registry(registry_file_path)
	files_already_processed = []
	
	files_to_process = []
	for f1 in os.listdir(path):
		if (f1.endswith("root")) and f1 not in files_already_processed:
			root_file = "file://" + path + f1
			files_to_process.append(root_file)
	
	for root_file in sorted(files_to_process):			
		stdoutdata, stderrdata = subprocess.Popen(["cmsRun", "filenameRun.py", root_file, registry_file_path]).communicate()
		
		if str(stderrdata) != "None":
			log_file = open(log_file_path, 'a')
			log_file.write(str(stderrdata) + "\n") 
			log_file.close()
start = time()

create_registry(path, log_file_path)

end = time()

print "Everything done in " + str(end - start) + " seconds!"