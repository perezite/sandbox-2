Compile and run a simple Cmake based Emscripten project

# Prerequisites (Windows)
- mingw32-make must be installed (because emcmake uses this)

# Compile and run (Windows)
- Open an emscripten shell using open-emscripten-shell.bat. This also installs the Emscripten SDK if needed.
	- Note: to uninstall the Emscripten SDK, call uninstall-emscripten.bat
- From inside the Emscripten Shell, run: compile-and-run.bat

# Links
- https://emscripten.org/docs/tools_reference/emcc.html#emcc-emrun
- https://stackoverflow.com/questions/50959871/generate-web-assembly-from-cmake-project-with-a-specified-html-output
- https://emscripten.org/docs/compiling/Building-Projects.html
- https://marcelbraghetto.github.io/a-simple-triangle/2019/03/10/part-06/