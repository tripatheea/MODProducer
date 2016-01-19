#include <memory>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <limits>
#include <cmath>
#include <algorithm>
#include <sys/time.h>
#include <sys/stat.h>

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

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"


#include "RecoJets/JetProducers/interface/BackgroundEstimator.h"

#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"

#include "HLTrigger/HLTcore/interface/HLTConfigData.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"



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


   bool file_exists(const std::string& name);

   
   InputTag srcCorrJets_;


   HLTConfigProvider hltConfig_;
   InputTag hltInputTag_;
   InputTag rhoTag_;
   InputTag PFCandidateInputTag_;
   InputTag AK5PFInputTag_;
   
   InputTag lumiSummaryLabel_;
   
   int runNum;
   int eventNum;
   edm::LuminosityBlockNumber_t lumiBlockNumber_;
   
   long int startTime_;
   long int endTime_;

   int particleType;
   double px;
   double py;
   double pz;
   double energy;
   double mass;
   double area;
   
   long int eventSerialNumber_;
   
   FactorizedJetCorrector * AK5JetCorrector_;

   std::vector<std::string> filenames_;
   
   string mapFilename_;
   ifstream mapFile_;

   
   ifstream mapNumbersFile_;
   
   string outputDir_;
   ofstream fileOutput_;
   
   stringstream output_;
   
   string outputFilename_;
   string lastOutputFilename_;

   bool processFromTheBeginning_;
   
   string inputFile_;
   
   InputTag primaryVertices_;
   string dataVersion_;
};



PFCandidateProducer::PFCandidateProducer(const ParameterSet& iConfig)
: hltConfig_(),
  hltInputTag_("TriggerResults","","HLT"),
  rhoTag_(iConfig.getParameter<edm::InputTag>("rho")),
  PFCandidateInputTag_(iConfig.getParameter<InputTag>("PFCandidateInputTag")),
  AK5PFInputTag_(iConfig.getParameter<edm::InputTag>("AK5PFInputTag")),
  lumiSummaryLabel_(iConfig.getUntrackedParameter<edm::InputTag>("LumiSummaryLabel", InputTag("lumiProducer"))),
  primaryVertices_(iConfig.getParameter<InputTag>("primaryVertices")),
  dataVersion_(iConfig.getParameter<string>("dataVersion"))
{
  mapFilename_ = iConfig.getParameter<string>("mapFilename");
  mapFile_.open(mapFilename_.c_str()); 
  
  outputDir_ = iConfig.getParameter<string>("outputDir");
  outputFilename_ = "";
  lastOutputFilename_ = "";

  cout << outputDir_ << endl;

  processFromTheBeginning_ = iConfig.getParameter<bool>("processFromTheBeginning");

  if ( ! processFromTheBeginning_)
    inputFile_ = iConfig.getParameter<string>("inputFile");
	  
}


PFCandidateProducer::~PFCandidateProducer() {

}

