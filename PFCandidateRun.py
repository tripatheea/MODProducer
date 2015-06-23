import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList
import sys
import os
from subprocess import call

input_dir = sys.argv[2]
output_base_path = sys.argv[3]
map_file_path = sys.argv[4]
process_from_the_beginning = (sys.argv[5] == 1)



files_to_process = []

if os.path.isdir(input_dir):
	for file in os.listdir(input_dir):
		if file.endswith("root"):
			files_to_process.append("file://" + input_dir + "/" + file)
			
			is_input_directory = True
			
			# Delete any output MOD files that might already be there.
			'''
			output_file = input_dir.replace("AOD", "MOD") + file + ".mod"
			if os.path.exists(output_file):
				call(["rm", output_file])
			'''
else:
	files_to_process.append("file://" + input_dir)
	segments = files_to_process[0].split("/")
	input_file = segments[len(segments) - 1:len(segments)][0]
	
# This sorting is crucial.
files_to_process = sorted(files_to_process)

readFiles = cms.untracked.vstring()
readFiles.extend(files_to_process)

process = cms.Process("MITCMSOpenData")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'GR_R_42_V25::All'

process.source = cms.Source ("PoolSource", fileNames=readFiles)

goodJSON = "file_paths/Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt"
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.PFCandidateProducer = cms.EDProducer("PFCandidateProducer",
					rho = cms.InputTag("kt6PFJets","rho"),
					PFCandidateInputTag = cms.InputTag("particleFlow"),
					AK5PFInputTag = cms.InputTag("ak5PFJets"),
					outputBasePath = cms.string(output_base_path),
					mapFilename = cms.string(map_file_path),
					processFromTheBeginning = cms.bool(process_from_the_beginning),
					inputFile = cms.string(input_file)
				)
				
process.producer = cms.Path(process.PFCandidateProducer)
process.schedule = cms.Schedule( process.producer )
