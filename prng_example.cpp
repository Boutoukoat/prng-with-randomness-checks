
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static inline uint64_t rotl(uint64_t a, uint8_t shift)
{
    return (a << shift) ^ (a >> (64 - shift));
}

static inline uint64_t lcg(void)
{
    static uint64_t state = 0xaaaaffffaaaafffful;
    state = state * 137 + 13;
    return state ^ (state << 13) ^ (state >> 17);
}

uint64_t feistel(void)
{
    static uint64_t R = 0x9abcdef012345678ul;
    static uint64_t L = 0x123456789abcdef0ul;
    uint64_t rr, ll;
    // Feistel round 1
    rr = R;
    ll = L;
    L = rr;
    R = ll ^ lcg() ^ rotl(rr, 60);
    // Feistel round 2
    rr = R;
    ll = L;
    L = rr;
    R = ll ^ lcg() ^ rotl(rr, 59);
    // Feistel round 3
    rr = R;
    ll = L;
    L = rr;
    R = ll ^ lcg() ^ rotl(rr, 58);
    // trap function
    return L ^ rotl(R, 31);
}

const uint64_t LEN = 1ul << 17;
uint64_t buff[LEN];

void fill(void)
{
    for (uint64_t i = 0; i < LEN; i++)
    {
        buff[i] = feistel();
    }
}

int main(int argc, char **argv)
{
    feistel();
    feistel();
    feistel();
    if (argc == 2)
    {
        FILE *f = fopen(argv[1], "wb");
        for (unsigned i = 0; i < 4096; i++)
        {
            fill();
            fwrite(buff, sizeof(uint64_t), LEN, f);
        }
        fclose(f);
    }
    else
    {
        while (1)
        {
            fill();
            fwrite(buff, sizeof(uint64_t), LEN, stdout);
        }
    }
    return 0;
}
