import FWCore.ParameterSet.Config as cms

AK5PFJetsProducer = cms.EDProducer('AK5PFJetsProducer' ,
                            inputTag = cms.InputTag("ak5PFJets"),
                            outputFilename = cms.string("AK5PFJets"),
                            )
