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


class AK5PFJetsFilter : public edm::EDProducer 
{
public: 
  explicit AK5PFJetsFilter(const edm::ParameterSet&);
  ~AK5PFJetsFilter();

private:
  virtual void beginJob() ;
  virtual bool produce(edm::Event&, const edm::EventSetup&);
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

AK5PFJetsFilter::AK5PFJetsFilter(const edm::ParameterSet& iConfig)
  : inputTag_(iConfig.getParameter<edm::InputTag>("inputTag")),
    outputFilename_(iConfig.getParameter<std::string>("outputFilename"))
{ 
  fileOutput_.open(outputFilename_.c_str());
}


AK5PFJetsFilter::~AK5PFJetsFilter() {}

bool AK5PFJetsFilter::produce(edm::Event& event, const edm::EventSetup& eventSetup) {  
  
  edm::Handle<reco::PFJetCollection> collection;
  event.getByLabel(inputTag_, collection);

  if ( ! collection.isValid()){
    std::cerr << "Invalid collection." << std::endl;
    return false;
  }

  for(reco::PFJetCollection::const_iterator it = collection->begin(), end = collection->end(); it != end; it++) {
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    pt = it->pt();
    
    fileOutput_ << event.id().run() << "," << event.id().event() << "," << px << "," << py << "," << pz << "," << energy << std::endl;
  }

  return true;
}

void AK5PFJetsFilter::beginJob() {
  // fileOutput_ << "Run, Event, px, py, pz, energy, pt" << std::endl;
}

void AK5PFJetsFilter::endJob() {
  fileOutput_.close();
}


DEFINE_FWK_MODULE(AK5PFJetsFilter);
