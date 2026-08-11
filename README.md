# prng-with-randomness-checks

When implementing a prng in a software, it is wise to verify its output.

Ever dreamed of a PRNG (Pseudo-Random Number Generator) where randomness can be verified ?

But how to verify something which is supposed to be randomily generated ?

Since random sequences include sequences that are perceived as non-random for some tests, the paradox has no solution. But in most cases, the PRNG testers can distinguish between weak generators and failed generators.

e.g. get Linux randomness, and check it

```
$ sudo apt-get install dieharder

$ dd if=/dev/urandom of=linux.bin bs=1000000 count=4000

$ dieharder -g 201 -a -f linux.bin  | grep FAILED
 marsaglia_tsang_gcd|   0|  10000000|     100|0.00000000|  FAILED  
      rgb_lagged_sum|   9|   1000000|     100|0.00000000|  FAILED  
      rgb_lagged_sum|  24|   1000000|     100|0.00000000|  FAILED  
      rgb_lagged_sum|  29|   1000000|     100|0.00000000|  FAILED
      rgb_lagged_sum|  31|   1000000|     100|0.00000000|  FAILED  
```

However, some of the many tests in that utility fail a bit at random, due to ???? (overflows ? bugs ? mistakes ? luck ?) .
Cryptographers could be surprized to know that AES in CTR mode fails some of dieharder tests (and so might suspect the test tools).

The impacts of failing randomness tests are multiple

- The prng logic might be perceived as suspicious 
- Monte-Carlo types of tests based on suspicious prng are seen as biased

# What makes this PRNG different ?

The code in this repository implements a prng, and generates a file which pass ALL dieharder tests

The structure of this generator is very simple, it is the combination of 3 known techniques: a (quite good) lcg provides an infinite key schedule for 3 rounds of Feistel cipher, plus a trapdoor function.

- https://en.wikipedia.org/wiki/Linear_congruential_generator
- https://en.wikipedia.org/wiki/Feistel_cipher
- https://en.wikipedia.org/wiki/Trapdoor_function

# Dieharder

The diehard tests are a battery of statistical tests for measuring the quality of a random number generator 

https://en.wikipedia.org/wiki/Diehard_tests

