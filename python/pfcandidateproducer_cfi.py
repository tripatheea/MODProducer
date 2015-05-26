import FWCore.ParameterSet.Config as cms

PFCandidateProducer = cms.EDProducer('PFCandidateProducer' ,
                            inputTag = cms.InputTag("particleFlow"),
                            outputFilename = cms.string("pfcandidates.dat"),
                            )
