# MIT Open Data Producer

This package downloads AOD files from the [CERN Open Data Portal](http://opendata.cern.ch "CERN Open Data Portal") release and converts them into a human-readable file format called MOD (MIT Open Data). Currently, the following information are stored:
	
- 4-momentum and pdgId for PF Candidates.
- 4-momentum, Jet Area and Jet Energy Correction factors for AK5 Jets. These are stored for redundancy and validation of the jets clustered from the stored PF Candidates.
- Trigger Names, prescale values and whether or not that trigger fired for each event. 
- Run Number and Event Number.
- Luminosity Block, Average Instantaneous Luminosity and Number of Primary Vertices.


## Usage Instruction

You're supposed to follow the following steps inside a VM you've created as instructed [here](http://opendata.cern.ch/VM/CMS "CERN Open Data Portal"). 

### Preparation

- Create a CMSSW environment: 

    ```
    cmsrel CMSSW_4_2_8
    ```

- Change to the CMSSW_4_2_8/src/ directory:

    ```
    cd CMSSW_4_2_8/src/
    ```

- Initialize the CMSSW environment:

  ```
  cmsenv
  ```
- Clone the source code:

  ```
  git clone https://github.com/tripatheea/MODProducer.git CMSOpenData/MODProducer
  ```
- Go to the source directory:

  ```
  cd CMSOpenData/MODProducer
  ```
- Compile everything:

  ```
  scram b
  ```


### Workflow

We adopt the following workflow for extracting MOD files out of the given AOD files.

1.  Download all the ROOT files and arrange them in the same directory structure as they live on the CMS server.

2. Create a registry that maps each event and run number to a certain ROOT file. This is done so that things can be done one file at a time as we're dealing with multiple TeraBytes of data here and it's a pain to have to do everything at once. 

3. Run the Producer on those AOD files. This reads the download directory and processes only the files in there. This produces N MOD files. 

4. Filter those N MOD files to get only those files for which the correct trigger fired. This process is called Skimming in this workflow. This will produce M <= N MOD files. For a certain AOD file, if none of the events in there have the correct trigger fired, a corresponding skimmed MOD file will not be written. That's why M might be less than N.

5. Read in those "skimmed" M <= N output files one by one and calculate stuff to produce a single DAT file. 

6. Produce plots using the DAT file produced in step (5).

Note that this repository is concerned with steps (1) to (3) only. Steps (4) to (6) are carried out by the [MODAnalyzer](https://github.com/tripatheea/MODAnalyzer/ "MODAnalyzer") package.

### Workflow Instructions


- The first step is to download ROOT files from the CMS server. You can start the download process using the Python script `utilities/download.py`. This script takes two arguments:
	
    1. a path to a file which contains a list of links to files to download (one link per line) 
    2. a destination path to write the files to. For a sample file that contains links to file, see `file_paths/Jet/small_list.txt`. Note that the ROOT files are each ~1 GB, so unless you have a lot of storage available, make sure you don't download too many files. 

    ```
    python ./utilities/download.py ./file_paths/Jet/small_list.txt ~/MITOpenDataProject/
    ```
    The download script will skip any ROOT file that you have already downloaded and will resume any broken downloads. So you don't have to download all the files at once as long as you are downloading all of them to the same directory.

- Once you've downloaded the AOD files (these are ROOT files), you need to create what's called a "registry". A registry creates a map between event and run number, and the corresponding ROOT file. The registry creator is just an [EDProducer](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookEDMTutorialProducer "EDProducer") that you run N times for N files, each time simply recording which events and runs are there in a certain ROOT file, in a human readable format. Because this is an [EDProducer](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookEDMTutorialProducer "EDProducer"), you need to initiazlie CMSSW environment variables first with `cmsenv`. You then create the registry using the Python script `create_registry.py`. This script takes two arguments: 
	
    1. a path to the ROOT files that you want to process. Note that this is NOT the same as the second argument in the previous step. Because the code maintain the same directory structure as in the CMS servers, the download script creates sub-directories inside the directory that you specified in the previous step. So you need to manually go inside the directory you provided in the previous step, following the subdirectories until you find yourself inside a directory where the ROOT files reside. 
    2. a path to the registry file.

    ```
    cmsenv
    ```
    ```
    python ./create_registry.py ~/MITOpenDataProject/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/ ~/MITOpenDataProject/registry.txt
    ```

- Now that you have created a registry for all the AOD files that tou want to process, you are ready to run another [EDProducer](https://twiki.cern.ch/twiki/bin/view/CMSPublic/WorkBookEDMTutorialProducer "EDProducer") called PFCandidateProducer to convert them into MOD (MIT Open Data) files. You can run PFCandidateProducer with the Python script `PFCandidateRun.py`. This script takes three arguments: 

	1. input directory (path to the directory which contains all the AOD files). This is the same as the second argument that you supplied in the previous step.
	2. output directory (path to the directory where you'd like to store all the MOD files). If this directory is not already present, it will create the directory.
	3. path to the registry file, including the filename. 
	4. whether to process from the beginning or not (1 or 0). If set to 1, the Producer will start AOD->MOD conversion from the first file in the registry. However, because it's desirable to break this step into multiple instances, you can run the producer once, quit it and come back later to resume it. So if set to 0, the producer will skip the files already in the MOD output directory and resume from there. Note that, the smallest discrete interval that the producer can detect is one ROOT (or MOD) file. So if you interrupted the producer while it's running, make sure you remove that particular MOD file from the output directory because else, the producer will skip that the next time even though < 100% events of that file are done.

	As mentioned earlier, the "download" step above maintains the directory structure of CMS servers. This includes a directory named "AOD". 

	Note that to get trigger prescales, PFCandidateProducer needs to load GlobalTags and so, it takes a long time before anything happens (it takes ~10 minutes on my computer).
    
  ```
  cmsRun PFCandidateRun.py ~/MITOpenDataProject/eos/opendata/cms/Run2010B/Jet/AOD/Apr21ReReco-v1/0000/ ~/MITOpenDataProject/eos/opendata/cms/Run2010B/Jet/MOD/Apr21ReReco-v1/0000/ ~/MITOpenDataProject/registry.txt 1
  ```
  
- Congratulations! You've successfully converted all the AOD files that you downloaded to MOD files. In other words, you've completed steps (1) to (3) in the workflow given above. Heaad over to [MODAnalyzer](https://github.com/tripatheea/MODAnalyzer/ "MODAnalyzer") to see how you can analyze data in these MOD files to produce all sorts of super-interesting plots. 

### Notes about JEC
While this repository already contains the necessary files to calculate JEC factors inside the directory `data/JEC/`, if you want to regenerate them or need to use a global tag other than GR_R_42:V25, you can use the Python script `JEC_cfg.py`. The global tag can be edited on line 9 and 19. It might take a while (~20 minutes or more) for the script to complete. 

```
cmsRun JEC_cfg.py
```

If you're using a global tag other than GR_R_42:V25, the filenames will be different from what they are in the repository. For those cases, you need to edit the filenames in `src/PFCandidateProducer.cc`, under the method `PFCandidateProducer::beginJob()`, lines 389 to 392.

## Other Notes
Some random notes that might be helpful as you play around with the code here:

1. Do not forget to run `scram b` to compile your code any time you make a change to a C++ source file. 

2. It's a pain to wait ~10 mins every time you want to run PFCandidateProducer. So for debugging/hacking purposes, you might want to turn off loading the GlobalTags. You can do this by commenting lines 55 and 56 in `PFCandidateRun.py`. You also have to comment out lines 291-307 in `src/PFCandidateProducer.cc`. Those lines are responsible to write out trigger information so if you need to test stuff pretaining to triggers, you cannot get around waiting (technically, you need to load the GlobalTags for trigger prescales only).

3. If you use a new module/component inside `src/PFCandidateProducer.cc`, do not forget to include the corresponding module in the buildfile. Likewise, if you're not using a certain module, remove the corresponding module from the buildfile so that it doesn't slow down compilation of your code.  

4. If you're using something like an external hard drive to store the MOD files into, you'll have to use a "shared directory". Here's how to set it up:
   - Add whatever directory you'd like to use under Shared Folder in the VM settings.
   - Create a user group called `vboxsf` and add the user cms-opendata to that group with `sudo usermod -aG vboxsf cms-opendata`.
   - Restart your VM.

## Troubleshooting

    Standard library exception caught in cmsRun:
  
    Can not get data (Additional Information: [frontier.c:793]: No more servers/proxies. 
  
This error message generally means that you didn't use  an encrypted Internet connection. If you are on MIT campus that means you should be using either the MIT SECURE access point or an Ethernet cable.