```
$ sudo apt-get install dieharder

$ gcc -march=native -O3 prng_example.cpp -o prng_example

$ time ./prng_example prng_example.bin
real    0m18.079s
user    0m4.795s
sys     0m4.349s

$ wc -c prng_example.bin
4294967296 prng_example.bin

$ dieharder -a -g 201 -f prng_example.bin

#=============================================================================#
#            dieharder version 3.31.1 Copyright 2003 Robert G. Brown          #
#=============================================================================#
   rng_name    |           filename             |rands/second|
 file_input_raw|                prng_example.bin|  3.02e+07  |
#=============================================================================#
        test_name   |ntup| tsamples |psamples|  p-value |Assessment
#=============================================================================#
   diehard_birthdays|   0|       100|     100|0.68948565|  PASSED  
      diehard_operm5|   0|   1000000|     100|0.45500745|  PASSED  
  diehard_rank_32x32|   0|     40000|     100|0.94717263|  PASSED  
    diehard_rank_6x8|   0|    100000|     100|0.74425940|  PASSED  
   diehard_bitstream|   0|   2097152|     100|0.31164842|  PASSED  
        diehard_opso|   0|   2097152|     100|0.82174915|  PASSED  
        diehard_oqso|   0|   2097152|     100|0.90729148|  PASSED  
         diehard_dna|   0|   2097152|     100|0.87536351|  PASSED  
diehard_count_1s_str|   0|    256000|     100|0.07219878|  PASSED  
diehard_count_1s_byt|   0|    256000|     100|0.87464797|  PASSED  
 diehard_parking_lot|   0|     12000|     100|0.66176816|  PASSED  
    diehard_2dsphere|   2|      8000|     100|0.56896806|  PASSED  
    diehard_3dsphere|   3|      4000|     100|0.82366648|  PASSED  
     diehard_squeeze|   0|    100000|     100|0.55298047|  PASSED  
        diehard_sums|   0|       100|     100|0.67347661|  PASSED  
        diehard_runs|   0|    100000|     100|0.09775568|  PASSED  
        diehard_runs|   0|    100000|     100|0.98181228|  PASSED  
       diehard_craps|   0|    200000|     100|0.67448305|  PASSED  
       diehard_craps|   0|    200000|     100|0.95550822|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.08148197|  PASSED  
 marsaglia_tsang_gcd|   0|  10000000|     100|0.62065319|  PASSED  
         sts_monobit|   1|    100000|     100|0.24704598|  PASSED  
            sts_runs|   2|    100000|     100|0.81936218|  PASSED  
          sts_serial|   1|    100000|     100|0.93425039|  PASSED  
          sts_serial|   2|    100000|     100|0.05019417|  PASSED  
          sts_serial|   3|    100000|     100|0.03820062|  PASSED  
          sts_serial|   3|    100000|     100|0.23741817|  PASSED  
          sts_serial|   4|    100000|     100|0.41168096|  PASSED  
          sts_serial|   4|    100000|     100|0.39285310|  PASSED  
          sts_serial|   5|    100000|     100|0.42683740|  PASSED  
          sts_serial|   5|    100000|     100|0.61046545|  PASSED  
          sts_serial|   6|    100000|     100|0.25104356|  PASSED  
          sts_serial|   6|    100000|     100|0.85514357|  PASSED  
          sts_serial|   7|    100000|     100|0.64136180|  PASSED  
          sts_serial|   7|    100000|     100|0.53174391|  PASSED  
          sts_serial|   8|    100000|     100|0.25458998|  PASSED  
          sts_serial|   8|    100000|     100|0.32130841|  PASSED  
          sts_serial|   9|    100000|     100|0.49508660|  PASSED  
          sts_serial|   9|    100000|     100|0.91359783|  PASSED  
          sts_serial|  10|    100000|     100|0.60339799|  PASSED  
          sts_serial|  10|    100000|     100|0.79227647|  PASSED  
          sts_serial|  11|    100000|     100|0.62607755|  PASSED  
          sts_serial|  11|    100000|     100|0.96420219|  PASSED  
          sts_serial|  12|    100000|     100|0.98402820|  PASSED  
          sts_serial|  12|    100000|     100|0.57272492|  PASSED  
          sts_serial|  13|    100000|     100|0.60046820|  PASSED  
          sts_serial|  13|    100000|     100|0.65015622|  PASSED  
          sts_serial|  14|    100000|     100|0.76592885|  PASSED  
          sts_serial|  14|    100000|     100|0.29738286|  PASSED  
          sts_serial|  15|    100000|     100|0.58948370|  PASSED  
          sts_serial|  15|    100000|     100|0.96213000|  PASSED  
          sts_serial|  16|    100000|     100|0.89164521|  PASSED  
          sts_serial|  16|    100000|     100|0.79090747|  PASSED  
         rgb_bitdist|   1|    100000|     100|0.99301591|  PASSED  
         rgb_bitdist|   2|    100000|     100|0.03513911|  PASSED  
         rgb_bitdist|   3|    100000|     100|0.99151420|  PASSED  
         rgb_bitdist|   4|    100000|     100|0.71700624|  PASSED  
         rgb_bitdist|   5|    100000|     100|0.79893499|  PASSED  
         rgb_bitdist|   6|    100000|     100|0.25044778|  PASSED  
         rgb_bitdist|   7|    100000|     100|0.63940745|  PASSED  
         rgb_bitdist|   8|    100000|     100|0.35866624|  PASSED  
         rgb_bitdist|   9|    100000|     100|0.69344998|  PASSED  
         rgb_bitdist|  10|    100000|     100|0.62450075|  PASSED  
         rgb_bitdist|  11|    100000|     100|0.00566478|  PASSED  
         rgb_bitdist|  12|    100000|     100|0.18794185|  PASSED  
rgb_minimum_distance|   2|     10000|    1000|0.29886262|  PASSED  
rgb_minimum_distance|   3|     10000|    1000|0.68521438|  PASSED  
rgb_minimum_distance|   4|     10000|    1000|0.76037561|  PASSED  
rgb_minimum_distance|   5|     10000|    1000|0.03541875|  PASSED  
    rgb_permutations|   2|    100000|     100|0.74120864|  PASSED  
    rgb_permutations|   3|    100000|     100|0.58287843|  PASSED  
    rgb_permutations|   4|    100000|     100|0.64220282|  PASSED  
    rgb_permutations|   5|    100000|     100|0.83906013|  PASSED  
      rgb_lagged_sum|   0|   1000000|     100|0.97536329|  PASSED  
      rgb_lagged_sum|   1|   1000000|     100|0.68897649|  PASSED  
      rgb_lagged_sum|   2|   1000000|     100|0.25316521|  PASSED  
      rgb_lagged_sum|   3|   1000000|     100|0.01754963|  PASSED  
      rgb_lagged_sum|   4|   1000000|     100|0.79529128|  PASSED  
      rgb_lagged_sum|   5|   1000000|     100|0.12302629|  PASSED  
      rgb_lagged_sum|   6|   1000000|     100|0.08082325|  PASSED  
      rgb_lagged_sum|   7|   1000000|     100|0.25628578|  PASSED  
      rgb_lagged_sum|   8|   1000000|     100|0.78212114|  PASSED  
      rgb_lagged_sum|   9|   1000000|     100|0.17571319|  PASSED  
      rgb_lagged_sum|  10|   1000000|     100|0.52417581|  PASSED  
      rgb_lagged_sum|  11|   1000000|     100|0.62221742|  PASSED  
      rgb_lagged_sum|  12|   1000000|     100|0.68079501|  PASSED  
      rgb_lagged_sum|  13|   1000000|     100|0.47498229|  PASSED  
      rgb_lagged_sum|  14|   1000000|     100|0.51299452|  PASSED  
      rgb_lagged_sum|  15|   1000000|     100|0.17482502|  PASSED  
      rgb_lagged_sum|  16|   1000000|     100|0.93552854|  PASSED  
      rgb_lagged_sum|  17|   1000000|     100|0.04214981|  PASSED  
      rgb_lagged_sum|  18|   1000000|     100|0.88297880|  PASSED  
      rgb_lagged_sum|  19|   1000000|     100|0.17467922|  PASSED  
      rgb_lagged_sum|  20|   1000000|     100|0.65366613|  PASSED  
      rgb_lagged_sum|  21|   1000000|     100|0.69346098|  PASSED  
      rgb_lagged_sum|  22|   1000000|     100|0.45477569|  PASSED  
      rgb_lagged_sum|  23|   1000000|     100|0.04584833|  PASSED  
      rgb_lagged_sum|  24|   1000000|     100|0.60271492|  PASSED  
      rgb_lagged_sum|  25|   1000000|     100|0.57217878|  PASSED  
      rgb_lagged_sum|  26|   1000000|     100|0.80129714|  PASSED  
      rgb_lagged_sum|  27|   1000000|     100|0.11592246|  PASSED  
      rgb_lagged_sum|  28|   1000000|     100|0.08826469|  PASSED  
      rgb_lagged_sum|  29|   1000000|     100|0.34150757|  PASSED  
      rgb_lagged_sum|  30|   1000000|     100|0.93382118|  PASSED  
      rgb_lagged_sum|  31|   1000000|     100|0.08800161|  PASSED  
      rgb_lagged_sum|  32|   1000000|     100|0.48624140|  PASSED  
     rgb_kstest_test|   0|     10000|    1000|0.82683223|  PASSED  
     dab_bytedistrib|   0|  51200000|       1|0.83216858|  PASSED  
             dab_dct| 256|     50000|       1|0.84266596|  PASSED  
        dab_filltree|  32|  15000000|       1|0.08037161|  PASSED  
        dab_filltree|  32|  15000000|       1|0.64473622|  PASSED  
       dab_filltree2|   0|   5000000|       1|0.95422362|  PASSED  
       dab_filltree2|   1|   5000000|       1|0.75585399|  PASSED  
        dab_monobit2|  12|  65000000|       1|0.16793877|  PASSED  
```

