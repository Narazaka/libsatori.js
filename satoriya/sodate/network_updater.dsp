# Microsoft Developer Studio Project File - Name="network_updater" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** �ҏW���Ȃ��ł������� **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=network_updater - Win32 Debug
!MESSAGE ����͗L����Ҳ�̧�قł͂���܂���B ������ۼު�Ă�����ނ��邽�߂ɂ� NMAKE ���g�p���Ă��������B
!MESSAGE [Ҳ�̧�ق̴���߰�] ����ނ��g�p���Ď��s���Ă�������
!MESSAGE 
!MESSAGE NMAKE /f "network_updater.mak".
!MESSAGE 
!MESSAGE NMAKE �̎��s���ɍ\�����w��ł��܂�
!MESSAGE ����� ײݏ��ϸۂ̐ݒ���`���܂��B��:
!MESSAGE 
!MESSAGE NMAKE /f "network_updater.mak" CFG="network_updater - Win32 Debug"
!MESSAGE 
!MESSAGE �I���\������� Ӱ��:
!MESSAGE 
!MESSAGE "network_updater - Win32 Release" ("Win32 (x86) Application" �p)
!MESSAGE "network_updater - Win32 Debug" ("Win32 (x86) Application" �p)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "network_updater - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /Oy- /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_ASM" /D "USE_ZIPMAIN" /YX /FD /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /subsystem:windows /machine:I386 /out:"Release/sodate.exe"

!ELSEIF  "$(CFG)" == "network_updater - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "NO_ASM" /D "USE_ZIPMAIN" /YX /FD /GZ /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wininet.lib /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/sodate.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "network_updater - Win32 Release"
# Name "network_updater - Win32 Debug"
# Begin Group "_"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\_\File.cpp
# End Source File
# Begin Source File

SOURCE=..\_\File.h
# End Source File
# Begin Source File

SOURCE=..\_\FileLister.cpp
# End Source File
# Begin Source File

SOURCE=..\_\FileLister.h
# End Source File
# Begin Source File

SOURCE=..\_\FTP.cpp
# End Source File
# Begin Source File

SOURCE=..\_\FTP.h
# End Source File
# Begin Source File

SOURCE=..\_\MD5.cpp
# End Source File
# Begin Source File

SOURCE=..\_\MD5.h
# End Source File
# Begin Source File

SOURCE=..\_\stltool.cpp
# End Source File
# Begin Source File

SOURCE=..\_\stltool.h
# End Source File
# Begin Source File

SOURCE=..\_\Utilities.cpp
# End Source File
# Begin Source File

SOURCE=..\_\Utilities.h
# End Source File
# End Group
# Begin Group "zip"

# PROP Default_Filter ""
# Begin Group "win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\infozip\win32\nt.c
# End Source File
# Begin Source File

SOURCE=..\infozip\win32\win32.c
# End Source File
# Begin Source File

SOURCE=..\infozip\win32\win32zip.c
# End Source File
# End Group
# Begin Source File

SOURCE=..\infozip\crc32.c
# End Source File
# Begin Source File

SOURCE=..\infozip\crctab.c
# End Source File
# Begin Source File

SOURCE=..\infozip\crypt.c
# End Source File
# Begin Source File

SOURCE=..\infozip\deflate.c
# End Source File
# Begin Source File

SOURCE=..\infozip\fileio.c
# End Source File
# Begin Source File

SOURCE=..\infozip\globals.c
# End Source File
# Begin Source File

SOURCE=..\infozip\trees.c
# End Source File
# Begin Source File

SOURCE=..\infozip\ttyio.c
# End Source File
# Begin Source File

SOURCE=..\infozip\util.c
# End Source File
# Begin Source File

SOURCE=..\infozip\zip.c
# End Source File
# Begin Source File

SOURCE=..\infozip\zipfile.c
# End Source File
# Begin Source File

SOURCE=..\infozip\zipup.c
# End Source File
# End Group
# Begin Source File

SOURCE=.\backup.h
# End Source File
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\main.ico
# End Source File
# Begin Source File

SOURCE=.\makeZip.h
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# Begin Source File

SOURCE=.\Sender.cpp
# End Source File
# Begin Source File

SOURCE=.\Sender.h
# End Source File
# Begin Source File

SOURCE=.\uploadFiles.cpp
# End Source File
# End Target
# End Project
