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
  unsigned char digest[16];
  unsigned char* data; // the pointer to data in next layer
} DTCP;

int isValid(DTCP* dtcp, unsigned char* data) {
  // interact inner_data
  // +1 is for '\0'
  unsigned char *inner_data = (unsigned char *)calloc(dtcp->len, sizeof(unsigned char));
  memcpy(inner_data, data, dtcp->len);
  // printf("RAW: %s\n", inner_data);

  unsigned char *real_digest = (unsigned char*)calloc(16, sizeof(unsigned char));
  calc_md5(&inner_data[0], dtcp->len, &real_digest[0]);
  printf("actual : ");
  show_hexdump(real_digest, 16);

  int res = strncmp(dtcp->digest, real_digest, 16) == 0;
  free(real_digest);
  free(inner_data);
  return res;
}

// freeするのを忘れないこと
DTCP* unwrap_DTCP_Data(unsigned char *data)
{
  puts("========================DTCP============================");
  DTCP *dtcp = (DTCP *)calloc(1, sizeof(DTCP));

  memcpy(&dtcp->type, data, 4);
  printf("type   : %0d\n", dtcp->type);

  memcpy(&dtcp->len, data + 4, 4);
  printf("len    : %0d\n", dtcp->len);

  printf("data   :\n");
	show_hexdump(&data[24], dtcp->len);

  memcpy(&dtcp->digest, data + 8, 16);
  printf("digest : ");
  show_hexdump(dtcp->digest, 16);
  if (!isValid(dtcp, &data[24])) {
    fprintf(stderr, "invalid checksum\n");
    free(dtcp);
    return (DTCP*)NULL;
  }
  puts("========================================================");

  write(1, "RAWDATA: ", 9);
  write(1, &data[24], dtcp->len);
  puts("");

  return dtcp;
}

void wrap_DTCP_Data(DTCP* dtcp, unsigned char* ip_data) {
  memcpyUint32(&ip_data[0], dtcp->type);
  memcpyUint32(&ip_data[4], dtcp->len);
  memcpy(&ip_data[8], &dtcp->digest[0], 16);
  memcpy(&ip_data[24], &dtcp->data[0], dtcp->len); 
}