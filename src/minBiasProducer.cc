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

#include "JetMETCorrections/MinBias/interface/MinBias.h"
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

class minBiasProducer : public EDProducer 
{
public: 
  explicit minBiasProducer(const ParameterSet&);
  ~minBiasProducer();

private:
  virtual void beginJob();
  virtual void produce(Event&, const EventSetup&);
  virtual void endJob();

  virtual void beginRun(Run&, EventSetup const&);
  virtual void endRun(Run&, EventSetup const&);
  virtual void beginLuminosityBlock(LuminosityBlock&, EventSetup const&);
  virtual void endLuminosityBlock(LuminosityBlock&, EventSetup const&);

  bool triggerFired(const string& triggerWildCard, const TriggerResults& triggerResults);
  unsigned int findTrigger(const string& triggerWildCard);
 
  InputTag inputTag_;
  
  ofstream fileOutput_;
  string outputFilename_;

  HLTConfigProvider hltConfig_;
  InputTag hltInputTag_;

  int runNum;
  int eventNum;

  int particleType;
  double px;
  double py;
  double pz;
  double energy;
  double mass;

  double pt;
  double eta;
  double phi;

  int eventSerialNumber_;
};

minBiasProducer::minBiasProducer(const ParameterSet& iConfig)
  : inputTag_(iConfig.getParameter<InputTag>("inputTag")),
    outputFilename_(iConfig.getParameter<string>("outputFilename")),
    hltConfig_(),
    hltInputTag_("TriggerResults","","HLT")
{
  fileOutput_.open(outputFilename_.c_str());
}


minBiasProducer::~minBiasProducer() {}

void minBiasProducer::produce(Event& iEvent, const EventSetup& iSetup) {

  Handle<reco::PFCandidateCollection> collection;
  iEvent.getByLabel(inputTag_, collection);

  if ( ! collection.isValid()){
    cerr << "Invalid collection." << endl;
    return;
  }
  
  runNum = iEvent.id().run();
  eventNum = iEvent.id().event();
  
  cout << "Event number: " << eventSerialNumber_ << " being processed." << endl;

  fileOutput_ << "BeginEvent Run " << runNum << " Event " << eventNum << endl;  
  fileOutput_ << "#PFC               px               py               pz               energy               mass               pdgId" << fixed << endl;

  eventSerialNumber_++;

  for(reco::PFCandidateCollection::const_iterator it = collection->begin(), end = collection->end(); it != end; it++) {
    particleType = (int) (*it).particleId();
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    mass = it->mass();
    int pdgId = it->pdgId();

    pt = it->pt();
    eta = it->eta();
    phi = it->phi();
    
    fileOutput_ << "PFC " 
		<< setw(21) << setprecision(8) << px
		<< setw(17) << setprecision(8) << py
		<< setw(18) << setprecision(8) << pz
		<< setw(18) << setprecision(8) << energy
		<< setw(19) << setprecision(5) << mass
		<< setw(18) << noshowpos << pdgId
		<< endl;
  }


  // Jets info recorded
  // Now record trigger information.

  fileOutput_ << "#Trig          Name          Prescale_1          Prescale_2          Fired?" << endl;

  Handle<TriggerResults> trigResults; 
  iEvent.getByLabel(hltInputTag_, trigResults);

  const vector<string> triggerNames = hltConfig_.triggerNames();

  string triggers[1] = {"HLT_MinBiasPixel_SingleTrack"}; // Only these trigger info will be stored.
  vector<string> triggersThatMatter(triggers, triggers + sizeof triggers / sizeof triggers[0]);

  for (unsigned int i = 0; i < triggersThatMatter.size(); i++) {
    string name = triggersThatMatter[i];

    pair<int, int> prescale = hltConfig_.prescaleValues(iEvent, iSetup, name);
    bool fired = triggerFired(name, ( * trigResults));
    
    fileOutput_ << "trig" 
		<< setw(16) << name
		<< setw(15) << prescale.first 
		<< setw(20) << prescale.second 
		<< setw(17) << fired 
		<< endl;

  }

  fileOutput_ << "EndEvent" << endl;
  
}

void minBiasProducer::beginJob() {
  eventSerialNumber_ = 1;
}

void minBiasProducer::endJob() {
  fileOutput_.close();
}

void minBiasProducer::beginRun(Run & iRun, EventSetup const & iSetup){

  bool changed = true;
  if ( hltConfig_.init(iRun, iSetup, hltInputTag_.process(), changed) ) {
    // if init returns TRUE, initialisation has succeeded!
    LogInfo("TopPairElectronPlusJetsSelectionFilter") << "HLT config with process name "
        << hltInputTag_.process() << " successfully extracted";
  }
  else {
    LogError("TopPairElectronPlusJetsSelectionFilter_Error")
        << "Error! HLT config extraction with process name " << hltInputTag_.process() << " failed";
  }

}

void minBiasProducer::endRun(Run&, EventSetup const&) {

}

void minBiasProducer::beginLuminosityBlock(LuminosityBlock&, EventSetup const&) {

}

void minBiasProducer::endLuminosityBlock(LuminosityBlock&, EventSetup const&) {

}

bool minBiasProducer::triggerFired(const string& triggerWildCard, const TriggerResults& triggerResults) {
  bool fired = false;
  unsigned int index = findTrigger(triggerWildCard);

  if (index < triggerResults.size()) {
    if (triggerResults.accept(index)) {
      fired = true;
    }
  }

  return fired;

}

unsigned int minBiasProducer::findTrigger(const string& triggerWildCard) {
  const vector<string>& triggers = hltConfig_.triggerNames();
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


DEFINE_FWK_MODULE(minBiasProducer);
