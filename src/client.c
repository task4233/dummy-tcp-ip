#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

const uint BUF_SIZE = 1024;

// build_payload builds payload
// if error occurs, return 1
int build_payload(uint protocol, char *file_name, char *payload, uint *len)
{
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "failed to open file: %s\n", file_name);
    return 1;
  }

  // ip(12) + (tcp(24) | udp(8))
  uint base_idx = 12 + (protocol == 6 ? 24 : 8);
  *len = base_idx;

  // write data
  char ch;
  while ((ch = fgetc(fp)) != EOF)
  {
    payload[*len++] = ch;
  }
  fclose(fp);

  // write digest in case of TCP
  if (payload == 6)
  {
    calc_md5(&payload[base_idx], *len - base_idx, &payload[12])
  }

  // set ip & tcp/udp information
  &payload[12] = 10;
  &payload[0] = protocol;
  &payload[4] = 1;
  &payload[8] = 0x40;

  show_hexdump(ch, *len);
  return 0;
}

// send_payload sends payload to server
// if error occurs, return 1
int send_payload(unsigned char *payload, uint len)
{
  int res;
  uint sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "server_socket");
  uint len = sizeof(addr);

  if (connect(sockfd, (struct sockaddr *)&addr, len) == -1)
  {
    fprintf(stderr, "failed to connect: fd%d\n", sockfd);
    close(sockfd);
    return 1;
  }

  if (write(sockfd, &ch[0], len) == -1)
  {
    fprintf(stderr, "failed to write: fd%d\n", sockfd);
    close(sockfd);
    return 1;
  }

  unsigned char *recvCh = (unsigned char *)calloc(BUF_SIZE, sizeof(unsigned char));
  uint read_len = read(sockfd, &recvCh[0], BUF_SIZE);
  if (read_len == -1) {
    fprintf(stderr, "failed to read response: fd%d", sockfd);
    close(sockfd);
    return 1;
  }

  show_hexdump(&recvCh[0], read_len);
  close(sockfd);

  return 0;
}

// client_call do client call
// if error occurs, return 1
int client_call(uint protocol, char *file_name)
{
  uint len;
  unsigned char *payload = (unsigned char *)calloc(BUF_SIZE, sizeof(unsigned char));
  if (build_payload(protocol, &file_name[0], &payload[0], &len))
  {
    fprintf(stderr, "failed to build payload\n");
    free(payload);
    return 1;
  }

  if (send_paylod(&payload[0], len))
  {
    fprintf(stderr, "failed to send payload\n");
    free(payload);
    return 1;
  }

  free(payload);
  return 0;
}

int main(void)
{
  int protocol;
  char* file_name = (char*)malloc(64); // it's enough, i think
  printf("input protocol(dtcp: 6, dudp: 17) => ");
  scanf("%d", &protocol);
  printf("input file name which you want to send => ");
  scanf("%s", file_name);

  if (client_call(protocol, &file_name[0])) {
    fprintf(stderr, "failed to client_call\n");
    free(file_name);
    return 1;
  }

  free(file_name);
  return 0;
}
