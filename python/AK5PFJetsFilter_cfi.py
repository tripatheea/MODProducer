import FWCore.ParameterSet.Config as cms

# particleFlow (container = *)
# particleFlowTmp (container = reco::PFCandidates)

AK5PFJetsFilter = cms.EDFilter('AK5PFJetsFilter' ,
                            AK5PFJetsInputTag = cms.InputTag("ak5PFJets"),
                            csvFileName = cms.string("AK5PFJets.csv"),
                            rootFileName = cms.string("AK5PFJets.root"),
                            maxNEvents = cms.int32(1)
                            )
