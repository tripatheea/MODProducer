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
cmsRun PFCandidateRun.py /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/1C27B39E-7171-E011-AC3A-003048D436CA.root /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/MOD/Apr21ReReco-v1/ /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/registry/Apr21ReReco-v1/0.txt 0
```
```
cmsRun PFCandidateRun.py /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/problem/ /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/MOD/Apr21ReReco-v1/problem/ /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/registry/Apr21ReReco-v1/0.txt 1
```
```
python map.py /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/ /media/sf_opendata/eos/opendata/cms/Run2010B/Jet/registry/Apr21ReReco-v1/0.txt
```

```
python download.py ../file_paths/Jet/remote/CMS_Run2010B_Jet_AOD_Apr21ReReco-v1_0000_file_index.txt  /media/sf_virtual_machine/CMS/
```


## Workflow


- The first step is to download ROOT files from the CMS server. We do this using the Python script `utilities/download.py`. This script takes two arguments; path to a file which contains a list of link to files to download (one link per line) and, a destination path to write the files to. For a sample file that contains links to file, see `./file_paths/Jet/small_list.txt`. Note that these files each are ~1 GB so unless you have a lot of storage available, use a file that contains only a handful of files to download. 

`python ./utilities/download.py ./file_paths/Jet/small_list.txt ~/root_files`

- 