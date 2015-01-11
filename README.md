This package is a filter that selects events from the Mu primary dataset from the CMS open
data release. In particular, the event is selected if (for simplicity) there are precisely two muons
in the event and at least one is a global muon. A csv file containing the four-vector information, charge, and invariant mass of the two muons is produced.

See http://opendata.cern.ch for more information and for context on the instructions below.

To produce files in the VM open a terminal with the X terminal emulator (an icon bottom-left of the VM screen)
and input the commands as explained below.

* Create a CMSSW environment: 

```
    cmsrel CMSSW_4_2_8
```

* Change to the CMSSW_4_2_8/src/ directory:

```
    cd CMSSW_4_2_8/src/
```
* Initialize the CMSSW environment:

```
    cmsenv
```
* Clone the source code:

```
    git clone https://github.com/tripatheea/PFCandidate-Filter PFCandidateFilter/PFCandidateFilter
````
* Compile the code with the command:

```
    scram b
```
* Go to the source directory:

```
    cd PFCandidate/PFCandidate
```
* Run the example configuration file (see comments in the file on changing parameters):

```
    cmsRun PFCandidateRun.py
```
which will produce a csv file: PFCandidate.csv

Enjoy!
