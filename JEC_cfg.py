import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'GR_R_41_V0::All'

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )

process.source = cms.Source("EmptySource")

process.ak5 = cms.EDAnalyzer('JetCorrectorDBReader', 
        payloadName    = cms.untracked.string('AK5PF'),
	globalTag      = cms.untracked.string('GR_R_41_V0'),  
        printScreen    = cms.untracked.bool(False),
        createTextFile = cms.untracked.bool(True)
)
process.ak7 = cms.EDAnalyzer('JetCorrectorDBReader', 
        payloadName    = cms.untracked.string('AK7PF'),
	globalTag      = cms.untracked.string('GR_R_41_V0'),  
        printScreen    = cms.untracked.bool(False),
        createTextFile = cms.untracked.bool(True)
)

process.p = cms.Path(process.ak5*process.ak7)