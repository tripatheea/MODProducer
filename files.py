from __future__ import division

from subprocess import call
from time import time
import os
import sys

source_path = sys.argv[1]
destination_path = sys.argv[2]
path_to_copy_to = sys.argv[3]

def mark_as_completed(input_path, output_path, path_to_copy_to):
  
  source, destination = [], []

  source_files = [f for f in os.listdir(input_path) if f.endswith("root")]
  destination_files = [f for f in os.listdir(output_path) if f.endswith("mod") ]
  		
  to_move = []
  for source_f in source_files:
  	if source_f.split(".")[0] + ".mod" in destination_files:
  		to_move.append( source_f )

  for f in to_move:
    call(['mv', input_path + "/" + f, path_to_copy_to + "/" + f])


start = time()

mark_as_completed(source_path, destination_path, path_to_copy_to)

end = time()

print "Everything done in " + str(end - start) + " seconds!"


# /media/aashish/opendata/eos/opendata/cms/Run2010B/Jet/MOD/Apr21ReReco-v1/0000

