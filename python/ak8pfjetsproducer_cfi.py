import FWCore.ParameterSet.Config as cms

AK8PFJetsProducer = cms.EDProducer('AK8PFJetsProducer' ,
                            inputTag = cms.InputTag("ak8PFJets"),
                            outputFilename = cms.string("AK8PFJets.dat"),
                            )
