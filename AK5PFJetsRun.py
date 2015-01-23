import FWCore.ParameterSet.Config as cms

process = cms.Process("OPENDATA")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'GR_R_42_V25::All'

readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles)
readFiles.extend( [
	'root://eospublic.cern.ch//eos/opendata/cms/Run2010B/Mu/AOD/Apr21ReReco-v1/0000/00459D48-EB70-E011-AF09-90E6BA19A252.root'
        	] );




from FWCore.MessageLogger.MessageLogger_cfi import *

process_load = process.load("PFCandidateFilter.PFCandidateFilter.AK5PFJetsFilter_cfi")

# Set the output file name and the invariant mass range (GeV)
process.AK5PFJetsFilter.csvFileName = cms.string("AK5PFJets.csv")

# Change this to set the maximum number of events to process
# -1 means all of them
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1))

process.Filter = cms.Path(process.AK5PFJetsFilter)
process.schedule = cms.Schedule(process.Filter)
