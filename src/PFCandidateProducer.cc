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

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockFwd.h"

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
 
  InputTag PFCandidateInputTag_;
  InputTag AK5PFInputTag_;
  InputTag AK7PFInputTag_;
  
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


  int eventSerialNumber_;
};

PFCandidateProducer::PFCandidateProducer(const ParameterSet& iConfig)
  : PFCandidateInputTag_(iConfig.getParameter<InputTag>("PFCandidateInputTag")),
    AK5PFInputTag_(iConfig.getParameter<InputTag>("AK5PFInputTag")),
    AK7PFInputTag_(iConfig.getParameter<InputTag>("AK7PFInputTag")),
    outputFilename_(iConfig.getParameter<string>("outputFilename")),
    hltConfig_(),
    hltInputTag_("TriggerResults","","HLT")
{
  fileOutput_.open(outputFilename_.c_str());
}


PFCandidateProducer::~PFCandidateProducer() {}

void PFCandidateProducer::produce(Event& iEvent, const EventSetup& iSetup) {

  // Get PFCandidates first.

  Handle<reco::PFCandidateCollection> PFCollection;
  iEvent.getByLabel(PFCandidateInputTag_, PFCollection);

  if ( ! PFCollection.isValid()){
    cerr << "Invalid collection." << endl;
    return;
  }
  
  runNum = iEvent.id().run();
  eventNum = iEvent.id().event();
  
  cout << "Event number: " << eventSerialNumber_ << " being processed." << endl;

  fileOutput_ << "BeginEvent Run " << runNum << " Event " << eventNum << endl;
  fileOutput_ << "#PFC" << "          px          py          pz     energy       mass   pdgId" << endl;  

  eventSerialNumber_++;

  for(reco::PFCandidateCollection::const_iterator it = PFCollection->begin(), end = PFCollection->end(); it != end; it++) {
    particleType = (int) (*it).particleId();
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    mass = it->mass();
    int pdgId = it->pdgId();


  fileOutput_ << " PFC"
        << setw(12) << fixed << setprecision(5) << px
        << setw(12) << fixed << setprecision(5) << py
        << setw(12) << fixed << setprecision(5) << pz
        << setw(11) << fixed << setprecision(5) << energy
        << setw(11) << fixed << setprecision(5) << mass
        << setw(8) << noshowpos << pdgId
        << endl;

  }

  // Jets info recorded

  // Get AK5PFJets.
  edm::Handle<reco::PFJetCollection> AK5Collection;
  iEvent.getByLabel(AK5PFInputTag_, AK5Collection);

  if ( ! AK5Collection.isValid()){
    std::cerr << "Invalid collection." << std::endl;
    return;
  }

  fileOutput_ << "#AK5" << "          px          py          pz     energy" << endl;  

  eventSerialNumber_++;

  for(reco::PFJetCollection::const_iterator it = AK5Collection->begin(), end = AK5Collection->end(); it != end; it++) {
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();

  fileOutput_ << " AK5"
        << setw(12) << fixed << setprecision(5) << px
        << setw(12) << fixed << setprecision(5) << py
        << setw(12) << fixed << setprecision(5) << pz
        << setw(11) << fixed << setprecision(5) << energy
        << endl;
  }

  // Get AK7PFJets.
  edm::Handle<reco::PFJetCollection> AK7Collection;
  iEvent.getByLabel(AK7PFInputTag_, AK7Collection);

  if ( ! AK7Collection.isValid()){
    std::cerr << "Invalid collection." << std::endl;
    return;
  }

  fileOutput_ << "#AK7" << "          px          py          pz     energy" << endl;  

  eventSerialNumber_++;

  for(reco::PFJetCollection::const_iterator it = AK7Collection->begin(), end = AK7Collection->end(); it != end; it++) {
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();

  fileOutput_ << " AK7"
        << setw(12) << fixed << setprecision(5) << px
        << setw(12) << fixed << setprecision(5) << py
        << setw(12) << fixed << setprecision(5) << pz
        << setw(11) << fixed << setprecision(5) << energy
        << endl;
  }

  // Now record trigger information.

  fileOutput_ << "#Trig            Name     Prescale_1          Prescale_2           Fired?" << endl;  

  Handle<TriggerResults> trigResults; 
  iEvent.getByLabel(hltInputTag_, trigResults);

  const vector<string> triggerNames = hltConfig_.triggerNames();

  string triggers[7] = {"HLT_L1Jet6U", "HLT_L1Jet10U", "HLT_Jet15U", "HLT_Jet30U", "HLT_Jet50U", "HLT_Jet70U", "HLT_Jet100U"}; // Only these trigger info will be stored.
  vector<string> triggersThatMatter(triggers, triggers + sizeof triggers / sizeof triggers[0]);

  for (unsigned int i = 0; i < triggersThatMatter.size(); i++) {
    string name = triggersThatMatter[i];

    pair<int, int> prescale = hltConfig_.prescaleValues(iEvent, iSetup, name);
    bool fired = triggerFired(name, ( * trigResults));
    
    fileOutput_ << " trig" 
        << setw(16) <<  name 
        << setw(15) << prescale.first 
        << setw(20) << prescale.second 
        << setw(17) << fired
        << endl;

  }

  fileOutput_ << "EndEvent" << endl;
}

void PFCandidateProducer::beginJob() {
  eventSerialNumber_ = 1;
  std::cout << "Processing PFCandidates." << std::endl;
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
