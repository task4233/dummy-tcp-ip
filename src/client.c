#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "dtcp.h"
#include "dudp.h"
#include "dip.h"

const unsigned int MAX_BUF_SIZE = 1024;

// read_file reads data in file_name, the data is written in data
// if failed to open file, return 0
// if not, returns read length
int read_file(char *file_name, unsigned char *data)
{
  FILE *fp = fopen(file_name, "r");
  if (fp == NULL)
  {
    fprintf(stderr, "failed to open file: %s\n", file_name);
    return 0;
  }

  char ch;
  unsigned int size = 0;
  while ((ch = fgetc(fp)) != EOF)
  {
    data[size++] = ch;
  }
  fclose(fp);
  return size;
}

// build_payload builds payload
// if error occurs, return 1
int build_payload(unsigned int protocol, char *file_name, unsigned char *payload, unsigned int *len)
{
  puts("[BEGIN] build_payload");

  int res;
  unsigned char *raw_data = (unsigned char *)calloc(MAX_BUF_SIZE, sizeof(unsigned char));
  uint raw_data_size = read_file(&file_name[0], &raw_data[0]);
  if (raw_data_size == 0) {
    fprintf(stderr, "failed to read file\n");
    return 1;
  }

  unsigned int ip_data_size = raw_data_size;

  unsigned char *ip_data = (unsigned char *)calloc(MAX_BUF_SIZE, sizeof(unsigned char));
  switch (protocol)
  {
  case 6: // DTCP
  {
    DTCP *dtcp = (DTCP *)malloc(sizeof(DTCP));
    dtcp->type = 10;
    dtcp->len = raw_data_size;
    dtcp->data = &raw_data[0];
    write(1, &raw_data[0], raw_data_size);
    show_hexdump(&raw_data[0], raw_data_size);
    calc_md5(&raw_data[0], raw_data_size, &dtcp->digest[0]);
    res = wrap_DTCP_Data(&dtcp[0], &ip_data[0]);
    free(dtcp);
    if (res) {
      fprintf(stderr, "failed to wrap dtcp data\n");
        free(raw_data);
      return 1;
    }

    ip_data_size += 24;
    break;
  }
  case 17: // DUDP
  {
    DUDP *dudp = (DUDP *)malloc(sizeof(DUDP));
    dudp->type = 10;
    dudp->len = raw_data_size;
    dudp->data = &raw_data[0];
    res = wrap_DUDP_Data(&dudp[0], &ip_data[0]);
    free(dudp);

    if (res) {
      fprintf(stderr, "failed to wrap dudp data\n");
        free(raw_data);
      return 1;
    }

    ip_data_size += 8;
    break;
  }
  default:
    fprintf(stderr, "invalid protocol: %d\n", protocol);
      free(raw_data);
      return 1;
  }
  free(raw_data);

  // DIP
  DIP *dip = (DIP *)malloc(sizeof(DIP));
  dip->version = 1;
  dip->ttl = 0x40;
  dip->type = protocol;
  dip->data = &ip_data[0];
  res = wrap_DIP_Data(&dip[0], &payload[0], ip_data_size);
  free(dip);
  free(ip_data);

  if (res) {
    fprintf(stderr, "failed to wrap dip data\n");
    return 1;
  }

  *len = ip_data_size + 12;
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

  unsigned char *recvCh = (unsigned char *)calloc(MAX_BUF_SIZE, sizeof(unsigned char));
  unsigned int read_len = read(sockfd, &recvCh[0], MAX_BUF_SIZE);
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
  unsigned int len = MAX_BUF_SIZE;
  unsigned char *payload = (unsigned char *)calloc(MAX_BUF_SIZE, sizeof(unsigned char));
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

int main(int argc, char* argv[])
{
  if (argc < 3) {
    fprintf(stderr, "./client protocol_number file_name");
    return 1;
  }

  int protocol = atoi(argv[1]);
  char *file_name = argv[2];
  if (client_call(protocol, &file_name[0]))
  {
    fprintf(stderr, "failed to client_call\n");
    return 1;
  }
  return 0;
}
