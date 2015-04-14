import FWCore.ParameterSet.Config as cms

# particleFlow (container = *)
# particleFlowTmp (container = reco::PFCandidates)

minBiasFilter = cms.EDFilter('minBiasFilter' ,
                            minBiasInputTag = cms.InputTag("particleFlow"),
                            csvFileName = cms.string("minBias.csv"),
                            rootFileName = cms.string("minBias.root"),
                            maxNEvents = cms.int32(1000)
                            )
