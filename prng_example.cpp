
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>

struct prng_rand_t
{
    __m128i key[9];
    __m128i state;
    uint64_t seed1;
    uint64_t seed2;
};

union prng_bytes_16_t {
    uint64_t s[2];
    uint8_t r[16];
} __attribute__((__packed__));

void prng_init_rand(prng_rand_t *c, uint64_t s)
{
    c->seed1 = s;
    c->seed2 = ~(s * s);
    // a few hexisms to initiate a state
    c->state = _mm_set_epi32(0x0000ffff, 0x1111eeee, 0x2222dddd, 0x3333cccc);

    // a few hexisms to initiate a AES scheduling
    c->key[0] = _mm_set_epi32(0xb0000001, 0xba1dbabe, 0xdeadf00d, 0xf0000001);

    c->key[1] = _mm_set_epi32(0xbadfaece, 0xdeadbeef, 0x87654321, 0x09abcdef);
    c->key[1] = _mm_aesenc_si128(c->key[1], c->key[0]);

    c->key[2] = _mm_set_epi32(0xcafecafe, 0xb01dface, 0x09abcdef, 0x12345678);
    c->key[2] = _mm_aesenc_si128(c->key[2], c->key[0]);

    c->key[3] = _mm_set_epi32(0x0bad0bad, 0xdeaddeaf, 0x2baddead, 0xdeadface);
    c->key[3] = _mm_aesenc_si128(c->key[3], c->key[1]);

    c->key[4] = _mm_set_epi32(0xb000dead, 0xb0bababe, 0xcafed00d, 0xfacefeed);
    c->key[4] = _mm_aesenc_si128(c->key[4], c->key[2]);

    c->key[5] = _mm_set_epi32(0x09abcdef, 0x87654321, 0xfeedbabe, 0x2badface);
    c->key[5] = _mm_aesenc_si128(c->key[5], c->key[3]);

    c->key[6] = _mm_set_epi32(0xfeed0123, 0xface4567, 0xbabe89ab, 0xdeadabcd);
    c->key[6] = _mm_aesenc_si128(c->key[6], c->key[4]);

    c->key[7] = _mm_xor_si128(c->key[6], c->key[4]);
    c->key[7] = _mm_aesenc_si128(c->key[7], c->key[5]);

    c->key[8] = _mm_xor_si128(c->key[3], c->key[1]);
    c->key[8] = _mm_aesenc_si128(c->key[8], c->key[2]);

    // make it looking like a AES key
    c->key[0] = _mm_aeskeygenassist_si128(c->key[0], 0);
    c->key[1] = _mm_aeskeygenassist_si128(c->key[1], 3);
    c->key[2] = _mm_aeskeygenassist_si128(c->key[2], 6);
    c->key[3] = _mm_aeskeygenassist_si128(c->key[3], 9);
    c->key[4] = _mm_aeskeygenassist_si128(c->key[4], 12);
    c->key[5] = _mm_aeskeygenassist_si128(c->key[5], 15);
    c->key[6] = _mm_aeskeygenassist_si128(c->key[6], 18);
    c->key[7] = _mm_aeskeygenassist_si128(c->key[7], 21);
    c->key[8] = _mm_aeskeygenassist_si128(c->key[8], 24);
}

void prng_get_rand(prng_rand_t *c, uint8_t *r, unsigned len)
{
    __m128i b, a = c->state;
    prng_bytes_16_t u;
    while (len > 0)
    {
        // 2 Linear congruential generators with known period 2^64
        // refresh the seeds mod 2^64
        c->seed1 *= 137;
        c->seed1 += 13;
        c->seed2 *= 11213;
        c->seed2 += 132049;
        // insert the seeds in the state
        a = _mm_insert_epi64(a, (c->seed1 >> 31) ^ (c->seed1 << 13) ^ (c->seed2), 1);
        // 3 rounds of AES block cipher applied to the state
        a = _mm_aesenc_si128(a, c->key[0]);
        a = _mm_aesenc_si128(a, c->key[1]);
        a = _mm_aesenc_si128(a, c->key[2]);
        // 6 rounds of AES block cipher applied to
        b = _mm_aesenc_si128(a, c->key[3]);
        b = _mm_aesenc_si128(b, c->key[4]);
        b = _mm_aesenc_si128(b, c->key[5]);
        b = _mm_aesenc_si128(b, c->key[6]);
        b = _mm_aesenc_si128(b, c->key[7]);
        b = _mm_aesenc_si128(b, c->key[8]);
        // extract the AES output
        u.s[0] = _mm_extract_epi64(b, 0);
        u.s[1] = _mm_extract_epi64(b, 1);
        // copy the requested amount, up to 15 bytes (1 byte is discarded)
        unsigned l = len > 15 ? 15 : len;
        memcpy(r, &u.r[0], l);
        r += l;
        len -= l;
    }
    c->state = a;
}

int main(int argc, const char **argv)
{
    const char *filename = "prng_example.bin";
    if (argc > 1)
    {
        filename = argv[1];
    }

    FILE *fd = fopen(filename, "wb");
    if (fd)
    {
        uint8_t r[44000];
        prng_rand_t c;

        // initialize the generator
        prng_init_rand(&c, 100);

        for (int i = 0; i < 20000; i++)
        {
            // get randomness
            prng_get_rand(&c, r, 44000);

            if (fwrite(r, 44, 1000, fd) != 1000)
            {
                printf("Unable to write to file %s\n", filename);
                exit(1);
            }
        }

        fclose(fd);
    }
    else
    {
        printf("Unable to open file %s\n", filename);
        exit(1);
    }
    return (0);
}
