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

`python ./utilities/download.py ./file_paths/Jet/small_list.txt ~/opendata/`

- Once you've downloaded the ROOT files, we need to create what's called a "registry". This is not actually required to process AOD data but we maintain the same file and directory structure while writing out MOD files and so, we need to create a map between event and run number, and the corresponding ROOT file. The registry creator is in fact just an EDProducer that we run N times for N files, each time simply recording which events and runs are there in a certain filename in a human readable format. Because this is an EDProducer, we need to initiazlie CMSSW environment variables before running with `cmsenv`. We create the registry using the Python script `create_registry.py`. This script takes two arguments: a path to the ROOT files that we want to process, and a path to the file that we want to write the registry information to.

`cmsenv`

`python ./create_registry.py ~/opendata/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/ ~/opendata/registry.txt`

- Now that we have created a registry for all the AOD files that we want to process, we are ready to run another EDProducer called PFCandidateProducer to convert all the AOD files to MOD (MIT Open Data). We start PFCandidateProducer with the Python file `PFCandidateRun.py`. This Python file takes two arguments: input directory (path to directory which contains all the AOD files) and, registry file path (path, including the filename, to the registry file). The "download" step above maintains the directory structure from CMS, which includes a directory named "AOD". This step maintains the same directory structure except AOD replaced with MOD. That's why you don't need to enter an output directory. Note that to get trigger prescales PFCandidateProducer needs to load globaltags because of which it takes a long time before anything happens. So be patient:

`cmsRun PFCandidateRun.py ~/opendata/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/ ~/opendata/registry.txt`

- Congratulations! You've successfully converted AOD files to MOD files. You can analyze these MOD files using the repository [MODAnalyzer](https://github.com/tripatheea/MODAnalyzer/ "MODAnalyzer"). 