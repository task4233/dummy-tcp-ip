#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "../md5_src/global.h"
#include "../md5_src/md5.h"

typedef struct
{
  uint32_t type;
  uint32_t len;
  unsigned char digest[16];
} DTCP;

void calc_md5(unsigned char *data, unsigned int len, unsigned char digest[16])
{
  MD5_CTX context;
  MD5Init(&context);
  MD5Update(&context, data, len);
  MD5Final(digest, &context);
}

int isValid(DTCP* dtcp, unsigned char* data) {
  // interact inner_data
  // +1 is for '\0'
  unsigned char *inner_data = (unsigned char *)calloc(dtcp->len + 1, sizeof(unsigned char));
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

  memcpy(&dtcp->digest, data + 8, 16);
  printf("digest : ");
  show_hexdump(dtcp->digest, 16);
  if (!isValid(dtcp, &data[24])) {
    fprintf(stderr, "invalid checksum\n");
    free(dtcp);
    return (DTCP*)NULL;
  }

  puts("========================================================");

  return dtcp;
}
