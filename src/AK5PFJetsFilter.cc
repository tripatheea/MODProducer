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

#include <TH1.h>
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

#include "TH1D.h"
#include "TMath.h"
#include <vector>
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/IsolatedPFCandidate.h"
#include <DataFormats/TrackReco/interface/Track.h>
#include <DataFormats/MuonReco/interface/Muon.h>



class AK5PFJetsFilter : public edm::EDFilter 
{
public: 
  explicit AK5PFJetsFilter(const edm::ParameterSet&);
  ~AK5PFJetsFilter();

private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
  edm::InputTag AK5PFJetsInputTag_;

  std::string rootFileName_;
  
  TFile * rootFile_;
  TTree * AK5PFJetsTree_;
  
  std::ofstream csvOut_;
  std::string csvFileName_;

  int maxNEvents_;
  int nEvents_;

  int runNum;
  int eventNum;

  double px;
  double py;
  double pz;
  double energy;

  double pt;

};

AK5PFJetsFilter::AK5PFJetsFilter(const edm::ParameterSet& iConfig)
  : AK5PFJetsInputTag_(iConfig.getParameter<edm::InputTag>("AK5PFJetsInputTag")),
    rootFileName_(iConfig.getParameter<std::string>("rootFileName")),
    csvFileName_(iConfig.getParameter<std::string>("csvFileName")),
    maxNEvents_(iConfig.getParameter<int>("maxNEvents")),
    nEvents_(0)
{
  rootFile_ = new TFile(rootFileName_.c_str(), "RECREATE");
  AK5PFJetsTree_ = new TTree("AK5PFJets", "AK5PFJets");
  
  csvOut_.open(csvFileName_.c_str());
}


AK5PFJetsFilter::~AK5PFJetsFilter() {}

bool AK5PFJetsFilter::filter(edm::Event& event, const edm::EventSetup& eventSetup) {  
  
  edm::Handle<reco::PFJetCollection> collection;
  event.getByLabel(AK5PFJetsInputTag_, collection);

  if ( ! collection.isValid()){
    std::cerr << "AK5PFJetsFilter: Invalid collection." << std::endl;
    return false;
  }
  
  std::cout << "Valid collection created." << std::endl;

  for(reco::PFJetCollection::const_iterator it = collection->begin(), end = collection->end(); it != end; it++) {
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    pt = it->pt();
    
    csvOut_ << event.id().run() << "," << event.id().event() << "," << px << "," << py << "," << pz << "," << energy << "," << pt << std::endl;
    AK5PFJetsTree_->Fill();
  }

  return true;
}

void AK5PFJetsFilter::beginJob() {
  csvOut_ << "Run, Event, px, py, pz, energy, pt" << std::endl;

  AK5PFJetsTree_->Branch("runNum", &runNum, "runNum/I"); // TTree::Branch(name, address, leaflist. leaflist is the concatenation of all variable names and types. The variable name and variable type (1 character) are separated by a slash.
  AK5PFJetsTree_->Branch("eventNum", &eventNum, "eventNum/I");
  AK5PFJetsTree_->Branch("px", &px, "px/D");
  AK5PFJetsTree_->Branch("py", &py, "py/D");
  AK5PFJetsTree_->Branch("pz", &pz, "pz/D");
  AK5PFJetsTree_->Branch("energy", &energy, "energy/D");
  AK5PFJetsTree_->Branch("pt", &pt, "pt/D");
  
}

void AK5PFJetsFilter::endJob() {
  rootFile_->cd();
  AK5PFJetsTree_->Write();
  rootFile_->Close();

  csvOut_.close();
}


DEFINE_FWK_MODULE(AK5PFJetsFilter);
