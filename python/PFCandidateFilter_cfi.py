import FWCore.ParameterSet.Config as cms

PFCandidateFilter = cms.EDFilter('PFCandidateFilter' ,
                            muonInputTag = cms.InputTag("pfcandidates"),
                            csvFileName = cms.string("pfcandidate.csv"),
                            rootFileName = cms.string("pfcandidate.root"),
                            binInterval = cms.double(0.05),
                            invariantMassMin = cms.double(2.0),
                            invariantMassMax = cms.double(110.0),
                            maxNEvents = cms.int32(100)
                            )
