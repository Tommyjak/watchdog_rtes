#/bin/bash

echo "compiling..."
rm -rf build && mkdir build && cd build
cmake ..

make -j$(nproc)

cd ..

echo "done!"