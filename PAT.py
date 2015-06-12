from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.coreTools import *

process.p = cms.Path(process.patDefaultSequence)

process.GlobalTag.globaltag = 'GR_R_42_V25::All' 
process.source = cms.Source("PoolSource", fileNames = cms.untracked.vstring('file://00052C5A-EF70-E011-B43F-00266CF32A00.root'))
process.maxEvents.input = 100
process.out.fileName = 'file://pat.root'
