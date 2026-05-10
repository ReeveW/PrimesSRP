# PrimesSRP

# Things I've thought about:

How to test any changes to the experiment? 

Possible edge cases for program: initial value is prime, first value is prime, extremely large values, the values that Dr. Fiori found that took a large number of steps to find, 

How do the bounds work? We want to know what happens for numbers up to 10^10, but what does that mean? If we have variables for starting value and step size, is 10^10 the bound for the start, step size, or both? Or do we just stop the 
program once a number goes out of bounds?

Why can't we just store all primes up to 10^10 in memory? Only 50 million, can store in 2GB of ram (assuming 32 bit integers). Then lookup is O(1), and experiment seems really easy.

