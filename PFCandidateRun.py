import FWCore.ParameterSet.Config as cms 
import FWCore.PythonUtilities.LumiList as LumiList 

from FWCore.MessageLogger.MessageLogger_cfi import *

process = cms.Process("OPENDATA")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'GR_R_42_V25::All'

readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles)

# Enumerate all ROOT file links into a list.
all_root_links = []
indices_file = 'CMS_Run2010B_MultiJet_AOD_Apr21ReReco-v1_0002_file_index.txt'
for line in open(indices_file, 'r'):
	all_root_links.append(line.strip)

readFiles.extend(all_root_links);

# Process only those luminosity sections in which runs are considered good and should be processed.
goodJSON = 'Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt' 
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange() 
process.source.lumisToProcess.extend(myLumis)

process_load = process.load("PFCandidateFilter.PFCandidateFilter.PFCandidateFilter_cfi")

# Set the output file name.
process.PFCandidateFilter.csvFileName = cms.string("PFCandidate.csv")

# Change this to set the maximum number of events to process
# -1 means all of them
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

process.Filter = cms.Path(process.PFCandidateFilter)
process.schedule = cms.Schedule(process.Filter)