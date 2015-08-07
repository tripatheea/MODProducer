import sys
import os
from subprocess import call

import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import FWCore.PythonUtilities.LumiList as LumiList

from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from RecoJets.JetProducers.ak5PFJets_cfi import ak5PFJets
from RecoJets.JetProducers.kt4PFJets_cfi import kt4PFJets

input_dir = sys.argv[2]
map_file_path = sys.argv[3]

dir_to_create = input_dir.replace("AOD", "MOD")
if not os.path.exists(dir_to_create):
    os.makedirs(dir_to_create)

files_to_process = []

if os.path.isdir(input_dir):
	for file in os.listdir(input_dir):
		if file.endswith("root"):
			output_file = input_dir.replace("AOD", "MOD") + "/" + file + ".mod"
			if not os.path.exists(output_file):
				files_to_process.append("file://" + input_dir + "/" + file)
			
			is_input_directory = True
			
			
else:
	files_to_process.append("file://" + input_dir)
	segments = files_to_process[0].split("/")
	input_file = segments[len(segments) - 1:len(segments)][0]
	is_input_directory = False
	
# This sorting is crucial.
files_to_process = sorted(files_to_process)


if is_input_directory:
	segments = files_to_process[0].split("/")
	input_file = segments[len(segments) - 1:len(segments)][0]



readFiles = cms.untracked.vstring()
readFiles.extend(files_to_process)


process = cms.Process("MITCMSOpenData")

#process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'GR_R_42_V25::All'

process.source = cms.Source("PoolSource", fileNames=readFiles)

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

goodJSON = "file_paths/Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt"
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',')
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange()
process.source.lumisToProcess.extend(myLumis)


process.ak5PFJets = ak5PFJets.clone(doAreaFastjet = cms.bool(True))
		    	
process.kt6PFJetsForIsolation = kt4PFJets.clone(rParam = 0.6, doRhoFastjet = True)

process.PFCandidateProducer = cms.EDProducer("PFCandidateProducer",
					rho = cms.InputTag("kt6PFJets","rho"),
					PFCandidateInputTag = cms.InputTag("particleFlow"),
					AK5PFInputTag = cms.InputTag("ak5PFJets"),
					mapFilename = cms.string(map_file_path),
					inputFile = cms.string(input_file),
					primaryVertices = cms.InputTag("offlinePrimaryVertices"),
					dataVersion = cms.string("1")
				)
				
process.producer = cms.Path( process.ak5PFJets * process.kt6PFJetsForIsolation * process.PFCandidateProducer)
process.schedule = cms.Schedule( process.producer )
