#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include <TFile.h>
#include <TTree.h>





#include "DataFormats/PatCandidates/interface/TriggerFilter.h"


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

  // Triggers and Prescale Factors.

  edm::Handle< std::vector<int> > trigPreScalesHandle;
  event.getByLabel(edm::InputTag("dijetTriggerFilter", "prescales"), trigPreScalesHandle);

  edm::Handle< std::vector<std::string> > trigNamesHandle;
  event.getByLabel( edm::InputTag("dijetTriggerFilter", "jetPaths"), trigNamesHandle );

  double prescaleJet30U = 1;
  double prescaleJet50U = 1;
  double prescaleJet70U = 1;
  double prescaleJet100U = 1;
  
  // Jet30U
  std::vector<std::string>::const_iterator jet30U = find( trigNamesHandle->begin(), trigNamesHandle->end(), "HLT_Jet30U" );
  if ( jet30U != trigNamesHandle->end() ) {
    prescaleJet30U = trigPreScalesHandle->at( jet30U - trigNamesHandle->begin() );
    std::cout << prescaleJet30U << std::endl;
  }

  // Jet50U
  std::vector<std::string>::const_iterator jet50U = find( trigNamesHandle->begin(), trigNamesHandle->end(), "HLT_Jet50U" );
  if ( jet50U != trigNamesHandle->end() ) {
    prescaleJet50U = trigPreScalesHandle->at( jet50U - trigNamesHandle->begin() );
    std::cout << prescaleJet50U << std::endl;
  }

  // Jet70U
  std::vector<std::string>::const_iterator jet70U = find( trigNamesHandle->begin(), trigNamesHandle->end(), "HLT_Jet70U" );
  if ( jet70U != trigNamesHandle->end() ) {
    prescaleJet70U = trigPreScalesHandle->at( jet70U - trigNamesHandle->begin() );
    std::cout << prescaleJet70U << std::endl;
  }

  // Jet100U
  std::vector<std::string>::const_iterator jet100U = find( trigNamesHandle->begin(), trigNamesHandle->end(), "HLT_Jet100U" );
  if ( jet100U != trigNamesHandle->end() ) {
    prescaleJet100U = trigPreScalesHandle->at( jet100U - trigNamesHandle->begin() );
    std::cout << prescaleJet100U << std::endl;
  }

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
    
    // csvOut_ << runNum << "," << eventNum << "," << px << "," << py << "," << pz << "," << energy << "," << pt << "," << eta << "," << phi << "," << particleType << std::endl;
    csvOut_ << runNum << "," << eventNum << "," << px << "," << py << "," << pz << "," << energy << std::endl;
    pfCandidateTree_->Fill();
  }

  return true;
}

void PFCandidateFilter::beginJob() {
  std::cout << "Started my analysis job!" << std::endl;

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
