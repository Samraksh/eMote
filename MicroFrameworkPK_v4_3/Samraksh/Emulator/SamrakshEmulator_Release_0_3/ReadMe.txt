Samraksh's Micro Framework Emulator Release 0.3 Notes
====================================================

Current Version 0.3: Release date Nov 15, 2012
Version 0.2: Release date June 20, 2012
version 0.1: Release date May 20, 2012

Directory Structure:
--------------------
bin - Contains all the binary dlls 
    - includes all dlls needed to run the emulator without installing the porting kit
    - includes a binary of the ADAPT emulator and test applications in the tutorial
    - The *.bat in the bin directory are windows batch files used to run different MF applications in the emulator from command mode

DemoVideo - Contains a video of the demo of the two example applications

Documenation - contains a manual detailing the design of the emulator and a tutorial which helps the users through the two example applications

SourceCode - contains the complete source code.
    - ADAPTEmulator: Has the source for the emulator
		- SamrakshLibrary: Has the source code for MF APIs written by Samraksh for Emulator. For example, 
		  the ADC api.
    - MFApplications: Has the soruce for the two test applications in the tutorial
    - PhysicalModels: Has the source code for the Open Loop Car Model
    - PhyscialModelCommunicationLibrary: Has the code for the PhysicalModelEmulatorComm class written by Samraksh 
	to make the interaction between the model and emulator easy.


Change Log: 
-------------
Release 0.3
- Moved the emulator from MF 4.0 to MF 4.3
Release 0.2
- ADC support added.
Release 0.1
- Logging features added
- Bug fix to make emulator quit properly while starting from command line
- Added more binary libraries to run applications using serial port from command line
- Emulator will continue run (instead of hanging) if physical model does not connect, but will display warning
Release 0.0 Feature
- Support for GPIO input automation
- Support for Physical Models (through serial (only) communication) 
- Timers, GPIO, Interrupt, Serial


Future Features:
------------------
Release 1.0:
- Radio, Network Emulation
- SPI, I2C


Support:
Email support@samraksh.com with the subject "Emulator:"