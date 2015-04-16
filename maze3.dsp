# Microsoft Developer Studio Project File - Name="maze3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=maze3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "maze3.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "maze3.mak" CFG="maze3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "maze3 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "maze3 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "adamh_ltspr01"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "maze3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "z:\maze3\Release"
# PROP Intermediate_Dir "z:\maze3\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\templates\non-lt" /I "..\..\win" /I "..\..\ILT" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"y:\maze3\maze3.exe"

!ELSEIF  "$(CFG)" == "maze3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "z:\maze3\Debug"
# PROP Intermediate_Dir "z:\maze3\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\templates\non-lt" /I "..\..\win" /I "..\..\ILT" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=xilink6.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"y:\maze3\maze3D.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "maze3 - Win32 Release"
# Name "maze3 - Win32 Debug"
# Begin Group "Driver"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\win\bitmap.h
# End Source File
# Begin Source File

SOURCE=..\..\ILT\bitmap\bitmaploadsave.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ILT\bitmap\bitmaploadsave.h
# End Source File
# Begin Source File

SOURCE=..\..\win\bitmapwindows.cpp
# End Source File
# Begin Source File

SOURCE=..\..\win\bitmapwindows.h
# End Source File
# Begin Source File

SOURCE=..\..\ILT\lithtech_module.cpp
# End Source File
# Begin Source File

SOURCE=..\..\ILT\lithtech_module.h
# End Source File
# Begin Source File

SOURCE=..\..\ILT\lithtech_module_templates.h
# End Source File
# Begin Source File

SOURCE=..\..\ILT\lithtech_stdlib.h
# End Source File
# Begin Source File

SOURCE=..\..\ILT\lithtech_types.h
# End Source File
# Begin Source File

SOURCE=.\main_driver.cpp
# End Source File
# Begin Source File

SOURCE=.\maze_debug.cpp
# End Source File
# End Group
# Begin Group "Array Generators"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\compute_clipped_corner_valid_maze_subset.cpp
# End Source File
# End Group
# Begin Group "Maze"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\maze3.h
# End Source File
# Begin Source File

SOURCE=.\maze_query.cpp
# End Source File
# Begin Source File

SOURCE=.\maze_query.h
# End Source File
# Begin Source File

SOURCE=.\maze_tables.cpp
# End Source File
# Begin Source File

SOURCE=.\maze_tables.h
# End Source File
# Begin Source File

SOURCE=.\maze_unit.cpp
# End Source File
# Begin Source File

SOURCE=.\maze_unit.h
# End Source File
# Begin Source File

SOURCE=.\maze_util.cpp
# End Source File
# Begin Source File

SOURCE=.\maze_util.h
# End Source File
# End Group
# Begin Group "Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\random.cpp
# End Source File
# Begin Source File

SOURCE=.\random.h
# End Source File
# Begin Source File

SOURCE="..\..\templates\non-lt\template_pointer.h"
# End Source File
# End Group
# End Target
# End Project
