README file for the CrowdSight-SDK example for Visual Studio
============================================================

These instructions are for vs2010 build of CrowdSight:

Installation:

* Install the CrowdSight SDK
* Download and install Visual Studio 2010 C++ Express edition
* Run Visual Studio C++ Express as administrator
* Open the example.sln file in Visual Studio
* Right click the project and select 'Properties'
* In the 'General->Debugging' settings, fill in command line 
  arguments according to following syntax:

    <videofile> <data dir> <resources dir> <auth key>
  
  or for webcam input:

    --capture <frame-id> <data dir> <resources dir> <auth key>

* Compile and run the example using the play button

Command line arguments specification:

  Using a video as input:
  <videofile>     A video file
  <data dir>      Default location is ../data/
  <resources dir> Default location is ../resources/
  <auth key>      Your crowdsight license key

  Using a webcam as input:
  <frame-id>      ID of the webcam to be used, most of the time this is 0 
  <data dir>      Default location is ../data/
  <resources dir> Default location is ../resources/
  <auth key>      Your crowdsight license key

   Example arguments: 

   testmovie.avi ../data/ ../resources/ mycrowdsightlicensekey
   --capture 0 ../data/ ../resources/ mycrowdsightlicensekey


Support email:

If you have any questions, please contact us at

support@sightcorp.com