# PractRand tests

PractRand provides statistical tests and pseudo-random number generators 

PractRand can be found and installed from https://pracrand.sourceforge.net https://sourceforge.net/projects/pracrand/files

```
cat prng_example.bin | tools/RNG_test stdin
RNG_test using PractRand version 0.96
RNG = RNG_stdin, seed = unknown
test set = core, folding = standard(unknown format)

rng=RNG_stdin, seed=unknown
length= 64 megabytes (2^26 bytes), time= 2.4 seconds
  no anomalies in 174 test result(s)

rng=RNG_stdin, seed=unknown
length= 128 megabytes (2^27 bytes), time= 5.8 seconds
  no anomalies in 187 test result(s)

rng=RNG_stdin, seed=unknown
length= 256 megabytes (2^28 bytes), time= 10.7 seconds
  no anomalies in 201 test result(s)

rng=RNG_stdin, seed=unknown
length= 512 megabytes (2^29 bytes), time= 20.4 seconds
  no anomalies in 216 test result(s)

rng=RNG_stdin, seed=unknown
length= 1 gigabyte (2^30 bytes), time= 39.9 seconds
  no anomalies in 231 test result(s)

rng=RNG_stdin, seed=unknown
length= 2 gigabytes (2^31 bytes), time= 75.8 seconds
  no anomalies in 246 test result(s)

rng=RNG_stdin, seed=unknown
length= 4 gigabytes (2^32 bytes), time= 144 seconds
  no anomalies in 261 test result(s)

rng=RNG_stdin, seed=unknown
length= 8 gigabytes (2^33 bytes), time= 283 seconds
  no anomalies in 274 test result(s)
```

