import FWCore.ParameterSet.Config as cms

minBiasProducer = cms.EDProducer('minBiasProducer' ,
                            inputTag = cms.InputTag("particleFlow"),
                            outputFilename = cms.string("CMS_MinBiasSample.dat"),
                            )