void PFCandidateProducer::produce(Event& iEvent, const EventSetup& iSetup) {

   string line, directory, filename;
   int fileEventNum, fileRunNum;

   getline(mapFile_, line);
   istringstream iss(line);
  	
   iss >> fileEventNum >> fileRunNum >> directory >> filename;

   runNum = iEvent.id().run();
   eventNum = iEvent.id().event();
   lumiBlockNumber_ = iEvent.luminosityBlock();
   
   if ((fileRunNum == runNum) && (fileEventNum == eventNum)) {
   	
   	outputFilename_ = outputDir_ + "/" + filename + ".mod";
	
	if ((eventSerialNumber_ == 1) || (outputFilename_ != lastOutputFilename_)) {
	   fileOutput_.close();
	   fileOutput_.open(outputFilename_.c_str(), ios::out | ios::app );
	   lastOutputFilename_ = outputFilename_;
	}
   }
   
   output_.str("");
   output_.clear(); // Clear state flags.
   
   output_ << "BeginEvent Version " << dataVersion_ << " CMS_2010 Jet_Primary_Dataset" << endl;
   
   // Primary Vertices.
   edm::Handle<VertexCollection> primaryVerticesHandle;
   iEvent.getByLabel( edm::InputTag("offlinePrimaryVertices"), primaryVerticesHandle);
   
   
   // Luminosity Block Begins
   
   LuminosityBlock const& iLumi = iEvent.getLuminosityBlock();
   Handle<LumiSummary> lumi;
   iLumi.getByLabel(lumiSummaryLabel_, lumi);
   
   double avgInstLumi = -3.1415;
   if (lumi.isValid())
      avgInstLumi = lumi->avgInsDelLumi();
   

   //cout << lumi->isValid() << " " << lumi->intgDelLumi() << "  " << lumi->lumiSecQual() << "  " << lumi->deadcount() << "  " << " " << lumi->deadFrac() << " " << lumi->liveFrac() << " " << lumi->lumiSectionLength() << " " << lumi->lsNumber() << " " << lumi->intgRecLumi() << " " << lumi->startOrbit() << " " << lumi->numOrbit() << std::endl;
   
   // Luminosity Block Ends
   output_ << "#   Cond          RunNum        EventNum       LumiBlock     AvgInstLumi             NPV       validLumi     intgDelLumi       deadCount    liveFraction   lumiSecLength     intgRecLumi        numOrbit" << endl;
   output_ << "    Cond"
   	       << setw(16) << runNum
	       << setw(16) << eventNum
	       << setw(16) << lumiBlockNumber_
   	       << setw(16) << lumi->avgInsDelLumi()
   	       << setw(16) << primaryVerticesHandle->size()
   	       << setw(16) << lumi->isValid()
   	       << setw(16) << lumi->intgDelLumi()
   	       << setw(16) << lumi->deadcount()
   	       << setw(16) << lumi->liveFrac()
   	       << setw(16) << lumi->lumiSectionLength()
   	       << setw(16) << lumi->intgRecLumi()
   	       << setw(16) << lumi->numOrbit()
 	       << endl;   
   	       
   
   Handle<reco::PFCandidateCollection> PFCollection;
   iEvent.getByLabel(PFCandidateInputTag_, PFCollection);
   
   Handle<TriggerResults> trigResults; 
   iEvent.getByLabel(hltInputTag_, trigResults);
   
   edm::Handle<reco::PFJetCollection> AK5Collection;
   iEvent.getByLabel(AK5PFInputTag_, AK5Collection);
   
   if ( ! PFCollection.isValid()){
    cerr << "Invalid collection." << endl;
    return;
   }
   
   if ( ! AK5Collection.isValid()){
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
   
   
   // Record trigger information first.
   
   
   // Get all trigger names associated with the "Jet" dataset.
   const vector<string> triggerNames = hltConfig_.datasetContent("Jet");
   /*
   for (unsigned i = 0; i < triggerNames.size(); i++) {
      if (i == 0)
         output_ << "#   Trig                            Name      Prescale_1      Prescale_2          Fired?" << endl;
      
      string name = triggerNames[i];
      
      pair<int, int> prescale = hltConfig_.prescaleValues(iEvent, iSetup, name);

      bool fired = triggerFired(name, ( * trigResults));

      output_ << "    Trig"
       	          << setw(32) << name
	          << setw(16) << prescale.first
	          << setw(16) << prescale.second
                  << setw(16) << fired
                  << endl;
   }
   */
  // Get AK5 Jets.
  
  // Setup background density for AK5 JEC.
  
  edm::Handle<double> rhoHandle;
  iEvent.getByLabel( edm::InputTag("kt6PFJetsForIsolation", "rho"), rhoHandle);
  double rho = * rhoHandle;
  
  for(reco::PFJetCollection::const_iterator it = AK5Collection->begin(), end = AK5Collection->end(); it != end; it++) {    
    if (it == AK5Collection->begin())
       output_ << "#    AK5" << "              px              py              pz          energy             jec            area     no_of_const     chrg_multip    neu_had_frac     neu_em_frac   chrg_had_frac    chrg_em_frac" << endl;
    
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    area = it->jetArea();
    
    // JEC Factor.
    
    AK5JetCorrector_->setJetEta(it->eta());
    AK5JetCorrector_->setJetPt(it->pt());
    AK5JetCorrector_->setJetA(it->jetArea());
    AK5JetCorrector_->setRho(rho);
         
    double correction = AK5JetCorrector_->getCorrection();

    // Jet Quality Cut Parameters.

    double neutral_hadron_fraction = it->neutralHadronEnergy() / it->energy();
    double neutral_em_fraction = it->neutralEmEnergy() / it->energy();
    int number_of_constituents = it->nConstituents();
    double charged_hadron_fraction = it->chargedHadronEnergy() / it->energy();
    int charged_multiplicity = it->chargedMultiplicity();
    double charged_em_fraction = it->chargedEmEnergy() / it->energy();
 
    output_ << "     AK5"
        << setw(16) << fixed << setprecision(8) << px
        << setw(16) << fixed << setprecision(8) << py
        << setw(16) << fixed << setprecision(8) << pz
        << setw(16) << fixed << setprecision(8) << energy
        << setw(16) << fixed << setprecision(8) << correction
        << setw(16) << fixed << setprecision(8) << area   
        << setw(16) << fixed << setprecision(8) << number_of_constituents   
        << setw(16) << fixed << setprecision(8) << charged_multiplicity  
        << setw(16) << fixed << setprecision(8) << neutral_hadron_fraction   
        << setw(16) << fixed << setprecision(8) << neutral_em_fraction   
        << setw(16) << fixed << setprecision(8) << charged_hadron_fraction    
        << setw(16) << fixed << setprecision(8) << charged_em_fraction       
        << endl;
  }
  
  
  // Get PFCandidates.
  for(reco::PFCandidateCollection::const_iterator it = PFCollection->begin(), end = PFCollection->end(); it != end; it++) {
    if (it == PFCollection->begin())
       output_ << "#    PFC" << "              px              py              pz          energy           pdgId" << endl;  
    
    px = it->px();
    py = it->py();
    pz = it->pz();
    energy = it->energy();
    int pdgId = it->pdgId();
    
    output_ << "     PFC"
        << setw(16) << fixed << setprecision(8) << px
        << setw(16) << fixed << setprecision(8) << py
        << setw(16) << fixed << setprecision(8) << pz
        << setw(16) << fixed << setprecision(8) << energy
        << setw(16) << noshowpos << pdgId
        << endl;
   }
   
   output_ << "EndEvent" << endl;
   
   fileOutput_ << output_.rdbuf();
   
   eventSerialNumber_++;
}

void PFCandidateProducer::beginJob() {
   eventSerialNumber_ = 1;
   
   // Start timer.
   struct timeval tp;
   gettimeofday(&tp, NULL);
   startTime_ = tp.tv_sec * 1000 + tp.tv_usec / 1000;
   
   // Figure out the JetCorrector objects for AK5 corrections.
   
   
   // AK5
   
   // Create the JetCorrectorParameter objects, the order does not matter.
   // YYYY is the first part of the txt files: usually the global tag from which they are retrieved
   JetCorrectorParameters *AK5ResJetPar = new JetCorrectorParameters("data/JEC/GR_R_42_V25_AK5PF_L2L3Residual.txt"); 
   JetCorrectorParameters *AK5L3JetPar  = new JetCorrectorParameters("data/JEC/GR_R_42_V25_AK5PF_L3Absolute.txt");
   JetCorrectorParameters *AK5L2JetPar  = new JetCorrectorParameters("data/JEC/GR_R_42_V25_AK5PF_L2Relative.txt");
   JetCorrectorParameters *AK5L1JetPar  = new JetCorrectorParameters("data/JEC/GR_R_42_V25_AK5PF_L1FastJet.txt");
   
   //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
   vector<JetCorrectorParameters> vParAK5;
   vParAK5.push_back(*AK5L1JetPar);
   vParAK5.push_back(*AK5L2JetPar);
   vParAK5.push_back(*AK5L3JetPar);
   vParAK5.push_back(*AK5ResJetPar);
   
   AK5JetCorrector_ = new FactorizedJetCorrector(vParAK5);

   
   std::cout << "Processing PFCandidates." << std::endl;
   
   // Map thingy.

   if ( ! processFromTheBeginning_) {
   	
   	string line, directory;
   	int fileEventNum, fileRunNum;
   	int linesDown = 1;
   
	ifstream registryFile(mapFilename_.c_str());
   
   	string rootFilename = "";
   	
   	while((rootFilename != inputFile_)) {
   		
		getline(registryFile, line);
		istringstream iss(line);
   		iss >> fileEventNum >> fileRunNum >> directory >> rootFilename;
   		linesDown++;
	}
	
	cout << "Trying to find the correct line here!" << endl;
	cout << linesDown << endl;
	
	for(int i = 0; i < linesDown - 2; i++) {
		getline(mapFile_, line);
		istringstream iss(line);
		iss >> fileEventNum >> fileRunNum >> directory >> rootFilename;
	}
	
   }


}

void PFCandidateProducer::endJob() {
   struct timeval tp2;
   gettimeofday(&tp2, NULL);
   long int endTime_ = tp2.tv_sec * 1000 + tp2.tv_usec / 1000;   
   double elapsed_milliseconds = endTime_ - startTime_;
   
   cout << endl << endl << endl << "Finished processing " << (eventSerialNumber_ - 1) << " events in " << elapsed_milliseconds / (60*1000) << " minutes!" << endl;
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


  cout << "This is beginJob()" << endl;

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

bool PFCandidateProducer::file_exists(const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

DEFINE_FWK_MODULE(PFCandidateProducer);
