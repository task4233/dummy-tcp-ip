#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

const unsigned int BUF_SIZE = 1024;

// build_payload builds payload
// if error occurs, return 1
int build_payload(unsigned int protocol, char *file_name, unsigned char *payload, unsigned int *len)
{
  puts("[BEGIN] build_payload");
  // printf("with: (%d) %s\n", protocol, file_name);
  // printf("with: (%d) %s\n", *len, payload);

  FILE *fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "failed to open file: %s\n", file_name);
    return 1;
  }

  // ip(12) + (tcp(24) | udp(8))
  unsigned int base_idx = 12 + (protocol == 6 ? 24 : 8);
  *len = base_idx;

  // write data
  char ch;
  while ((ch = fgetc(fp)) != EOF)
  {
    payload[(*len)++] = ch;
  }
  fclose(fp);

  // write digest in case of TCP
  if (protocol == 6)
  {
    // show_hexdump(&payload[base_idx], *len - base_idx);
    calc_md5(&payload[base_idx], *len - base_idx, &payload[20]);
  }

  // set ip & tcp/udp information
  payload[16] = *len - base_idx;
  payload[12] = 10;
  payload[0] = protocol;
  payload[4] = 1;
  payload[8] = 0x40;

  puts("payload: ");
  show_hexdump(&payload[0], *len);
  puts("[END] build_payload");
  return 0;
}

// send_payload sends payload to server
// if error occurs, return 1
int send_payload(unsigned char *payload, unsigned int len)
{
  puts("[BEGIN] send_payload");
  // printf("with: (%d), %s\n", len, payload);
  
  int res;
  unsigned int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strcpy(addr.sun_path, "server_socket");
  unsigned int addr_len = sizeof(addr);

  if (connect(sockfd, (struct sockaddr *)&addr, addr_len) == -1)
  {
    fprintf(stderr, "failed to connect: fd%d\n", sockfd);
    close(sockfd);
    return 1;
  }

  if (write(sockfd, &payload[0], len) == -1)
  {
    fprintf(stderr, "failed to write: fd%d\n", sockfd);
    close(sockfd);
    return 1;
  }

  unsigned char *recvCh = (unsigned char *)calloc(BUF_SIZE, sizeof(unsigned char));
  unsigned int read_len = read(sockfd, &recvCh[0], BUF_SIZE);
  if (read_len == -1)
  {
    fprintf(stderr, "failed to read response: fd%d", sockfd);
    close(sockfd);
    return 1;
  }

  puts("recv data: ");
  show_hexdump(&recvCh[0], read_len);
  close(sockfd);
  puts("[END] send_payload");
  return 0;
}

// client_call do client call
// if error occurs, return 1
int client_call(unsigned int protocol, char *file_name)
{
  puts("[BEGIN] client_call");
  // printf("with: (%d), %s\n", protocol, file_name);
  unsigned int len = BUF_SIZE;
  unsigned char *payload = (unsigned char *)calloc(BUF_SIZE, sizeof(unsigned char));
  if (build_payload(protocol, &file_name[0], &payload[0], &len))
  {
    fprintf(stderr, "failed to build payload\n");
    free(payload);
    return 1;
  }

  if (send_payload(&payload[0], len))
  {
    fprintf(stderr, "failed to send payload\n");
    free(payload);
    return 1;
  }

  free(payload);
  puts("[END] client_call");
  return 0;
}

int main(void)
{
  int protocol;
  char *file_name = (char *)calloc(64, sizeof(char)); // it's enough, i think
  printf("input protocol(dtcp: 6, dudp: 17) => ");
  // scanf("%d", &protocol);
  printf("input file name which you want to send => ");
  // scanf("%s", file_name);

  protocol = 6;
  strcpy(file_name, "build.sh");
  printf("set: (%d), %s\n", protocol, file_name);

  if (client_call(protocol, &file_name[0]))
  {
    fprintf(stderr, "failed to client_call\n");
    free(file_name);
    return 1;
  }

  free(file_name);
  return 0;
}
