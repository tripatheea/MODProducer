import FWCore.ParameterSet.Config as cms


#from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *
#from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
#from JetMETCorrections.Configuration.JetCorrectionProducers_cff import *

#from JetMETCorrections.Configuration.DefaultJEC_cff import *


# ak5PFL1Fastjet sort of works but that's a ESProducer.


PFCandidateProducer = cms.EDProducer('PFCandidateProducer' ,
                            JetCollectionName = cms.InputTag("ak5PFJets"),
                            JetCorrector      = cms.string("ak5PFL1Fastjet"),
                            )
