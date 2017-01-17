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




using namespace std;
using namespace edm;



class filenameMapProducer : public EDProducer 
{
public: 
   explicit filenameMapProducer(const ParameterSet&);
   ~filenameMapProducer();

private:
   virtual void beginJob();
   virtual void produce(edm::Event&, const edm::EventSetup&);
   virtual void endJob() ;
   virtual void beginRun(edm::Run&, edm::EventSetup const&);
   virtual void endRun(edm::Run&, edm::EventSetup const&);
   virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
   virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

   ofstream fileOutput_;
   string currentProcessingFilename_;
   string outputFilename_;

};



filenameMapProducer::filenameMapProducer(const ParameterSet& iConfig)
: currentProcessingFilename_(iConfig.getParameter<string>("filename")),
  outputFilename_(iConfig.getParameter<string>("outputFile"))
{
  fileOutput_.open(outputFilename_.c_str(), std::fstream::out | std::fstream::app);
}


filenameMapProducer::~filenameMapProducer() {

}

void filenameMapProducer::produce(Event& iEvent, const EventSetup& iSetup) {
   
   int runNum = iEvent.id().run();
   int eventNum = iEvent.id().event();
   
   fileOutput_ << eventNum << " " << runNum << " " << currentProcessingFilename_ << endl;
}

void filenameMapProducer::beginJob() {

}

void filenameMapProducer::endJob() {
   fileOutput_.close();
}

void filenameMapProducer::beginRun(edm::Run & iRun, edm::EventSetup const & iSetup){

}

void filenameMapProducer::endRun(edm::Run&, edm::EventSetup const&) {

}

void filenameMapProducer::beginLuminosityBlock(edm::LuminosityBlock& iLumi, edm::EventSetup const& iSetup) {

}

void filenameMapProducer::endLuminosityBlock(edm::LuminosityBlock& iLumi, edm::EventSetup const& iSetup) {

}


DEFINE_FWK_MODULE(filenameMapProducer);
