#MIT Open Data Producer

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


```git clone https://github.com/tripatheea/MODProducer.git CMSOpenData/MODProducer```


* Go to the source directory:

```
    cd CMSOpenData/MODProducer
```
* Run the shell file `run_producers.sh`:

```
    ./run_producers.sh
```
which will produce the data files-  `CMS_JetSample.dat` and `CMS_MinBiasSample.dat`.

Enjoy!

##Utilities

```
cmsRun PFCandidateRun.py /media/sf_virtual_machine/CMS/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/0611BEF5-0E71-E011-913B-003048D3739A.root /media/sf_virtual_machine/CMS/eos/opendata/cms/Run2010B/Jet/MOD/Apr21ReReco-v1/0000/ /media/sf_virtual_machine/CMS/eos/opendata/cms/Run2010B/Jet/registry/0.txt 0
```

```
python map.py /media/sf_virtual_machine/CMS/AOD/0000/ /media/sf_virtual_machine/CMS/registry/0.txt
```

```
python download.py ../file_paths/Jet/remote/CMS_Run2010B_Jet_AOD_Apr21ReReco-v1_0000_file_index.txt  /media/sf_virtual_machine/CMS/AOD/
```
