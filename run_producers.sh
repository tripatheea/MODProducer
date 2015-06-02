#!/bin/bash

# Initialize all CMS runtime variables.
cmsenv

# Clean up everything and recompile.
scram b clean
scram b

echo "Clean up and compilation complete."
echo "Now running all producers."

#Run all the producers.
cmsRun PFCandidateRun.py
cmsRun minBiasRun.py
cmsRun AK5PFJetsRun.py
cmsRun AK7PFJetsRun.py
