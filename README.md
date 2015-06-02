#CMS Open Data Producer

This package selects events from the CMS open
data release. A custom DAT file containing the four-vector information, mass, PdgId and trigger information of the particles is produced.

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


```git clone https://github.com/tripatheea/CMSOpenDataProducer.git CMSOpenDataProducer/CMSOpenDataProducer```


* Go to the source directory:

```
    cd CMSOpenDataProducer/CMSOpenDataProducer
```
* Run the shell file `run_producers.sh`:

```
    ./run_producers.sh
```
which will produce the data files-  `pfcandidate.dat`, `minBias.dat`, `ak5pfjets.dat` and `ak7pfjets.dat`.

Enjoy!
