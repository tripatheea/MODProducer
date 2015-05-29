import FWCore.ParameterSet.Config as cms 
import FWCore.PythonUtilities.LumiList as LumiList 
import FWCore.Utilities.FileUtils as FileUtils

from FWCore.MessageLogger.MessageLogger_cfi import *

process = cms.Process("OPENDATA")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'GR_R_42_V25::All'

# Need to split into multiple separate files because there's a limit of 255 values for a set of parameters and thereby for the indices files too.
indices_file = 'file_paths/Jet/CMS_Run2010B_Jet_AOD_Apr21ReReco-v1_000'	# Should append "_" + indexNumber + ".txt" to this.

mylist = FileUtils.loadListFromFile (indices_file + '0_file_index.txt')
mylist.extend(FileUtils.loadListFromFile(indices_file + '1_file_index.txt') )
mylist.extend(FileUtils.loadListFromFile(indices_file + '2_file_index.txt') )
mylist.extend(FileUtils.loadListFromFile(indices_file + '3_file_index.txt') )


readFiles = cms.untracked.vstring( *mylist)

process.source = cms.Source ("PoolSource", fileNames=readFiles)

# Process only those luminosity sections in which runs are considered good and should be processed.
goodJSON = 'file_paths/Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt' 
myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 
process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange() 
process.source.lumisToProcess.extend(myLumis)

process_load = process.load("CMSOpenDataProducer.CMSOpenDataProducer.ak8pfjetsproducer_cfi")


# Change this to set the maximum number of events to process
# -1 means all of them
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.Producer = cms.Path(process.AK8PFJetsProducer)
process.schedule = cms.Schedule(process.Producer)
