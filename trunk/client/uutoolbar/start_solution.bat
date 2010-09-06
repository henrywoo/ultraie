@echo off
rem -- This .bat file is used to start the solution
rem -- These variables can be changed as you need

set VIRTUALDIRVE=S:
set REALPATH=I:\6beenetworks\client

@echo Create the virtual dirve ...

if exist %VIRTUALDIRVE% ( subst %VIRTUALDIRVE% /D )
if not exist %VIRTUALDIRVE% ( subst %VIRTUALDIRVE% %REALPATH% )

@echo -- Success!
@echo.
@echo Set develop environment variables ...

set SOLUTIONNAME=uutoolbar
set ROOT=%VIRTUALDIRVE%\%SOLUTIONNAME%
set UUBIN=%ROOT%\bin
set UUCONFIG=%ROOT%\config
set UULIB=%ROOT%\lib\lib
set UUINC=%ROOT%\lib\inc
set UUSOURCE=%ROOT%\src
set UUTEMP=%ROOT%\temp
set UUOUTPUT=%UUTEMP%\output
set INTERMEDIATE=%UUTEMP%\intermediate
set VENDOR=%ROOT%\vendor
rem -- for debug version, we want to debug it in development machine, so here are somewhat difference to release version.
set UUOUTPUT_DEBUG=%UUBIN%\debug

@echo Third party library

set WTL_ROOT=%VENDOR%\wtl80
set ICE_ROOT=%VENDOR%\ice-3.3.0
set CPPUNIT_ROOT=%VENDOR%\cppunit-1.12.1
set SQLITE_ROOT=%VENDOR%\sqlite3
set MM_ROOT=%VENDOR%\dxsdk
set MP3_ROOT=%VENDOR%\lame-3.97
set FFMPEG_ROOT=%VENDOR%\ffmpeg
set PCRECPP_ROOT=%VENDOR%\pcrecpp

rem -- VC2005 setting
SET VSINSTALLDIR=C:\Program Files\Microsoft Visual Studio 8
SET VCINSTALLDIR=C:\Program Files\Microsoft Visual Studio 8\VC
SET FrameworkDir=C:\WINDOWS\Microsoft.NET\Framework
SET FrameworkVersion=v2.0.50727
SET FrameworkSDKDir=C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0
SET VC2005PlatformSDKDir=C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK
SET PlatformSDKDir=C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK
if "%VSINSTALLDIR%"=="" goto error_no_VSINSTALLDIR
if "%VCINSTALLDIR%"=="" goto error_no_VCINSTALLDIR

@echo Setting environment for using Microsoft Visual Studio 2005 x86 tools.

rem -- Root of Visual Studio IDE installed files.

set DevEnvDir=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE
set PATH=C:\Program Files\Microsoft Visual Studio 8\Common7\IDE;C:\Program Files\Microsoft Visual Studio 8\VC\BIN;C:\Program Files\Microsoft Visual Studio 8\Common7\Tools;C:\Program Files\Microsoft Visual Studio 8\Common7\Tools\bin;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\bin;C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\bin;C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;C:\Program Files\Microsoft Visual Studio 8\VC\VCPackages;%PATH%
set INCLUDE=C:\Program Files\Microsoft Visual Studio 8\VC\ATLMFC\INCLUDE;C:\Program Files\Microsoft Visual Studio 8\VC\INCLUDE;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\include;C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\include;%INCLUDE%
set LIB=C:\Program Files\Microsoft Visual Studio 8\VC\ATLMFC\LIB;C:\Program Files\Microsoft Visual Studio 8\VC\LIB;C:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\lib;C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\lib;%LIB%
set LIBPATH=C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727;C:\Program Files\Microsoft Visual Studio 8\VC\ATLMFC\LIB

@echo -- Success!
@echo -- Set DLL path for development(debug version)

set PATH=%UUBIN%\debug;%ICE_ROOT%\bin;%MP3_ROOT%\bin;%FFMPEG_ROOT%\bin;%SQLITE_ROOT%\bin;%PCRECPP_ROOT%\bin;%PATH%
rem -- This is for release version:
rem -- set PATH=%UUBIN%\release;%ICE_ROOT%\bin;%MP3_ROOT%\bin;%FFMPEG_ROOT%\bin;%SQLITE_ROOT%\bin;%PATH%
@echo %PATH%


@echo -- Success!
@echo.
@echo Open solution in VS.Net IDE ...

if not exist %UUSOURCE%\%SOLUTIONNAME%.sln goto ERROR_NOT_FOUND_SOLUTION_FILE
devenv %UUSOURCE%\%SOLUTIONNAME%.sln

@echo -- Success!

goto END

:ERROR_NOT_FOUND_SOLUTION_FILE
@echo ERROR: Not found the solution file!
goto END

:END
