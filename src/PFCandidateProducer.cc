#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip> 
#include <limits>
#include <cmath>

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
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include "DataFormats/JetReco/interface/CaloJet.h" 
#include "DataFormats/JetReco/interface/GenJet.h"

#include "RecoJets/JetProducers/interface/BackgroundEstimator.h"

#include "FWCore/Framework/interface/InputSourceDescription.h"

#include "FWCore/Sources/interface/ExternalInputSource.h"
#include "FWCore/Sources/interface/EDInputSource.h"

#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/Sources/interface/EDInputSource.h"


#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"


#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequenceAreaBase.hh>
#include <fastjet/RangeDefinition.hh>


using namespace std;
using namespace edm;
using namespace trigger;
using namespace reco;
using namespace fastjet;


class PFCandidateProducer : public EDProducer 
{
public: 
   explicit PFCandidateProducer(const ParameterSet&);
   ~PFCandidateProducer();

private:
   virtual void beginJob();
   virtual void produce(edm::Event&, const edm::EventSetup&);
   virtual void endJob() ;
   virtual void beginRun(edm::Run&, edm::EventSetup const&);
   virtual void endRun(edm::Run&, edm::EventSetup const&);
   virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
   virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

   bool triggerFired(const string& triggerWildCard, const TriggerResults& triggerResults);
   unsigned int findTrigger(const string& triggerWildCard);
   
   InputTag srcCorrJets_;

   ofstream fileOutput_;
   

   HLTConfigProvider hltConfig_;
   InputTag hltInputTag_;
   string outputFilename_;
   InputTag rhoTag_;
   InputTag PFCandidateInputTag_;
   InputTag AK5PFInputTag_;
   InputTag AK7PFInputTag_;
   
   InputTag lumiSummaryLabel_;
   
   int runNum;
   int eventNum;

   int particleType;
   double px;
   double py;
   double pz;
   double energy;
   double mass;
   double area;
   
   int eventSerialNumber_;
   
   FactorizedJetCorrector *AK5JetCorrector_;
   FactorizedJetCorrector *AK7JetCorrector_;
   
   std::vector<std::string> filenames_;
};



PFCandidateProducer::PFCandidateProducer(const ParameterSet& iConfig)
: hltConfig_(),
  hltInputTag_("TriggerResults","","HLT"),
  outputFilename_(iConfig.getParameter<string>("outputFilename")),
  rhoTag_(iConfig.getParameter<edm::InputTag>("rho")),
  PFCandidateInputTag_(iConfig.getParameter<InputTag>("PFCandidateInputTag")),
  AK5PFInputTag_(iConfig.getParameter<edm::InputTag>("AK5PFInputTag")),
  AK7PFInputTag_(iConfig.getParameter<edm::InputTag>("AK7PFInputTag")),
  lumiSummaryLabel_(iConfig.getUntrackedParameter<edm::InputTag>("LumiSummaryLabel", InputTag("lumiProducer")))
{
  fileOutput_.open(outputFilename_.c_str());
}


PFCandidateProducer::~PFCandidateProducer() {

}

