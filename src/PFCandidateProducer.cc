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

#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequenceAreaBase.hh>
#include <fastjet/RangeDefinition.hh>


using namespace std;
using namespace edm;
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

   InputTag AK5PFInputTag_;
   InputTag rhoTag_;
   InputTag PFCandidateInputTag_;
};

PFCandidateProducer::PFCandidateProducer(const ParameterSet& iConfig) {
  //correctorLabel_ = iConfig.getParameter<InputTag>("JetCorrector");
  AK5PFInputTag_ = iConfig.getParameter<edm::InputTag>("JetCollectionName");
  rhoTag_ = iConfig.getParameter<edm::InputTag>("rho");
  PFCandidateInputTag_ = iConfig.getParameter<InputTag>("PFCandidateInputTag");
}


PFCandidateProducer::~PFCandidateProducer() {

}

void PFCandidateProducer::produce(Event& iEvent, const EventSetup& iSetup) {
   

   
   

   // Get PFCandidates first.
   Handle<reco::PFCandidateCollection> PFCollection;
   iEvent.getByLabel(PFCandidateInputTag_, PFCollection);
   
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
   
   /*
   JetMedianBackgroundEstimator jetMedian = JetMedianBackgroundEstimator();
   jetMedian.setParticles(PFCForFastJet);
   jetMedian.set_cluster_sequence(clust_seq);
   
   double rho = jetMedian.rho();
   */
   
   

   
   // Create the JetCorrectorParameter objects, the order does not matter.
   // YYYY is the first part of the txt files: usually the global tag from which they are retrieved
   JetCorrectorParameters *ResJetPar = new JetCorrectorParameters("GR_R_41_V0_AK5PF_L2L3Residual.txt"); 
   JetCorrectorParameters *L3JetPar  = new JetCorrectorParameters("GR_R_41_V0_AK5PF_L3Absolute.txt");
   JetCorrectorParameters *L2JetPar  = new JetCorrectorParameters("GR_R_41_V0_AK5PF_L2Relative.txt");
   JetCorrectorParameters *L1JetPar  = new JetCorrectorParameters("GR_R_41_V0_AK5PF_L1FastJet.txt");
   //  Load the JetCorrectorParameter objects into a vector, IMPORTANT: THE ORDER MATTERS HERE !!!! 
   vector<JetCorrectorParameters> vPar;
   vPar.push_back(*L1JetPar);
   vPar.push_back(*L2JetPar);
   vPar.push_back(*L3JetPar);
   vPar.push_back(*ResJetPar);
   
   FactorizedJetCorrector *JetCorrector = new FactorizedJetCorrector(vPar);
  
   edm::Handle<reco::PFJetCollection> AK5Collection;
   iEvent.getByLabel(AK5PFInputTag_, AK5Collection);

   	
  
  for(reco::PFJetCollection::const_iterator it = AK5Collection->begin(), end = AK5Collection->end(); it != end; it++) {
    
    JetCorrector->setJetEta(it->eta());
    JetCorrector->setJetPt(it->pt());
    JetCorrector->setJetA(it->jetArea());
    JetCorrector->setRho(rho);

    
    double correction = JetCorrector->getCorrection();
    cout << correction << endl;
  }
  

  
}

void PFCandidateProducer::beginJob() {

}

void PFCandidateProducer::endJob() {

}

void PFCandidateProducer::beginRun(edm::Run & iRun, edm::EventSetup const & iSetup){

}

void PFCandidateProducer::endRun(edm::Run&, edm::EventSetup const&) {

}

void PFCandidateProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {

}

void PFCandidateProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&) {

}

DEFINE_FWK_MODULE(PFCandidateProducer);
