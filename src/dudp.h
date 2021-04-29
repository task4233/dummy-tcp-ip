#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "utils.h"

typedef struct
{
  uint32_t type;
  uint32_t len;
  unsigned char *data; // the pointer to data in next layer
} DUDP;

int unwrap_DUDP_Data(unsigned char* data, DUDP* dudp)
{
  puts("========================DUDP============================");
  memcpy(&dudp->type, data, 4);
  printf("type   : %0d\n", dudp->type);

  memcpy(&dudp->len, data + 4, 4);
  printf("len    : %0d\n", dudp->len);

  write(1, "RAWDATA: ", 9);
  write(1, &data[8], dudp->len);
  puts("========================================================");

  return 0;
}

void wrap_DUDP_Data(DUDP* dudp, unsigned char* ip_data) {
  memcpyUint32(&ip_data[0], dudp->type);
  memcpyUint32(&ip_data[4], dudp->len);
  memcpy(&ip_data[8], &dudp->data[0], dudp->len);
}