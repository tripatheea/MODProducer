#!/bin/bash

# Clean up everything and recompile.
scram b clean
scram b

echo "Clean up and compilation complete."
echo "Now running all producers."

#Run all the producers.
echo "Running PFCandidate Producers now."
cmsRun PFCandidateRun.py

echo "Moving to MinBias Producers now."
cmsRun minBiasRun.py

echo "All producers are complete now."