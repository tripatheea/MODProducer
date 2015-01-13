#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"


// The following four lines are for Muons. Remove them later on. 
// Not too sure about the last two lines though.
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include <iostream>
#include <string>

#include <TH1.h>
#include <TFile.h>

#include <fstream>

#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/JetReco/interface/PFJet.h"

#include "PhysicsTools/SelectorUtils/interface/PFJetIDSelectionFunctor.h"
#include "PhysicsTools/SelectorUtils/interface/strbitset.h"


class PFCandidateFilter : public edm::EDFilter 
{
public: 
  explicit PFCandidateFilter(const edm::ParameterSet&);
  ~PFCandidateFilter();

private:
  virtual void beginJob() ;
  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
 
  edm::InputTag muonInputTag_;

  double binInterval_;
  std::string rootFileName_;
  double invariantMassMin_;
  double invariantMassMax_;
 
  TFile* rootFile_;
  TH1D*  invariantMasses_;

  std::ofstream csvOut_;
  std::string csvFileName_;

  int maxNEvents_;
  int nEvents_;
};

PFCandidateFilter::PFCandidateFilter(const edm::ParameterSet& iConfig)
  : muonInputTag_(iConfig.getParameter<edm::InputTag>("muonInputTag")),
    binInterval_(iConfig.getParameter<double>("binInterval")),
    rootFileName_(iConfig.getParameter<std::string>("rootFileName")),
    invariantMassMin_(iConfig.getParameter<double>("invariantMassMin")),
    invariantMassMax_(iConfig.getParameter<double>("invariantMassMax")),
    csvFileName_(iConfig.getParameter<std::string>("csvFileName")),
    maxNEvents_(iConfig.getParameter<int>("maxNEvents")),
    nEvents_(0)
{
  //rootFile_ = new TFile(rootFileName_.c_str(), "RECREATE");

  int nbins = int((invariantMassMax_ - invariantMassMin_)/binInterval_);
  std::cout << "Analyzing" << std::endl; 
  
  invariantMasses_ = new TH1D("dimuon invariant mass",
                              "dimuon invariant mass",
                              nbins, invariantMassMin_, invariantMassMax_);

  csvOut_.open(csvFileName_.c_str());
}


PFCandidateFilter::~PFCandidateFilter()
{}

bool
PFCandidateFilter::filter(edm::Event& event, const edm::EventSetup& eventSetup)
{

  std::cout << "Started filtering. Ting." << std::endl;

  edm::Handle<reco::PFCandidateCollection> collection;


  std::cout << muonInputTag_ << std::endl;
  
  event.getByLabel(muonInputTag_, collection);


  std::cout << "The collection size is: " << collection->size() << std::endl;

  if ( ! collection.isValid() )
  {
    std::cerr<<"DimuonFilter: invalid collection"<<std::endl;
    return false;
  }
  
  std::cout << "Yay, it worked!" << std::endl;
  
  
  return true;
}

void 
PFCandidateFilter::beginJob()
{
  csvOut_<<"Run,Event,Type1,E1,px1,py1,pz1,pt1,eta1,phi1,Q1,Type2,E2,px2,py2,pz2,pt2,eta2,phi2,Q2,M"<<std::endl;
}

void 
PFCandidateFilter::endJob() 
{
  //rootFile_->cd();
  //invariantMasses_->Write();
  //rootFile_->Close();

  csvOut_.close();
}

DEFINE_FWK_MODULE(PFCandidateFilter);
