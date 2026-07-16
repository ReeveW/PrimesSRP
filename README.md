# PrimesSRP


# How to Build:
```bash
cd ~/PrimesSRP
rm -rf build
mkdir build
cd build
cmake ..
make -j4
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
./main <x> <n> <threads> <toggles>
```