# SmokeRand test

SmokeRand is a set of tests for pseudorandom number generators (PRNGs). Tested generators should return either 32-bit or 64-bit unsigned uniformly distributed unsigned integers.

SmokeRand can be found and installed from https://github.com/alvoskov/SmokeRand


```
./prng_example  | bin/smokerand full stdin64
    # Test name                    xemp              p Interpretation  Thr#
-------------------------------------------------------------------------------
    1 monobit_freq             0.485565          0.314 Ok                 0
    2 byte_freq                0.809866          0.528 Ok                 0
    3 word16_freq               1.14208          0.147 Ok                 0
    4 bspace64_1d                   967          0.848 Ok                 0
    5 bspace32_1d                 32460          0.955 Ok                 0
    6 bspace32_1d_high            32883          0.262 Ok                 0
    7 bspace32_2d                   978          0.751 Ok                 0
    8 bspace32_2d_high              985          0.675 Ok                 0
    9 bspace21_3d                   814          0.303 Ok                 0
   10 bspace21_3d_high              753          0.951 Ok                 0
   11 bspace16_4d                   792          0.602 Ok                 0
   12 bspace16_4d_high              786          0.682 Ok                 0
   13 bspace8_8d                    783          0.719 Ok                 0
   14 bspace8_8d_high               811          0.340 Ok                 0
   15 bspace4_8d_dec                 11          0.003 Ok                 0
   16 bspace4_16d                   791          0.616 Ok                 0
   17 bspace4_16d_high              845          0.055 Ok                 0
   18 collover20_2d               56824          0.530 Ok                 0
   19 collover20_2d_high          56850          0.486 Ok                 0
   20 collover13_3d              113488          0.718 Ok                 0
   21 collover13_3d_high         113781          0.385 Ok                 0
   22 collover8_5d                56779          0.604 Ok                 0
   23 collover8_5d_high           56647          0.793 Ok                 0
   24 collover5_8d                56922          0.368 Ok                 0
   25 collover5_8d_high           56804          0.563 Ok                 0
   26 collover3_13d              113485          0.721 Ok                 0
   27 collover3_13d_high         112963          0.984 Ok                 0
   28 collover2_20d               56518          0.913 Ok                 0
   29 collover2_20d_high          56460          0.946 Ok                 0
   30 gap_inv8                  146.221          0.059 Ok                 0
   31 gap_inv512                3846.32          0.621 Ok                 0
   32 gap_inv1024               9331.53          0.693 Ok                 0
   33 gap16_count0              1.94435          0.026 Ok                 0
   34 hamming_distr             1.60471          0.900 Ok                 0
   35 hamming_ot               -2.46353          0.993 Ok                 0
   36 hamming_ot_low1        -0.0280932          0.511 Ok                 0
   37 hamming_ot_low8           -1.4151          0.921 Ok                 0
   38 hamming_ot_values         1.69248          0.045 Ok                 0
   39 hamming_ot_u128          0.366488          0.357 Ok                 0
   40 hamming_ot_u256           1.71581          0.043 Ok                 0
   41 hamming_ot_u512          0.355373          0.361 Ok                 0
   42 linearcomp_high            500002          0.917 Ok                 0
   43 linearcomp_mid             499998          0.042 Ok                 0
   44 linearcomp_low             500000          0.667 Ok                 0
   45 matrixrank_4096           2.09984          0.350 Ok                 0
   46 matrixrank_4096_low8      1.05933          0.589 Ok                 0
   47 matrixrank_8192          0.505654          0.777 Ok                 0
   48 matrixrank_8192_low8      8.23272          0.016 Ok                 0
   49 mod3                      2.72428          0.003 Ok                 0
   50 sumcollector              40.8688          0.071 Ok                 0
-------------------------------------------------------------------------------
Passed:        50
Suspicious:    0
Failed:        0
Quality (0-4): 4.00 (good)
```


# Reuse the code ?

It is not complex to build, if needed, a library from this C standalone file. Just remove the main() function and rename the file .h .


