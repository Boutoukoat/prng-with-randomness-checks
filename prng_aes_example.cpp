
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <x86intrin.h>

typedef unsigned __int128 uint128_t;

class prng_aes
{

  private:
    __m128i R, L, state;

    inline uint64_t rotl(uint64_t a, uint8_t shift)
    {
        return (a << shift) ^ (a >> (64 - shift));
    }

    inline __m128i lcg(void)
    {
        __m128i s1 = _mm_slli_epi64(state, 8);
        __m128i s2 = _mm_slli_epi64(state, 3);
        // multiply by 137  (1 + multiple of 4)
        state = _mm_add_epi64(state, s1);
        state = _mm_add_epi64(state, s2);
        // add 13 or 17   (1 + multiple of 4)
        state = _mm_add_epi64(state, _mm_set_epi64x(13, 17));
        // shuffle
        __m128i r = state;
        r = _mm_xor_si128(r, _mm_srli_epi64(state, 31)); // rather large shift
        r = _mm_xor_si128(r, _mm_srli_epi64(state, 13)); // rather small shift
        r = _mm_xor_si128(r, _mm_set_epi64x(0x1ull, 0x1ull));
        return r;
    }

  public:
    prng_aes(void)
    {
        state = _mm_set_epi64x(0xffffaaaaffffaaaaull, 0x1234567890abcdefull); // 1 odd, 1 even
        lcg();                                                                // warm up
        lcg();
        lcg();
        R = lcg();
        lcg(); // warm up
        lcg();
        lcg();
        L = lcg();
        feistel(); // warm up
        feistel();
        feistel();
    }

    // An odd number of Feistel rounds
    // Finish with a trap function
    uint64_t feistel(void)
    {
        __m128i ll;
        // Feistel round 1
        ll = L;
        L = R;
        R = _mm_xor_si128(ll, _mm_aesenc_si128(R, lcg()));
        // Feistel round 2
        ll = L;
        L = R;
        R = _mm_xor_si128(ll, _mm_aesenc_si128(R, lcg()));
        // Feistel round 3
        ll = L;
        L = R;
        R = _mm_xor_si128(ll, _mm_aesenc_si128(R, lcg()));
        // trap function
        return rotl(_mm_extract_epi64(R, 0), 37) ^ _mm_extract_epi64(L, 1);
    }

    void fill(uint64_t *buff, unsigned len)
    {
        for (unsigned i = 0; i < len; i++)
        {
            buff[i] = feistel();
        }
    }
};

int main(int argc, char **argv)
{
    const unsigned LEN = 1ul << 17;
    static uint64_t buff[LEN];
    FILE *f = stdout;
    unsigned long mb = 4096;

    prng_aes p;
    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-mb"))
        {
            mb = atol(argv[++i]);
            continue;
        }
        if (!strcmp(argv[i], "-f"))
        {
            f = fopen(argv[++i], "wb");
            continue;
        }
        printf("Parameter error\n");
        exit(1);
    }

    // generate 4GB of data
    for (unsigned long i = 0; i < mb; i++)
    {
        p.fill(buff, LEN);
        if (fwrite(buff, sizeof(uint64_t), LEN, f) != LEN)
        {
            // cannot write
            break;
        }
    }

    if (f != stdout)
    {
        fclose(f);
    }
    return 0;
}
