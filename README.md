Kinect Motion Capture
=====================
Research project at University Lille 1 about motion capture (mocap) by using the Microsoft Kinect.

Requirements
------------
* [Microsoft Kinect for Windows SDK 1.6](http://www.microsoft.com/en-us/kinectforwindows/develop/developer-downloads.aspx)
* [Qt 5.0.1](http://releases.qt-project.org/qt5/5.0.1/qt-windows-opensource-5.0.1-msvc2010_32-x86-offline.exe)
* [Visual Studio Add-in 1.2.0 for Qt5](http://releases.qt-project.org/vsaddin/qt-vs-addin-1.2.0-opensource.exe)

Development
-----------
All the steps required to compile and execute the project:
1. Download and install all the requirements. Don't forget to configure the Qt version for Visual Studio 2010.
2. Select your Qt version in the Qt Project Settings.
3. Still in the Qt Project Settings, check: Core, GUI and Widgets.
After that if should work, however if the build is not successfull and ask for a file called "Qt5Cored5.lib", you have to go to the Visual Studio 2010 properties of the project then in the Linker, look at the Input and edit the Additionnal Dependencies field to remove all files like "Qt5Cored5.lib". It's a bug with the lastest Qt Add-In for Visual Studio 2010.

Authors
-------
* Derek Hendrickx
* Benjamin Makusa

Supervisor
----------
* Hazem Wannous
