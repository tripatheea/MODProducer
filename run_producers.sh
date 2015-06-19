#!/bin/bash

# Clean up everything and recompile.
scram b clean
scram b

echo "Clean up and compilation complete."
echo "Now running all producers."

#Run all the producers.
echo "Running PFCandidate Producers now."

cmsRun PFCandidateRun.py file:///media/sf_virtual_machine/CMS/AOD/6ADEA1BB-ED70-E011-A888-0025901D4C46.root
cmsRun PFCandidateRun.py file:///media/sf_virtual_machine/CMS/AOD/6CD3F555-EF70-E011-B0C0-003048D439A8.root

#file:///media/sf_virtual_machine/CMS/AOD/6E9DAA2C-7471-E011-8137-003048F0E426.root

echo "All producers are complete now."