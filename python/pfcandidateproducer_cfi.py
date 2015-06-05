import FWCore.ParameterSet.Config as cms

PFCandidateProducer = cms.EDProducer('PFCandidateProducer' ,
                            PFCandidateInputTag = cms.InputTag("particleFlow"),
                            AK5PFInputTag = cms.InputTag("ak5PFJets"),
                            AK7PFInputTag = cms.InputTag("ak7PFJets"),
                            outputFilename = cms.string("CMS_JetSample.dat"),
                            )
