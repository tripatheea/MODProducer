from __future__ import division

from subprocess import call
from time import time
import os
import sys

registry_file = sys.argv[1]
output_file = sys.argv[2]

def inspect_registry(registry_file, output_file):
  
  files = set()
  with open(registry_file) as f:
    for line in f:
      files.add(line.split(" ")[3])

  with open(output_file, "w") as output_f:
    for f in files:
      output_f.write(f)

start = time()

inspect_registry(registry_file, output_file)

end = time()

print "Everything done in " + str(end - start) + " seconds!"


# /media/aashish/opendata/eos/opendata/cms/Run2010B/Jet/MOD/Apr21ReReco-v1/0000

