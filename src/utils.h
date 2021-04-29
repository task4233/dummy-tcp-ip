#pragma once
#include <stdio.h>
#include <string.h>
#include "../md5_src/global.h"
#include "../md5_src/md5.h"

void show_hexdump(const unsigned char* data, const unsigned int len) {
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

void calc_md5(unsigned char *data, unsigned int len, unsigned char digest[16])
{
  MD5_CTX context;
  MD5Init(&context);
  MD5Update(&context, data, len);
  MD5Final(digest, &context);
}
