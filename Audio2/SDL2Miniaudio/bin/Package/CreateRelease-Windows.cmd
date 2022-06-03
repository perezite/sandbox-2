@echo off

set Version=1.0.0
set Name=o2-blocks
set Platform=windows
set Identifier=%Name%-%Version%-%Platform%

mkdir %Identifier%

xcopy /E /I ..\Windows %Identifier%\Windows
xcopy /E /I ..\Assets %Identifier%\Assets

del %Identifier%\Windows\*.ilk
del %Identifier%\Windows\*.pdb
del %Identifier%\Windows\*.iobj
del %Identifier%\Windows\*.ipdb

call 7za.exe a -r %Identifier%.zip %Identifier%

rmdir /S /Q %Identifier%

pause