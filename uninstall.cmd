:: @echo off
:: Uninstallation script for the fedra-package
:: for Windows XP and Windows 2003 Server

set installdir=%CD%

::-------------------------------------
IF NOT DEFINED DM_ROOT SET DM_ROOT=%installdir%

IF NOT EXIST %installdir%\src\config\ProjectDef.mk (
 echo PROJECT_ROOT=%installdir%
 echo.
 echo BIN_DIR = $^(PROJECT_ROOT^)/bin
 echo LIB_DIR = $^(PROJECT_ROOT^)/lib
 echo INC_DIR = $^(PROJECT_ROOT^)/include
 echo.
) > %installdir%\src\config\ProjectDef.mk

 set configdir=%installdir%\src\config
 set ln=call win32\tools\ln.exe
 if not exist %configdir%\RootDef.mk         %ln% %configdir%\RootDef.windows.mk        %configdir%\RootDef.mk
 if not exist %configdir%\TargetsDef.mk      %ln% %configdir%\TargetsDef.windows.mk     %configdir%\TargetsDef.mk
::-------------------------------------

 set PROJECT_INC=%installdir%\include
 set PROJECT_SRC=%installdir%\src
 
 pushd %installdir%\src
 call makeall.cmd clean 
 del  makeall-report.txt
 popd

 if exist %installdir%\bin     rmdir /s/q %installdir%\bin
 if exist %installdir%\lib     rmdir /s/q %installdir%\lib
 if exist %installdir%\include rmdir /s/q %installdir%\include
 if exist %installdir%\macros  rmdir /s/q %installdir%\macros

 set configdir=%installdir%\src\config
 if exist %configdir%\RootDef.mk        del /q %configdir%\RootDef.mk
 if exist %configdir%\TargetsDef.mk     del /q %configdir%\TargetsDef.mk
 if exist %configdir%\ProjectDef.mk     del /q %configdir%\ProjectDef.mk
 
 if exist %installdir%\setup_new.cmd del /q %installdir%\setup_new.cmd
 if DEFINED DM_ROOT %installdir%\win32\tools\setenv.exe  -u   DM_ROOT -delete
 if DEFINED DM_ROOT %installdir%\win32\tools\pathman    /ru %%DM_ROOT%%\bin
 if DEFINED DM_ROOT %installdir%\win32\tools\pathman    /ru %%DM_ROOT%%\lib

 if exist %installdir%\src\vsvars32.bat del /q %installdir%\src\vsvars32.bat 


