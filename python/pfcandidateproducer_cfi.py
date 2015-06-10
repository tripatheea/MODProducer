import FWCore.ParameterSet.Config as cms


from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *
from JetMETCorrections.Configuration.JetCorrectionServices_cff import *
from JetMETCorrections.Configuration.JetCorrectionProducers_cff import *

from JetMETCorrections.Configuration.DefaultJEC_cff import *






# Try ak5PFL3Absolute.





# ak5PFL1Fastjet sort of works but that's a ESProducer.


PFCandidateProducer = cms.EDProducer('PFCandidateProducer' ,
                            PFCandidateInputTag = cms.InputTag("particleFlow"),
                            srcCorrJets = cms.InputTag("ak5PFJets"),                            
                            AK5PFInputTag = cms.InputTag("ak5PFJets"),
                            AK7PFInputTag = cms.InputTag("ak7PFJets"),
                            outputFilename = cms.string("CMS_JetSample.dat"),
                            corrector = cms.string("ak5PFL1FastL2L3"),
                            )


correctedAK5  = cms.Sequence(
    ak5PFL1FastL2L3 *
    PFCandidateProducer
)



'''
# Jet energy corrections to use: 
#inputJetCorrLabel = ('AK5PF', ['L1Offset', 'L2Relative', 'L3Absolute', 'L2L3Residual'])

# no residual correction in 4_2_X: 
#inputJetCorrLabel = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute'])

# now residual correction in 4_2_X are available!!!
inputJetCorrLabel = ('AK5PF', ['L1FastJet', 'L2Relative', 'L3Absolute', 'L2L3Residual'])
'''