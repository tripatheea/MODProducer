import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils
import sys


data_file_link = sys.argv[2]
local_or_remote = sys.argv[3]

if local_or_remote == "remote":
	file_directory = data_file_link[75:79]
	file_name = data_file_link[80:len(data_file_link)]
else:
	file_directory = data_file_link[41:45]
	file_name = data_file_link[46:len(data_file_link)]

process = cms.Process("filenameMapProducer")

process.load("FWCore.MessageLogger.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.source = cms.Source ("PoolSource", fileNames=cms.untracked.vstring( data_file_link ) )
process.filenameMapProducer = cms.EDProducer("filenameMapProducer", 
								filename = cms.string(file_name), 
								file_dir = cms.string(file_directory), 
								outputFile = cms.string("map.mod") 
						)


process.MessageLogger = cms.Service("MessageLogger",
        				default   =  cms.untracked.PSet(
                                                     timespan = cms.untracked.int32(60)
       )                                                  
)       
						
process.producer = cms.Path(process.filenameMapProducer)
process.schedule = cms.Schedule( process.producer )
