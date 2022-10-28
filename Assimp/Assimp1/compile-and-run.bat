if not exist build/ (mkdir build)

pushd build
	call emcmake cmake ..
	call cmake --build .
popd

pushd binaries
	call emrun index.html --kill_exit
	cd ..
pushd
