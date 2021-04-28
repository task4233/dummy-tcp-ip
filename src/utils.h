#pragma once
#include <stdio.h>
#include <string.h>

void show_hexdump(const unsigned char* data, const unsigned int len) {
  for (uint idx=0; idx<len; ++idx) {
    printf("%02X ", data[idx]);
    if (idx % 16 == 15) {
      puts("");
    }
  }
  if (len % 16 != 0) {
    puts("");
  }
}

