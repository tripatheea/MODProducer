import FWCore.ParameterSet.Config as cms

AK7PFJetsProducer = cms.EDProducer('AK7PFJetsProducer' ,
                            inputTag = cms.InputTag("ak7PFJets"),
                            outputFilename = cms.string("AK7PFJets.dat"),
                            )
