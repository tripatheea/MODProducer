import FWCore.ParameterSet.Config as cms 
import FWCore.PythonUtilities.LumiList as LumiList 
import FWCore.Utilities.FileUtils as FileUtils

from FWCore.MessageLogger.MessageLogger_cfi import *


from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *
from JetMETCorrections.Configuration.DefaultJEC_cff import *


process = cms.Process("OPENDATA")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.load("JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff")
process.load("JetMETCorrections.Configuration.JetCorrectionServices_cff")

process.GlobalTag.globaltag = 'GR_R_42_V25::All'

# Need to split into multiple separate fileGR_R_42_V25::Alls a limit of 255 values for a set of parameters and thereby for the indices files too.
indices_file = 'file_paths/Jet/CMS_Run2010B_Jet_AOD_Apr21ReReco-v1_000'	# Should append "_" + index#umber + ".txt" to this.

mylist = FileUtils.loadListFromFile (indices_file + '0_file_index.txt')
#mylist.extend(FileUtils.loadListFromFile(indices_file + '1_file_index.txt') )
#mylist.extend(FileUtils.loadListFromFile(indices_file + '2_file_index.txt') )
#mylist.extend(FileUtils.loadListFromFile(indices_file + '3_file_index.txt') )


readFiles = cms.untracked.vstring( *mylist)

process.source = cms.Source ("PoolSource", fileNames=readFiles)

# Process only those luminosity sections in which runs are considered good and should be processed.

#goodJSON = 'file_paths/Cert_136033-149442_7TeV_Apr21ReReco_Collisions10_JSON_v2.txt' 
#myLumis = LumiList.LumiList(filename = goodJSON).getCMSSWString().split(',') 
#process.source.lumisToProcess = cms.untracked.VLuminosityBlockRange() 
#process.source.lumisToProcess.extend(myLumis)

process_load = process.load("CMSOpenData.MODProducer.pfcandidateproducer_cfi")


# Change this to set the maximum number of events to process
# -1 means all of them
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

#process.Producer = cms.Path(process.PFCandidateProducer)
#process.schedule = cms.Schedule(process.Producer)

process.ana      = cms.Sequence(process.correctedAK5)
process.p        = cms.Path(process.ana)
#process.outpath  = cms.EndPath(process.output)

