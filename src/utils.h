#pragma once
#include <stdio.h>
#include <string.h>

void show_hexdump(const char* data) {
  puts("[begin]");
  for (uint idx=0; data[idx] != EOF; ++idx) {
    printf("%02X ", data[idx]);
    if (idx % 16 == 15) {
      puts("");
    }
  }
  puts("");
  puts("[end]");
}

