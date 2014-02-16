@echo off
REM -- First make map file from Microsoft Visual C++ generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by SCM.HPJ. >"hlp\SCM.hm"
echo. >>"hlp\SCM.hm"
echo // Commands (ID_* and IDM_*) >>"hlp\SCM.hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>"hlp\SCM.hm"
echo. >>"hlp\SCM.hm"
echo // Prompts (IDP_*) >>"hlp\SCM.hm"
makehm IDP_,HIDP_,0x30000 resource.h >>"hlp\SCM.hm"
echo. >>"hlp\SCM.hm"
echo // Resources (IDR_*) >>"hlp\SCM.hm"
makehm IDR_,HIDR_,0x20000 resource.h >>"hlp\SCM.hm"
echo. >>"hlp\SCM.hm"
echo // Dialogs (IDD_*) >>"hlp\SCM.hm"
makehm IDD_,HIDD_,0x20000 resource.h >>"hlp\SCM.hm"
echo. >>"hlp\SCM.hm"
echo // Frame Controls (IDW_*) >>"hlp\SCM.hm"
makehm IDW_,HIDW_,0x50000 resource.h >>"hlp\SCM.hm"
REM -- Make help for Project SCM


echo Building Win32 Help files
start /wait hcw /C /E /M "hlp\SCM.hpj"
if errorlevel 1 goto :Error
if not exist "hlp\SCM.hlp" goto :Error
if not exist "hlp\SCM.cnt" goto :Error
echo.
if exist Debug\nul copy "hlp\SCM.hlp" Debug
if exist Debug\nul copy "hlp\SCM.cnt" Debug
if exist Release\nul copy "hlp\SCM.hlp" Release
if exist Release\nul copy "hlp\SCM.cnt" Release
echo.
goto :done

:Error
echo hlp\SCM.hpj(1) : error: Problem encountered creating help file

:done
echo.
