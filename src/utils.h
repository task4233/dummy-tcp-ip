#pragma once
#include <stdio.h>
#include <string.h>
#include "../md5_src/global.h"
#include "../md5_src/md5.h"

// show_hexdump show len bytes of given data with hexadecimal format 
void show_hexdump(const unsigned char* data, const unsigned int len) {
  // printf("len: %d\n", len);
  for (unsigned int idx=0; idx<len; ++idx) {
    printf("%02X ", data[idx]);
    if (idx % 16 == 15) {
      puts("");
    }
  }
  if (len % 16 != 0) {
    puts("");
  }
}

// calc_md5 calculates md5 hash
// The hash will be assigned `digest`, 3rd argument
void calc_md5(unsigned char *data, unsigned int len, unsigned char *digest)
{
  MD5_CTX context;
  MD5Init(&context);
  MD5Update(&context, data, len);
  MD5Final(digest, &context);
}
