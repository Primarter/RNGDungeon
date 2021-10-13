mkdir ./build
cd ./build && cmake ../ -G "Unix Makefiles" && cmake --build . && mv $1 ../.. && cd ../..
