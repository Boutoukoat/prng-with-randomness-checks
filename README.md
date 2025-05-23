# prng-with-randomness-checks

Ever dreamed of a PRNG (Pseudo-Random Number Generator) where randomness can be verified ?

There is a set of tools, all merged together, in a single utility named \"dieharder\". 

https://webhome.phy.duke.edu/~rgb/General/dieharder.php
https://linux.die.net/man/1/dieharder
https://rurban.github.io/dieharder/manual/dieharder.pdf
https://github.com/seehuhn/dieharder

When implementing a prng in a software, it is wise to verify its output, and this is the purpose of dieharder.

However, some of the many tests in that utility fail a bit at random, due to ???? (overflows ? bugs ? mistakes ? luck ?) .
Cryptographers could be surprized to know that AES in CTR mode fails some of dieharder tests (and so might suspect the test tools).

Anyway, since random sequences include sequences that are perceived as non-random for some tests, the paradox has no solution.

e.g. get Linux randomness, and check it

```
$ sudo apt-get install dieharder

$ dd if=/dev/urandom of=linux.bin bs=1000000 count=800

$ dieharder -g 201 -a -f linux.bin  | grep FAILED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00000000|  FAILED  
      rgb_lagged_sum|   9|   1000000|     100|0.00000000|  FAILED  
      rgb_lagged_sum|  24|   1000000|     100|0.00000000|  FAILED  
      rgb_lagged_sum|  29|   1000000|     100|0.00000000|  FAILED
      rgb_lagged_sum|  31|   1000000|     100|0.00000000|  FAILED  
```

The impacts of failing randomness tests are multiple
- The prng logic might be perceived as suspicious 
- Monte-Carlo types of tests based on suspicious prng are seen as biased

# What makes this PRNG different ?

The code in this repository implements a prng, and generates a file which pass ALL dieharder tests

