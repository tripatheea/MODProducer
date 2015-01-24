import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.Config as cms 
import FWCore.PythonUtilities.LumiList as LumiList 

process = cms.Process("OPENDATA")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'GR_R_42_V25::All'

readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles)
readFiles.extend( [
       'root://006C7249-7F71-E011-8764-0018F3D09614.root'
		] );

goodJSON = 'Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt' 
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange() 
process.source.lumisToProcess.extend(myLumis)


from FWCore.MessageLogger.MessageLogger_cfi import *

process_load = process.load("PFCandidateFilter.PFCandidateFilter.PFCandidateFilter_cfi")

# Set the output file name and the invariant mass range (GeV)
process.PFCandidateFilter.csvFileName = cms.string("PFCandidate.csv")

# Change this to set the maximum number of events to process
# -1 means all of them
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

process.Filter = cms.Path(process.PFCandidateFilter)
process.schedule = cms.Schedule(process.Filter)