void PFCandidateProducer::produce(Event& iEvent, const EventSetup& iSetup) {

   runNum = iEvent.id().run();
   eventNum = iEvent.id().event();
   
   // Luminosity Block Begins
   
   
   LuminosityBlock const& iLumi = iEvent.getLuminosityBlock();
   Handle<LumiSummary> lumi;
   iLumi.getByLabel(lumiSummaryLabel_, lumi);
   
   if (lumi.isValid()) {
      cout << "average inst lumi: " << lumi->avgInsDelLumi() << endl;
      cout << "delivered luminosity integrated over LS: " << lumi->intgDelLumi() << endl;      
   }
   
   // Luminosity Block Ends
   
   
   fileOutput_ << "BeginEvent Run " << runNum << " Event " << eventNum << endl;
   
   Handle<reco::PFCandidateCollection> PFCollection;
   iEvent.getByLabel(PFCandidateInputTag_, PFCollection);
   
   Handle<TriggerResults> trigResults; 
   iEvent.getByLabel(hltInputTag_, trigResults);
   
   edm::Handle<reco::PFJetCollection> AK5Collection;
   iEvent.getByLabel(AK5PFInputTag_, AK5Collection);
   
   edm::Handle<reco::PFJetCollection> AK7Collection;
   iEvent.getByLabel(AK7PFInputTag_, AK7Collection);
   
   if ( ! PFCollection.isValid()){
    cerr << "Invalid collection." << endl;
    return;
   }
   
   if ( ! AK5Collection.isValid()){
    std::cerr << "Invalid collection." << std::endl;
    return;
   }
   
   if ( ! AK7Collection.isValid()){
    std::cerr << "Invalid collection." << std::endl;
    return;
   }
   

   
   // Setup things for JEC factors.
   
   // Cluster with FastJet to get median background pt density.
   
   vector<PseudoJet> PFCForFastJet;
   
   double rapmin = std::numeric_limits<double>::min();
   double rapmax = std::numeric_limits<double>::max();
   for(reco::PFCandidateCollection::const_iterator it = PFCollection->begin(), end = PFCollection->end(); it != end; it++) {
      PFCForFastJet.push_back(PseudoJet(it->px(), it->py(), it->pz(), it->energy()));
      
      if (it->rapidity() < rapmin)
      	rapmin = it->rapidity();
      if (it->rapidity() > rapmax)
      	rapmax = it->rapidity();
   }
   

   double R = 0.6;
   fastjet::JetDefinition jet_def(fastjet::kt_algorithm, R);
   ClusterSequenceAreaBase clust_seq(PFCForFastJet, jet_def);
   
   fastjet::RangeDefinition range = fastjet::RangeDefinition(rapmin, rapmax);
   BackgroundEstimator bckg = BackgroundEstimator(clust_seq, range);
   
   double rho = bckg.median_rho();
   
 
   // Record trigger information first.
  const vector<string> triggerNames = hltConfig_.triggerNames();
  //string triggers[7] = {"HLT_L1Jet6U", "HLT_L1Jet10U", "HLT_Jet15U", "HLT_Jet30U", "HLT_Jet50U", "HLT_Jet70U", "HLT_Jet100U"}; // Only these trigger info will be stored.
  //vector<string> triggersThatMatter(triggers, triggers + sizeof triggers / sizeof triggers[0]);
  
  vector<string> triggersThatMatter = triggerNames;
  /*
  for (unsigned int i = 0; i < triggersThatMatter.size(); i++) {
    if (i == 0)
       fileOutput_ << "# Trig                                       Name  Prescale_1  Prescale_2  Fired?" << endl;  
       
    string name = triggersThatMatter[i];
    
    // Only include triggers related to the Jet dataset.
        
    size_t found = name.find("Jet");
    
    if (found != string::npos) { 
	pair<int, int> prescale = hltConfig_.prescaleValues(iEvent, iSetup, name);
	bool fired = triggerFired(name, ( * trigResults));
	    
	fileOutput_ << "  Trig" 
		    << setw(43) <<  name 
		    << setw(12) << prescale.first 
		    << setw(12) << prescale.second 
		    << setw(8) << fired
        	    << endl;
      }
   }
   */

  // Get AK5 Jets.
  
  for(reco::PFJetCollection::const_iterator it = AK5Collection->begin(), end = AK5Collection->end(); it != end; it++) {    
    if (it == AK5Collection->begin())
       fileOutput_ << "# AK5" << "            px            py            pz        energy          mass           jec          area" << endl;
    
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    mass = it->mass();
    mass = (abs(mass) <= std::numeric_limits<double>::epsilon()) ? +0.00 : mass;
    area = it->jetArea();
    
    // JEC Factor.
    AK5JetCorrector_->setJetEta(it->eta());
    AK5JetCorrector_->setJetPt(it->pt());
    AK5JetCorrector_->setJetA(it->jetArea());
    AK5JetCorrector_->setRho(rho);

    double correction = AK5JetCorrector_->getCorrection();
    
    fileOutput_ << "  AK5"
        << setw(14) << fixed << setprecision(8) << px
        << setw(14) << fixed << setprecision(8) << py
        << setw(14) << fixed << setprecision(8) << pz
        << setw(14) << fixed << setprecision(8) << energy
        << setw(14) << fixed << setprecision(8) << mass
        << setw(14) << fixed << setprecision(8) << correction  
        << setw(14) << fixed << setprecision(8) << area       
        << endl;
  }
  
  // Get AK7 Jets.
  
  for(reco::PFJetCollection::const_iterator it = AK7Collection->begin(), end = AK7Collection->end(); it != end; it++) {    
    if (it == AK7Collection->begin())
       fileOutput_ << "# AK7" << "            px            py            pz        energy          mass           jec          area" << endl;
    
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    mass = it->mass();
    mass = (abs(mass) <= std::numeric_limits<double>::epsilon()) ? +0.00 : mass;
    area = it->jetArea();
    
    // JEC Factor.
    AK7JetCorrector_->setJetEta(it->eta());
    AK7JetCorrector_->setJetPt(it->pt());
    AK7JetCorrector_->setJetA(it->jetArea());
    AK7JetCorrector_->setRho(rho);

    double correction = AK7JetCorrector_->getCorrection();
    
    fileOutput_ << "  AK7"
        << setw(14) << fixed << setprecision(8) << px
        << setw(14) << fixed << setprecision(8) << py
        << setw(14) << fixed << setprecision(8) << pz
        << setw(14) << fixed << setprecision(8) << energy
        << setw(14) << fixed << setprecision(8) << mass
        << setw(14) << fixed << setprecision(8) << correction    
        << setw(14) << fixed << setprecision(8) << area     
        << endl;
  }
  
  // Get PFCandidates.
  for(reco::PFCandidateCollection::const_iterator it = PFCollection->begin(), end = PFCollection->end(); it != end; it++) {
    if (it == PFCollection->begin())
       fileOutput_ << "# PFC" << "            px            py            pz        energy          mass   pdgId" << endl;  
    
    particleType = (int) (*it).particleId();
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    mass = it->mass();
    mass = (abs(mass) <= std::numeric_limits<double>::epsilon()) ? +0.00 : mass;
    
    int pdgId = it->pdgId();
    fileOutput_ << "  PFC"
        << setw(14) << fixed << setprecision(8) << px
        << setw(14) << fixed << setprecision(8) << py
        << setw(14) << fixed << setprecision(8) << pz
        << setw(14) << fixed << setprecision(8) << energy
        << setw(14) << fixed << setprecision(8) << mass
        << setw(8) << noshowpos << pdgId
        << endl;
   }
  
   eventSerialNumber_++;
   fileOutput_ << "EndEvent" << endl;
  
}