```
$ sudo apt-get install dieharder

$ gcc -march=native -O3 prng_example.cpp -o prng_example

$ time ./prng_example prng_example.bin
real	0m2.557s
user	0m0.969s
sys	    0m1.585s

$ wc -c prng_example.bin 
879996240 prng_example.bin

$ dieharder -a -g 201 -f prng_example.bin

#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
 file_input_raw|                       linux.bin|  2.03e+07  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.22765395|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.60610202|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.17374579|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.25582303|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.96220183|  PASSED  
        diehard_opso|   0|   2097152|     100|0.91513954|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.80103322|  PASSED  
         diehard_dna|   0|   2097152|     100|0.77967385|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.97798479|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.81159346|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.24520972|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.76330277|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.72745603|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.44185660|  PASSED  
        diehard_sums|   0|       100|     100|0.05008577|  PASSED  
        diehard_runs|   0|    100000|     100|0.49977794|  PASSED  
        diehard_runs|   0|    100000|     100|0.01066151|  PASSED  
       diehard_craps|   0|    200000|     100|0.84910752|  PASSED  
       diehard_craps|   0|    200000|     100|0.97705623|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.03765365|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00016352|   WEAK   
         sts_monobit|   1|    100000|     100|0.19907084|  PASSED  
            sts_runs|   2|    100000|     100|0.58319856|  PASSED  
          sts_serial|   1|    100000|     100|0.84416972|  PASSED  
          sts_serial|   2|    100000|     100|0.97368053|  PASSED  
          sts_serial|   3|    100000|     100|0.88118793|  PASSED  
          sts_serial|   3|    100000|     100|0.09040470|  PASSED  
          sts_serial|   4|    100000|     100|0.27414124|  PASSED  
          sts_serial|   4|    100000|     100|0.01384772|  PASSED  
          sts_serial|   5|    100000|     100|0.29098265|  PASSED  
          sts_serial|   5|    100000|     100|0.83008615|  PASSED  
          sts_serial|   6|    100000|     100|0.33112075|  PASSED  
          sts_serial|   6|    100000|     100|0.65427623|  PASSED  
          sts_serial|   7|    100000|     100|0.41305921|  PASSED  
          sts_serial|   7|    100000|     100|0.29277611|  PASSED  
          sts_serial|   8|    100000|     100|0.59523818|  PASSED  
          sts_serial|   8|    100000|     100|0.72808359|  PASSED  
          sts_serial|   9|    100000|     100|0.82435107|  PASSED  
          sts_serial|   9|    100000|     100|0.50246424|  PASSED  
          sts_serial|  10|    100000|     100|0.24293229|  PASSED  
          sts_serial|  10|    100000|     100|0.28946120|  PASSED  
          sts_serial|  11|    100000|     100|0.22643144|  PASSED  
          sts_serial|  11|    100000|     100|0.85944356|  PASSED  
          sts_serial|  12|    100000|     100|0.03042416|  PASSED  
          sts_serial|  12|    100000|     100|0.13898124|  PASSED  
          sts_serial|  13|    100000|     100|0.16106929|  PASSED  
          sts_serial|  13|    100000|     100|0.34141730|  PASSED  
          sts_serial|  14|    100000|     100|0.89059711|  PASSED  
          sts_serial|  14|    100000|     100|0.96948418|  PASSED  
          sts_serial|  15|    100000|     100|0.09717622|  PASSED  
          sts_serial|  15|    100000|     100|0.62585513|  PASSED  
          sts_serial|  16|    100000|     100|0.63279260|  PASSED  
          sts_serial|  16|    100000|     100|0.44757389|  PASSED  
         rgb_bitdist|   1|    100000|     100|0.42290132|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.96860929|  PASSED  
         rgb_bitdist|   3|    100000|     100|0.99439654|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.83604670|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.10266085|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.09970053|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.91753597|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.24657561|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.14332301|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.72460054|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.30356812|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.23183519|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.09775195|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.12959401|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.00003557|   WEAK   
rgb_minimum_distance|   5|     10000|    1000|0.05003014|  PASSED  
    rgb_permutations|   2|    100000|     100|0.90439775|  PASSED  
    rgb_permutations|   3|    100000|     100|0.28168167|  PASSED  
    rgb_permutations|   4|    100000|     100|0.50787140|  PASSED  
    rgb_permutations|   5|    100000|     100|0.88671128|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.79958254|  PASSED  
      rgb_lagged_sum|   1|   1000000|     100|0.72880900|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.85752828|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.01012412|  PASSED  
      rgb_lagged_sum|   4|   1000000|     100|0.36919256|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.81844605|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.82131793|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.15074865|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.03403854|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.00002378|   WEAK   
      rgb_lagged_sum|  10|   1000000|     100|0.79435994|  PASSED  
      rgb_lagged_sum|  11|   1000000|     100|0.00017385|   WEAK   
      rgb_lagged_sum|  12|   1000000|     100|0.05054573|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.26663432|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.05801454|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.93940479|  PASSED  
      rgb_lagged_sum|  16|   1000000|     100|0.28613748|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.54505147|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.89016493|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.06407712|  PASSED  
      rgb_lagged_sum|  20|   1000000|     100|0.35060280|  PASSED  
      rgb_lagged_sum|  21|   1000000|     100|0.84076164|  PASSED  
      rgb_lagged_sum|  22|   1000000|     100|0.66223325|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.03865661|  PASSED  
      rgb_lagged_sum|  24|   1000000|     100|0.32960227|  PASSED  
      rgb_lagged_sum|  25|   1000000|     100|0.94215747|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.00317576|   WEAK   
      rgb_lagged_sum|  27|   1000000|     100|0.00019399|   WEAK   
      rgb_lagged_sum|  28|   1000000|     100|0.71493385|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.00029144|   WEAK   
      rgb_lagged_sum|  30|   1000000|     100|0.75527427|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.30650200|  PASSED  
      rgb_lagged_sum|  32|   1000000|     100|0.78516330|  PASSED  
     rgb_kstest_test|   0|     10000|    1000|0.43662583|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.19928518|  PASSED  
             dab_dct| 256|     50000|       1|0.54548701|  PASSED  
        dab_filltree|  32|  15000000|       1|0.36436566|  PASSED  
        dab_filltree|  32|  15000000|       1|0.52458913|  PASSED  
       dab_filltree2|   0|   5000000|       1|0.84612534|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.87055665|  PASSED  
        dab_monobit2|  12|  65000000|       1|0.15727432|  PASSED  
```

# Reuse the code ?

It is not complex to build, if needed, a library from this C standalone file. Just remove the main() function and rename the file .h .


