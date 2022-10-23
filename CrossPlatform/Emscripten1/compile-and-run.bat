if not exist out/ (mkdir out)

emcc main.cpp -o out/index.html && emrun out/index.html