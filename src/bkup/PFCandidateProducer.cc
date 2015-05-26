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

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Common/interface/HLTGlobalStatus.h"
#include "DataFormats/Provenance/interface/ParameterSetID.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

using namespace std;
using namespace edm;
using namespace trigger;

class PFCandidateProducer : public EDProducer 
{
public: 
  explicit PFCandidateProducer(const ParameterSet&);
  ~PFCandidateProducer();

private:
  virtual void beginJob() ;
  virtual void produce(Event&, const EventSetup&);
  virtual void endJob() ;

  virtual void beginRun(edm::Run&, edm::EventSetup const&);
  virtual void endRun(edm::Run&, edm::EventSetup const&);
  virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

  bool triggerFired(const string& triggerWildCard, const TriggerResults& triggerResults);
  unsigned int findTrigger(const string& triggerWildCard);
 
  InputTag inputTag_;
  
  ofstream fileOutput_;
  string outputFilename_;

  ofstream triggersOutput_;

  HLTConfigProvider hltConfig_;
  InputTag hltInputTag_;

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

  int eventSerialNumber_;
};

PFCandidateProducer::PFCandidateProducer(const ParameterSet& iConfig)
  : inputTag_(iConfig.getParameter<InputTag>("inputTag")),
    outputFilename_(iConfig.getParameter<string>("outputFilename")),
    hltConfig_(),
    hltInputTag_("TriggerResults","","HLT")
{
  fileOutput_.open(outputFilename_.c_str());
  triggersOutput_.open("triggers_pfcandidates");
}


PFCandidateProducer::~PFCandidateProducer() {}

void PFCandidateProducer::produce(Event& iEvent, const EventSetup& iSetup) {

  Handle<reco::PFCandidateCollection> collection;
  iEvent.getByLabel(inputTag_, collection);

  if ( ! collection.isValid()){
    cerr << "Invalid collection." << endl;
    return;
  }
  
  runNum = iEvent.id().run();
  eventNum = iEvent.id().event();
  
  cout << "Event number: " << eventSerialNumber_ << " being processed." << endl;
  eventSerialNumber_++;

  for(reco::PFCandidateCollection::const_iterator it = collection->begin(), end = collection->end(); it != end; it++) {
    particleType = (int) (*it).particleId();
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();

    pt = it->pt();
    eta = it->eta();
    phi = it->phi();
    
    fileOutput_ << runNum << "," << eventNum << "," << px << "," << py << "," << pz << "," << energy << endl;
  }

  // Jets info recorded
  // Now record trigger information.

  Handle<TriggerResults> trigResults; 
  iEvent.getByLabel(hltInputTag_, trigResults);

  const vector<string> triggerNames = hltConfig_.triggerNames();

  string triggers[7] = {"HLT_L1Jet6U", "HLT_L1Jet10U", "HLT_Jet15U", "HLT_Jet30U", "HLT_Jet50U", "HLT_Jet70U", "HLT_Jet100U"}; // Only these trigger info will be stored.
  vector<string> triggersThatMatter(triggers, triggers + sizeof triggers / sizeof triggers[0]);

  for (unsigned int i = 0; i < triggersThatMatter.size(); i++) {
    string name = triggersThatMatter[i];

    pair<int, int> prescale = hltConfig_.prescaleValues(iEvent, iSetup, name);
    bool fired = triggerFired(name, ( * trigResults));
    triggersOutput_ << eventNum << "," << runNum << "," << name << "," << fired << "," << prescale.first << "," << prescale.second << endl;
  }
}

void PFCandidateProducer::beginJob() {
  // fileOutput_ << "Run, Event, px, py, pz, energy, pt, eta, phi, particleType" << endl;
  eventSerialNumber_ = 1;
}

void PFCandidateProducer::endJob() {
  fileOutput_.close();
}

void PFCandidateProducer::beginRun(edm::Run & iRun, edm::EventSetup const & iSetup){

  bool changed = true;
  if ( hltConfig_.init(iRun, iSetup, hltInputTag_.process(), changed) ) {
    // if init returns TRUE, initialisation has succeeded!
    edm::LogInfo("TopPairElectronPlusJetsSelectionFilter") << "HLT config with process name "
        << hltInputTag_.process() << " successfully extracted";
  }
  else {
    edm::LogError("TopPairElectronPlusJetsSelectionFilter_Error")
        << "Error! HLT config extraction with process name " << hltInputTag_.process() << " failed";
  }

}

void PFCandidateProducer::endRun(edm::Run&, edm::EventSetup const&) {

}

void PFCandidateProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {

}

void PFCandidateProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {

}

bool PFCandidateProducer::triggerFired(const std::string& triggerWildCard, const edm::TriggerResults& triggerResults) {
  bool fired = false;
  unsigned int index = findTrigger(triggerWildCard);

  if (index < triggerResults.size()) {
    if (triggerResults.accept(index)) {
      fired = true;
    }
  }

  return fired;

}

unsigned int PFCandidateProducer::findTrigger(const std::string& triggerWildCard) {
  const std::vector<std::string>& triggers = hltConfig_.triggerNames();
  unsigned int found = 9999;

  size_t length = triggerWildCard.size();
  for (unsigned int index = 0; index < triggers.size(); ++index) {
    if (length <= triggers[index].size() && triggerWildCard == triggers[index].substr(0, length)) {
      found = index;
      break;
    }
  }

  return found;
}


DEFINE_FWK_MODULE(PFCandidateProducer);