#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

const uint BUF_SIZE = 1024;

typedef struct
{
  uint32_t type;
  uint32_t version;
  uint32_t ttl;
} LayerDIP;

char* interpret_DIP_Data(char **data)
{
  printf("data\n: %s", *data);

  LayerDIP *dip = (LayerDIP*)calloc(1, sizeof(LayerDIP));
  char* buf = (char*)calloc(BUF_SIZE, sizeof(char));
  memcpy(buf, data, 4);
  printf("type: %d\n", dip->type);

  memcpy(&dip->version, data+4, 4);
  printf("version: %d\n", dip->version);

  memcpy(&dip->ttl, data+8, 4);
  printf("ttl: %d\n", dip->ttl);

  return *data;
}

int main(void)
{
  int server_sockfd, client_sockfd;
  int server_len, client_len;
  struct sockaddr_un server_address;
  struct sockaddr_un client_address;

  unlink("server_socket");
  server_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  server_address.sun_family = AF_UNIX;
  strcpy(server_address.sun_path, "server_socket");
  server_len = sizeof(server_address);
  bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

  listen(server_sockfd, 5);
  while(1)
  {
    char *ch = (char *)calloc(BUF_SIZE, sizeof(char));
    printf("server waiting\n");
    printf("%c\n", ch[0]);

    client_sockfd = accept(server_sockfd,
                           (struct sockaddr *)&client_address, &client_len);

    int read_len = read(client_sockfd, &ch[0], sizeof(ch));
    ch[read_len] = '\0';
    char* res = interpret_DIP_Data(&ch);
    printf("[servr] ch: %s\n", ch);
    write(client_sockfd, &ch[0], BUF_SIZE);
    close(client_sockfd);
  }
}
