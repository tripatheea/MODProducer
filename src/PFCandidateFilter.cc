#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>

#include <fstream>

#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockFwd.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"


#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefVector.h"


#include "RecoParticleFlow/PFProducer/interface/PFMuonAlgo.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementTrack.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementGsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"


#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateElectronExtraFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFDisplacedVertexFwd.h"
#include "DataFormats/EgammaCandidates/interface/ConversionFwd.h"
#include "DataFormats/Candidate/interface/VertexCompositeCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidatePhotonExtraFwd.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"


#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/IsolatedPFCandidate.h"
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/MuonReco/interface/Muon.h>



class PFCandidateFilter : public edm::EDFilter 
{
public: 
  explicit PFCandidateFilter(const edm::ParameterSet&);
  ~PFCandidateFilter();

private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
  edm::InputTag pfCandidateInputTag_;

  std::string rootFileName_;
  
  TFile * rootFile_;
  TTree * pfCandidateTree_;
  
  std::ofstream csvOut_;
  std::string csvFileName_;

  int maxNEvents_;
  int nEvents_;

  int runNum;
  int eventNum;

  int particleType;
  double px;
  double py;
  double pz;
  double energy;

  double pt;
  double eta;
  double phi;

  int eventSerialNumber;
};

PFCandidateFilter::PFCandidateFilter(const edm::ParameterSet& iConfig)
  : pfCandidateInputTag_(iConfig.getParameter<edm::InputTag>("pfCandidateInputTag")),
    rootFileName_(iConfig.getParameter<std::string>("rootFileName")),
    csvFileName_(iConfig.getParameter<std::string>("csvFileName")),
    maxNEvents_(iConfig.getParameter<int>("maxNEvents")),
    nEvents_(0)
{
  rootFile_ = new TFile(rootFileName_.c_str(), "RECREATE");
  pfCandidateTree_ = new TTree("PF Candidates", "ParticleFlow Candidates ");

  csvOut_.open(csvFileName_.c_str());
}


PFCandidateFilter::~PFCandidateFilter() {}

bool PFCandidateFilter::filter(edm::Event& event, const edm::EventSetup& eventSetup) {

  edm::Handle<reco::PFCandidateCollection> collection;
  event.getByLabel(pfCandidateInputTag_, collection);

  if ( ! collection.isValid()){
    std::cerr << "PFCandidateFilter: Invalid collection." << std::endl;
    return false;
  }

  runNum = event.id().run();
  eventNum = event.id().event();
  
  std::cout << "Event number: " << eventSerialNumber << " being processed." << std::endl;
  eventSerialNumber++;

  for(reco::PFCandidateCollection::const_iterator it = collection->begin(), end = collection->end(); it != end; it++) {
    particleType = (int) (*it).particleId();
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();

    pt = it->pt();
    eta = it->eta();
    phi = it->phi();
    
    csvOut_ << runNum << "," << eventNum << "," << px << "," << py << "," << pz << "," << energy << "," << pt << "," << eta << "," << phi << "," << particleType << std::endl;
    pfCandidateTree_->Fill();
  }
    
  return true;
}

void PFCandidateFilter::beginJob() {
  // csvOut_ << "Run, Event, px, py, pz, energy, pt, eta, phi, particleType" << std::endl;
  
  pfCandidateTree_->Branch("runNum", &runNum, "runNum/I"); // TTree::Branch(name, address, leaflist. leaflist is the concatenation of all variable names and types. The variable name and variable type (1 character) are separated by a slash.
  pfCandidateTree_->Branch("eventNum", &eventNum, "eventNum/I");
  pfCandidateTree_->Branch("px", &px, "px/D");
  pfCandidateTree_->Branch("py", &py, "py/D");
  pfCandidateTree_->Branch("pz", &pz, "pz/D");
  pfCandidateTree_->Branch("energy", &energy, "energy/D");
  pfCandidateTree_->Branch("pt", &pt, "pt/D");
  pfCandidateTree_->Branch("eta", &eta, "eta/D");
  pfCandidateTree_->Branch("phi", &phi, "phi/D");
  pfCandidateTree_->Branch("particleType", &particleType, "particleType/I");
  
  eventSerialNumber = 1;
}

void PFCandidateFilter::endJob() {
  rootFile_->cd();
  pfCandidateTree_->Write();
  rootFile_->Close();

  csvOut_.close();
}


DEFINE_FWK_MODULE(PFCandidateFilter);
