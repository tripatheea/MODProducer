import FWCore.ParameterSet.Config as cms

# particleFlow (container = *)
# particleFlowTmp (container = reco::PFCandidates)

PFCandidateFilter = cms.EDFilter('PFCandidateFilter' ,
                            pfCandidateInputTag = cms.InputTag("particleFlow"),
                            csvFileName = cms.string("pfcandidate.csv"),
                            rootFileName = cms.string("pfcandidate.root"),
			    			ptCut = cms.double(50),
                            maxNEvents = cms.int32(10000)
                            )
