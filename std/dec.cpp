#include <stdio.h>
#include <inttypes.h>
//#include "bits/stdc++.h"
//using namespace std;

/*
** Using documented GCC type unsigned __int128 instead of undocumented
** obsolescent typedef name __uint128_t.  Works with GCC 4.7.1 but not
** GCC 4.1.2 (but __uint128_t works with GCC 4.1.2) on Mac OS X 10.7.4.
*/
typedef unsigned __int128 uint128_t;

/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL   /* 19 zeroes */
#define E10_UINT64 19

#define STRINGIZER(x)   # x
#define TO_STRING(x)    STRINGIZER(x)

static int print_u128_u(uint128_t u128)
{
    int rc;
    if (u128 > UINT64_MAX)
    {
        uint128_t leading  = u128 / P10_UINT64;
        uint64_t  trailing = u128 % P10_UINT64;
        rc = print_u128_u(leading);
        rc += printf("%." TO_STRING(E10_UINT64) PRIu64, trailing);
    }
    else
    {
        uint64_t u64 = u128;
        rc = printf("%" PRIu64, u64);
    }
    return rc;
}

int print_uint128_decimal(__uint128_t big) {
  size_t rc = 0;
  size_t i = 0;
  if (big >> 64) {
    char buf[40];
    while (big / P10_UINT64) {
      rc += sprintf(buf + E10_UINT64 * i, "%." TO_STRING(E10_UINT64) PRIu64, (uint64_t)(big % P10_UINT64));
      ++i;
      big /= P10_UINT64;
    }
    rc += printf("%" PRIu64, (uint64_t)big);
    while (i--) {
      fwrite(buf + E10_UINT64 * i, sizeof(char), E10_UINT64, stdout);
    }
  } else {
    rc += printf("%" PRIu64, (uint64_t)big);
  }
  return rc;
}

int print_uint128_decimal1(__uint128_t big) {
  size_t rc = 0;
  
  if (big >> 64) {
    char buf[256] = {0};
    char buf_left[256] = {0};
    size_t pos = 254;
    char x = '\0';
    while (big / P10_UINT64) {
      size_t pre_pos = pos;
      x = buf[pre_pos];
      pos -= E10_UINT64;

      rc += sprintf(buf + pos, "%." TO_STRING(E10_UINT64) PRIu64, (uint64_t)(big % P10_UINT64));
      buf[pre_pos] = x;

      big /= P10_UINT64;
    }

    size_t left = sprintf(buf_left,"%" PRIu64, (uint64_t)big);
    //printf("\nleft %ld\n",left);

    size_t pre_pos = pos;
    x = buf[pre_pos];
    pos -= left;

    rc += sprintf(buf + pos, "%" PRIu64, (uint64_t)big);
    buf[pre_pos] = x;
    
    printf("%s",buf+pos);

  } else {
    rc += printf("%" PRIu64, (uint64_t)big);
  }
  return rc;
}



int main(void)
{
    uint128_t u128a = ((uint128_t)UINT64_MAX + 1) * 0x1234567890ABCDEFULL +
                      0xFEDCBA9876543210ULL;
    uint128_t u128b = ((uint128_t)UINT64_MAX + 1) * 0xF234567890ABCDEFULL +
                      0x1EDCBA987654320FULL;

    //24197857200151252746022455506638221840
    // uint128_t u128c = 1234545678910123458910ULL;


    // int ndigits = print_u128_u(u128a);
    // printf("\n%d digits\n", ndigits);

    // ndigits = print_uint128_decimal1(u128a);
    // printf("\n%d digits\n", ndigits);

    int ndigits = print_u128_u(u128b);
    printf("\n");
    // printf("\n%d digits\n", ndigits);
    //3 2194492825597240826 0334335944939549199
    ndigits = print_uint128_decimal1(u128b);
    printf("\n%d digits\n", ndigits);


    return(0);
}
