import FWCore.ParameterSet.Config as cms
import FWCore.Utilities.FileUtils as FileUtils

source = "local" # or "remote"

# Need to split into multiple separate files because there's a limit of 255 values for a set of parameters and thereby for the indices files too.
indices_file = "file_paths/Jet/" + source + "/CMS_Run2010B_Jet_AOD_Apr21ReReco-v1_000"	# Should append "_" + indexNumber + ".txt" to this.

mylist = FileUtils.loadListFromFile (indices_file + '0_file_index.txt')
#mylist = FileUtils.loadListFromFile (indices_file + '1_file_index.txt')
#mylist.extend(FileUtils.loadListFromFile(indices_file + '2_file_index.txt') )
#mylist.extend(FileUtils.loadListFromFile(indices_file + '3_file_index.txt') )
#mylist.extend(FileUtils.loadListFromFile(indices_file + '4_file_index.txt') )

readFiles = cms.untracked.vstring( *mylist)

process = cms.Process("myprocess")
process.source = cms.Source ("PoolSource", fileNames=readFiles)


process.PFCandidateProducer = cms.EDProducer("PFCandidateProducer",
					rho = cms.InputTag("kt6PFJets","rho"),
					PFCandidateInputTag = cms.InputTag("particleFlow"),
					AK5PFInputTag = cms.InputTag("ak5PFJets"),
					outputBasePath = cms.string("/media/sf_virtual_machine/CMS/MOD/"),
					mapFilename = cms.string("map.mod")
				)

				
process.producer = cms.Path(process.PFCandidateProducer)
process.schedule = cms.Schedule( process.producer )
