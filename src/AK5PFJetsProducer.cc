#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

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


class AK5PFJetsProducer : public edm::EDProducer 
{
public: 
  explicit AK5PFJetsProducer(const edm::ParameterSet&);
  ~AK5PFJetsProducer();

private:
  virtual void beginJob() ;
  virtual void produce(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
  edm::InputTag inputTag_;

  std::ofstream fileOutput_;
  std::string outputFilename_;

  

  int runNum;
  int eventNum;

  double px;
  double py;
  double pz;
  double energy;

  double pt;

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

  for(reco::PFJetCollection::const_iterator it = collection->begin(), end = collection->end(); it != end; it++) {
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    pt = it->pt();
    
    fileOutput_ << iEvent.id().run() << "," << iEvent.id().event() << "," << px << "," << py << "," << pz << "," << energy << std::endl;
  }

}

void AK5PFJetsProducer::beginJob() {
  // fileOutput_ << "Run, Event, px, py, pz, energy, pt" << std::endl;
}

void AK5PFJetsProducer::endJob() {
  fileOutput_.close();
}


DEFINE_FWK_MODULE(AK5PFJetsProducer);
