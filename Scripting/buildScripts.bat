@echo off
cl.exe /LD TestCPPScript.cpp /link /DLL /OUT:scripts/customScripts.dll
Rem IF %ERRORLEVEL% NEQ 0 (Echo An error was found &Exit /b 1)
Rem if error level != 0 then an error has occured.
Rem Writes to the log and exits.
pause