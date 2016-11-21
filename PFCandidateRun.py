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
output_dir = sys.argv[3]
map_file_path = sys.argv[4]
completed_log_filename = sys.argv[5]

dir_to_create = output_dir
if not os.path.exists(dir_to_create):
    os.makedirs(dir_to_create)

files_to_process = []

if os.path.isdir(input_dir):
	for file in os.listdir(input_dir):
		if file.endswith("root"):
			output_file = output_dir + "/" + file[:-5] + ".mod"
			
			'''
			if not os.path.exists(output_file):
				files_to_process.append("file://" + input_dir + "/" + file)
			else:
				#print "{} is already in the directory so skipping.".format(output_file)
				pass
			'''
			files_to_process.append("file://" + input_dir + "/" + file)
			is_input_directory = True
			
			
else:
	files_to_process.append("file://" + input_dir)

files_to_process.sort()

print "Total number of files processing = ", len(files_to_process)

readFiles = cms.untracked.vstring()
readFiles.extend(files_to_process)

process = cms.Process("MITCMSOpenData")

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_42_V25::All'


#bad_0000_files = ["root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/92EF2643-BB71-E011-B4D5-003048F02D36.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/349B7B9B-857A-E011-B621-0025901D4D76.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/506E8E63-9271-E011-B029-003048C692DA.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/8077989C-3971-E011-B7D8-003048D4399C.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/9037682E-7671-E011-9B9B-00266CF2718C.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/AE475FCF-3771-E011-924C-003048C64787.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/CCBD1AA5-EF70-E011-9D39-003048D437D2.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/E6F8C1BD-EE70-E011-B344-003048D439AA.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/E295FFD7-B171-E011-AA8F-00266CF327C0.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/0ECDEC5F-717A-E011-8E02-00237DDCBEA4.root"]
#bad_0005_files = ["root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/3C713477-5D71-E011-AE43-002481E0D50C.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/3E18FA90-6671-E011-9011-0030487D5EBD.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/08D6087D-8971-E011-B88F-0025901D492E.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/30F8B2AC-ED70-E011-B522-003048D4DFB8.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/44A6CBC3-AF71-E011-9D46-003048F0E1AE.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/78A1A25B-8F71-E011-855B-003048C692AC.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/A6E74D8E-ED70-E011-955F-003048F02CB4.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/E0E59AB4-A071-E011-A0C8-00266CF32EB8.root",  "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/FEE517A5-ED70-E011-BE21-0025901D4ADE.root"]
#bad_0005_missing_files = ["root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/44A6CBC3-AF71-E011-9D46-003048F0E1AE.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/30F8B2AC-ED70-E011-B522-003048D4DFB8.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/08D6087D-8971-E011-B88F-0025901D492E.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/E0E59AB4-A071-E011-A0C8-00266CF32EB8.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/FEE517A5-ED70-E011-BE21-0025901D4ADE.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/78A1A25B-8F71-E011-855B-003048C692AC.root", "root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0005/A6E74D8E-ED70-E011-955F-003048F02CB4.root"]
#missing_0000_files = [ "D4F0E262-9271-E011-BF37-0025901D4D64.root", "E41464FB-2671-E011-B4B2-003048F0E824.root", "DCA83164-837A-E011-9736-0025901D4138.root", "E015C7F5-EF70-E011-BD8B-003048C69040.root", "EE96816B-7171-E011-A7B9-003048D438FE.root", "FCE10B42-3A71-E011-A007-00266CF33054.root", "FC78CB27-FE70-E011-986C-003048D4797E.root", "2E90EE96-F570-E011-8254-003048C6928C.root", "F0DADA21-FE70-E011-BF21-003048D3CB3C.root", "EA50153B-2671-E011-B9DF-003048D4399E.root", "DCF2C83C-0071-E011-8103-002481E0D5E2.root", "0ED4517A-7C7A-E011-934A-003048C69032.root", "EAE4F1D8-EF70-E011-ACD7-003048D4399E.root", "FA348FE6-FD70-E011-BE62-003048D3C886.root", "2E89F45E-B071-E011-9038-003048C693E2.root" ]


#missing_0000_files = [ "506E8E63-9271-E011-B029-003048C692DA.root",  "92EF2643-BB71-E011-B4D5-003048F02D36.root",  "AE475FCF-3771-E011-924C-003048C64787.root",  "2CA2CA37-6871-E011-822B-003048C6928C.root",  "9037682E-7671-E011-9B9B-00266CF2718C.root", "8AE20EF4-BE71-E011-A97D-00266CF2AACC.root"  ]

#missing_0005_files = ["3AB36CE1-B171-E011-9090-0025901D4D20.root"]

#missing = ["root://eospublic.cern.ch/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/" + f for f in missing_0000_files]
#missing = ["file:///media/sf_37DCD64877C7286B/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/" + f for f in missing_0000_files]

#missing_0001_files = ["EC8896A7-A871-E011-B238-003048F0E3AE.root", "EAF18B48-4C71-E011-8E29-003048C68F6A.root"]


#missing = missing_0001_files

#readFiles = cms.untracked.vstring()
#readFiles.extend(missing)

process.source = cms.Source("PoolSource", fileNames=readFiles)

#print missing



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
					outputDir = cms.string(output_dir), 
					primaryVertices = cms.InputTag("offlinePrimaryVertices"),
					dataVersion = cms.string("5"),
					completedLogFilename = cms.string(completed_log_filename)
				)
				
process.producer = cms.Path( process.ak5PFJets * process.kt6PFJetsForIsolation * process.PFCandidateProducer)
process.schedule = cms.Schedule( process.producer )
