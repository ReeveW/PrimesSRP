# PrimesSRP


# How to Build On Eudoxus:

Run the following commands
```bash
cd ~/PrimesSRP
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
export LD_LIBRARY_PATH=/usr/local/lib64:$LD_LIBRARY_PATH
```

Then do 
```bash
./main_all <x> <n> <threads> <toggles>
```
with the experiment you want to run.

For example:
```bash
./main_all 1000 25 4 --PrimePowers
```
will run the experiment on all primes up to 1000, and it will be run on all n between 2 and 25.

It will use 4 threads, and will include prime powers as well as regular primes.

# Variations of the experiment

```bash
# includes all experiments
./main_all

# just the theta error terms
./main_theta_data

# just finds data for first prime in each AP
./main_first_prime

# just finds data for largest gap in each AP
./main_largest_gap
```
All is likely the only one needed. Others are mostly for testing purposes.

# Toggles

These are all optional. You can run the program without any of them.

Toggles to change the expected error value:
```bash
--sqrt
--tripleLog
--doubleLog
--singleLog
```

Other Toggles:
```bash
# Adds list of prime powers to the list of primes which the iterator uses
--PrimePowers
```
