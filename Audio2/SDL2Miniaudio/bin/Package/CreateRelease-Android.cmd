@echo off

set Version=1.0.0
set Name=o2-blocks
set Platform=android
set Identifier=%Name%-%Version%-%Platform%

copy "..\Android\Application.apk" %Identifier%.apk

pause