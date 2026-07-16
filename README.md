# PrimesSRP


# How to Build:
```bash
cd ~/PrimesSRP
rm -rf build
mkdir build
cd build
cmake ..
make
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
```

Then do 
```bash
./main <x> <n> <threads> <toggles>
```
with the experiment you want to run.

For example:
```bash
./main 1000 25 4 --PrimePowers
```
will run the experiment on all primes up to 1000, and it will be run on all n between 2 and 24.

It will use 4 threads, and will include prime powers as well as regular primes.
