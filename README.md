# prng-with-randomness-checks

Ever dreamed of a PRNG (Pseudo-Random Number Generator) where randomness can be verified ?

There is a set of tools merged together in an utility named dieharder. 
However, some of the many tests in that utility fail a bit at random, due to ???? (overflows ? bugs ? mistakes ? luck ?) .
Cryptographers could be surprized to know that AES in CTR mode fails some of dieharder tests (and might suspect the test tools).

Anyway, since random sequences include sequences that are perceived as non-random for some tests, the paradox has no solution.

e.g. get linux randomness, and check it

```
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

The code in this repository implements a prng and generates a file which pass ALL dieharder tests

```
$ gcc -march=native -O3 prng_example.cpp -o prng_example

$ time ./prng_example prng_example.bin
real	0m2.557s
user	0m0.969s
sys	    0m1.585s

$ ls -la prng_example.bin 
-rw-rw-r-- 1 xxxxxx xxxxxx 880000000 May 23 13:42 prng_example.bin

$ dieharder -a -g 201 -f prng_example.bin

#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
 file_input_raw|                       linux.bin|  2.03e+07  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.64614801|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.95240993|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.69098489|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.40070944|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.34521282|  PASSED  
        diehard_opso|   0|   2097152|     100|0.17887484|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.65221636|  PASSED  
         diehard_dna|   0|   2097152|     100|0.19288201|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.53815825|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.62457001|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.36905637|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.67486914|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.58271280|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.47994823|  PASSED  
        diehard_sums|   0|       100|     100|0.41805025|  PASSED  
        diehard_runs|   0|    100000|     100|0.48774820|  PASSED  
        diehard_runs|   0|    100000|     100|0.68473244|  PASSED  
       diehard_craps|   0|    200000|     100|0.44711962|  PASSED  
       diehard_craps|   0|    200000|     100|0.96900872|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00000154|   WEAK   
 marsaglia_tsang_gcd|   0|  10000000|     100|0.10549008|  PASSED  
         sts_monobit|   1|    100000|     100|0.80070731|  PASSED  
            sts_runs|   2|    100000|     100|0.82087011|  PASSED  
          sts_serial|   1|    100000|     100|0.68145655|  PASSED  
          sts_serial|   2|    100000|     100|0.59637359|  PASSED  
          sts_serial|   3|    100000|     100|0.50253098|  PASSED  
          sts_serial|   3|    100000|     100|0.57985391|  PASSED  
          sts_serial|   4|    100000|     100|0.24114625|  PASSED  
          sts_serial|   4|    100000|     100|0.91669171|  PASSED  
          sts_serial|   5|    100000|     100|0.51953345|  PASSED  
          sts_serial|   5|    100000|     100|0.92803248|  PASSED  
          sts_serial|   6|    100000|     100|0.11909733|  PASSED  
          sts_serial|   6|    100000|     100|0.11058217|  PASSED  
          sts_serial|   7|    100000|     100|0.10962645|  PASSED  
          sts_serial|   7|    100000|     100|0.07357501|  PASSED  
          sts_serial|   8|    100000|     100|0.91520591|  PASSED  
          sts_serial|   8|    100000|     100|0.54840843|  PASSED  
          sts_serial|   9|    100000|     100|0.78191184|  PASSED  
          sts_serial|   9|    100000|     100|0.41903998|  PASSED  
          sts_serial|  10|    100000|     100|0.93502201|  PASSED  
          sts_serial|  10|    100000|     100|0.85360116|  PASSED  
          sts_serial|  11|    100000|     100|0.78122089|  PASSED  
          sts_serial|  11|    100000|     100|0.99378502|  PASSED  
          sts_serial|  12|    100000|     100|0.68560784|  PASSED  
          sts_serial|  12|    100000|     100|0.79870541|  PASSED  
          sts_serial|  13|    100000|     100|0.84256677|  PASSED  
          sts_serial|  13|    100000|     100|0.86266471|  PASSED  
          sts_serial|  14|    100000|     100|0.89831213|  PASSED  
          sts_serial|  14|    100000|     100|0.88608987|  PASSED  
          sts_serial|  15|    100000|     100|0.49505858|  PASSED  
          sts_serial|  15|    100000|     100|0.60930587|  PASSED  
          sts_serial|  16|    100000|     100|0.90223934|  PASSED  
          sts_serial|  16|    100000|     100|0.05875582|  PASSED  
         rgb_bitdist|   1|    100000|     100|0.51660491|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.98620256|  PASSED  
         rgb_bitdist|   3|    100000|     100|0.45270574|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.66602544|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.38685709|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.46681876|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.82526547|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.60634549|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.65110238|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.05433905|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.72884953|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.99311818|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.91295965|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.59861490|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.67634784|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.17178114|  PASSED  
    rgb_permutations|   2|    100000|     100|0.51230631|  PASSED  
    rgb_permutations|   3|    100000|     100|0.62551798|  PASSED  
    rgb_permutations|   4|    100000|     100|0.67098081|  PASSED  
    rgb_permutations|   5|    100000|     100|0.95856377|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.51339784|  PASSED  
      rgb_lagged_sum|   1|   1000000|     100|0.69297674|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.44597568|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.27831662|  PASSED  
      rgb_lagged_sum|   4|   1000000|     100|0.32903329|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.80331666|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.19100341|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.10526301|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.95060391|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.05120148|  PASSED  
      rgb_lagged_sum|  10|   1000000|     100|0.00000272|   WEAK   
      rgb_lagged_sum|  11|   1000000|     100|0.79874144|  PASSED  
      rgb_lagged_sum|  12|   1000000|     100|0.66348740|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.60950375|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.55078060|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.00007231|   WEAK   
      rgb_lagged_sum|  16|   1000000|     100|0.92288441|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.50980964|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.92895889|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.00345300|   WEAK   
      rgb_lagged_sum|  20|   1000000|     100|0.97278693|  PASSED  
      rgb_lagged_sum|  21|   1000000|     100|0.00196214|   WEAK   
      rgb_lagged_sum|  22|   1000000|     100|0.97533391|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.07520004|  PASSED  
      rgb_lagged_sum|  24|   1000000|     100|0.15618575|  PASSED  
      rgb_lagged_sum|  25|   1000000|     100|0.49966597|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.02790343|  PASSED  
      rgb_lagged_sum|  27|   1000000|     100|0.05723121|  PASSED  
      rgb_lagged_sum|  28|   1000000|     100|0.75232737|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.01633947|  PASSED  
      rgb_lagged_sum|  30|   1000000|     100|0.99232301|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.00003823|   WEAK   
      rgb_lagged_sum|  32|   1000000|     100|0.00001017|   WEAK   
     rgb_kstest_test|   0|     10000|    1000|0.91331020|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.93331402|  PASSED  
             dab_dct| 256|     50000|       1|0.44616891|  PASSED  
        dab_filltree|  32|  15000000|       1|0.13693600|  PASSED  
        dab_filltree|  32|  15000000|       1|0.24223913|  PASSED  
       dab_filltree2|   0|   5000000|       1|0.94176042|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.35888228|  PASSED  
        dab_monobit2|  12|  65000000|       1|0.61299683|  PASSED  
```

# Reuse the code ?

It is not complex to build, if needed, a library from this C standalone file. Just remove the main() function and rename the file .h .


