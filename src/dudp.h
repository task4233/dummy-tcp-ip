#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "utils.h"

typedef struct
{
  uint32_t type;
  uint32_t len;
  unsigned char *data; // the pointer to data in next layer
} DUDP;

// DUDPのデータをパースする関数
// dudpをfreeすることを忘れない
DUDP* unwrap_DUDP_Data(unsigned char* data)
{
  puts("========================DUDP============================");
  DUDP *dudp = (DUDP*)calloc(1, sizeof(DUDP));

  memcpy(&dudp->type, data, 4);
  printf("type   : %0d\n", dudp->type);

  memcpy(&dudp->len, data + 4, 4);
  printf("len    : %0d\n", dudp->len);

  puts("========================================================");
  return dudp;
}

void wrap_DUDP_Data(DUDP* dudp, unsigned char* ip_data) {
  ip_data[0] = dudp->type;
  ip_data[4] = dudp->len;
  memcpy(&ip_data[8], &dudp->data[0], dudp->len);
}