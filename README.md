Kinect Motion Capture
=====================
Research project at University Lille 1 about motion capture (mocap) by using the Microsoft Kinect.

Requirements
------------
* [Microsoft Kinect for Windows SDK 1.7](http://www.microsoft.com/en-us/kinectforwindows/develop/developer-downloads.aspx)
* [Qt 5.0.1](http://releases.qt-project.org/qt5/5.0.1/qt-windows-opensource-5.0.1-msvc2010_32-x86-offline.exe)
* [Visual Studio Add-in 1.2.0 for Qt5](http://releases.qt-project.org/vsaddin/qt-vs-addin-1.2.0-opensource.exe)
* [Eigen 3.1.3](http://bitbucket.org/eigen/eigen/get/3.1.3.tar.bz2)

Development
-----------
All the steps required to compile and execute the project:

1. Download and install all the requirements. Don't forget to configure the Qt version for Visual Studio 2010.
2. Select your Qt version in the Qt Project Settings.
3. Still in the Qt Project Settings, check: Core, GUI and Widgets.
4. Follow the instructions on http://eigen.tuxfamily.org/index.php?title=Visual_Studio to use the Eigen Math Library with Visual Studio 2010.

After that if should work, however if the build is not successfull and ask for a file called "Qt5Cored5.lib", you have to go to the Visual Studio 2010 properties of the project then in the Linker, look at the Input and edit the Additionnal Dependencies field to remove all files like "Qt5Cored5.lib". It's a bug with the lastest Qt Add-In for Visual Studio 2010.

> When yout create a new Qt5 project the VS-addin puts incorrect names for the debug .libs, adding a "5" at the end.
> For example, it adds a dependency on Qt5Guid5.lib where it should be Qt5Guid.lib
>
> Debug (win32): qtmaind.lib;Qt5Cored.lib;Qt5Guid.lib;Qt5Widgetsd.lib;%(AdditionalDependencies)
>
> Release (win32): qtmain.lib;Qt5Core.lib;Qt5Gui.lib;Qt5Widgets.lib;%(AdditionalDependencies)

Authors
-------
* Derek Hendrickx
* Benjamin Makusa

Supervisor
----------
* Hazem Wannous
