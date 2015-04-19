import FWCore.ParameterSet.Config as cms

# particleFlow (container = *)
# particleFlowTmp (container = reco::PFCandidates)

zeroBiasFilter = cms.EDFilter('zeroBiasFilter' ,
                            zeroBiasInputTag = cms.InputTag("particleFlow"),
                            csvFileName = cms.string("zeroBias.csv"),
                            rootFileName = cms.string("zeroBias.root"),
                            maxNEvents = cms.int32(1000)
                            )
