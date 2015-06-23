import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import sys
import os
from subprocess import call

input_dir = sys.argv[2]
output_base_path = sys.argv[3]
map_file_path = sys.argv[4]

files_to_process = []
for file in os.listdir(input_dir):
	if file.endswith("root"):
		files_to_process.append("file://" + input_dir + "/" + file)
		
		# Delete any output MOD files that might already be there.
		output_file = input_dir.replace("AOD", "MOD") + file + ".mod"
		if os.path.exists(output_file):
			call(["rm", output_file])

# This sorting is crucial.
files_to_process = sorted(files_to_process)

readFiles = cms.untracked.vstring()
readFiles.extend(files_to_process)

process = cms.Process("MITCMSOpenData")
process.source = cms.Source ("PoolSource", fileNames=readFiles)
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.PFCandidateProducer = cms.EDProducer("PFCandidateProducer",
					rho = cms.InputTag("kt6PFJets","rho"),
					PFCandidateInputTag = cms.InputTag("particleFlow"),
					AK5PFInputTag = cms.InputTag("ak5PFJets"),
					outputBasePath = cms.string(output_base_path),
					mapFilename = cms.string(map_file_path)
				)
				
process.producer = cms.Path(process.PFCandidateProducer)
process.schedule = cms.Schedule( process.producer )