void PFCandidateProducer::beginJob() {
   eventSerialNumber_ = 1;
   
   // Figure out the JetCorrector objects for AK5 and AK7 corrections.
   
   
   // AK5
   
   // Create the JetCorrectorParameter objects, the order does not matter.
   // YYYY is the first part of the txt files: usually the global tag from which they are retrieved
   JetCorrectorParameters *AK5ResJetPar = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK5PF_L2L3Residual.txt"); 
   JetCorrectorParameters *AK5L3JetPar  = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK5PF_L3Absolute.txt");
   JetCorrectorParameters *AK5L2JetPar  = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK5PF_L2Relative.txt");
   JetCorrectorParameters *AK5L1JetPar  = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK5PF_L1FastJet.txt");
   
   //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
   vector<JetCorrectorParameters> vParAK5;
   vParAK5.push_back(*AK5L1JetPar);
   vParAK5.push_back(*AK5L2JetPar);
   vParAK5.push_back(*AK5L3JetPar);
   vParAK5.push_back(*AK5ResJetPar);
   
   AK5JetCorrector_ = new FactorizedJetCorrector(vParAK5);
   
   // AK7
   
   // Create the JetCorrectorParameter objects, the order does not matter.
   // YYYY is the first part of the txt files: usually the global tag from which they are retrieved
   JetCorrectorParameters *AK7ResJetPar = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK7PF_L2L3Residual.txt"); 
   JetCorrectorParameters *AK7L3JetPar  = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK7PF_L3Absolute.txt");
   JetCorrectorParameters *AK7L2JetPar  = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK7PF_L2Relative.txt");
   JetCorrectorParameters *AK7L1JetPar  = new JetCorrectorParameters("data/JEC/GR_R_41_V0_AK7PF_L1FastJet.txt");
   
   //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
   vector<JetCorrectorParameters> vParAK7;
   vParAK7.push_back(*AK7L1JetPar);
   vParAK7.push_back(*AK7L2JetPar);
   vParAK7.push_back(*AK7L3JetPar);
   vParAK7.push_back(*AK7ResJetPar);
   
   AK7JetCorrector_ = new FactorizedJetCorrector(vParAK7);
   
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

void PFCandidateProducer::beginLuminosityBlock(edm::LuminosityBlock& iLumi, edm::EventSetup const& iSetup) {
   
}

void PFCandidateProducer::endLuminosityBlock(edm::LuminosityBlock& iLumi, edm::EventSetup const& iSetup) {

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
