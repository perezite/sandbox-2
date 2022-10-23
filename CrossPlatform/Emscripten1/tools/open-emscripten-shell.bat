rem source: https://emscripten.org/docs/getting_started/downloads.html#platform-notes-installation-instructions-sdk

@echo off

rem clone and install emscripten
if not exist emsdk\ (
	git clone https://github.com/emscripten-core/emsdk.git
)
cd emsdk
git pull
call emsdk install latest
call emsdk activate latest

rem open the emscripten shell
cd ../..
call tools/emsdk/emcmdprompt.bat