#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip> 

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockFwd.h"


using namespace std;
using namespace edm;

class AK5PFJetsProducer : public edm::EDProducer {

public: 
  explicit AK5PFJetsProducer(const edm::ParameterSet&);
  ~AK5PFJetsProducer();

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  
  edm::InputTag inputTag_;

  std::ofstream fileOutput_;
  std::string outputFilename_;

  InputTag hltInputTag_;

  int runNum;
  int eventNum;

  double px;
  double py;
  double pz;
  double energy;
  double mass;

  int eventSerialNumber_;
};

AK5PFJetsProducer::AK5PFJetsProducer(const edm::ParameterSet& iConfig)
  : inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
    outputFilename_(iConfig.getParameter<std::string>("outputFilename"))
{ 
  fileOutput_.open(outputFilename_.c_str());
}


AK5PFJetsProducer::~AK5PFJetsProducer() {}

void AK5PFJetsProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {  
  
  edm::Handle<reco::PFJetCollection> collection;
  iEvent.getByLabel(inputTag_, collection);

  if ( ! collection.isValid()){
    std::cerr << "Invalid collection." << std::endl;
    return;
  }

  runNum = iEvent.id().run();
  eventNum = iEvent.id().event();
  
  cout << "Event number: " << eventSerialNumber_ << " being processed." << endl;

  fileOutput_ << "BeginEvent Run " << runNum << " Event " << eventNum << endl;
  fileOutput_ << "#AK5" << "          px          py          pz     energy" << endl;  

  eventSerialNumber_++;

  for(reco::PFJetCollection::const_iterator it = collection->begin(), end = collection->end(); it != end; it++) {
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    //mass = it->mass();
    //int pdgId = it->pdgId();
    mass = 0.0;
    int pdgId = 0;

  fileOutput_ << " AK5"
        << setw(12) << fixed << setprecision(5) << px
        << setw(12) << fixed << setprecision(5) << py
        << setw(12) << fixed << setprecision(5) << pz
        << setw(11) << fixed << setprecision(5) << energy
        << endl;
  }

  // Jets info recorded

  fileOutput_ << "EndEvent" << endl;

}

void AK5PFJetsProducer::beginJob() {
  eventSerialNumber_ = 1;
}

void AK5PFJetsProducer::endJob() {
  fileOutput_.close();
}

void AK5PFJetsProducer::beginRun(edm::Run & iRun, edm::EventSetup const & iSetup){

}

void AK5PFJetsProducer::endRun(edm::Run&, edm::EventSetup const&) {

}

void AK5PFJetsProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {

}

void AK5PFJetsProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {

}



DEFINE_FWK_MODULE(AK5PFJetsProducer);
