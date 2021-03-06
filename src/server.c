#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"
#include "dip.h"

const unsigned int MAX_BUF_SIZE = 1024;

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
    unsigned char *ch = (unsigned char *)calloc(MAX_BUF_SIZE, sizeof(unsigned char));
    printf("server waiting\n");

    client_sockfd = accept(server_sockfd,
                           (struct sockaddr *)&client_address, &client_len);

    int read_len = read(client_sockfd, &ch[0], MAX_BUF_SIZE); 
    puts("[server]");
    DIP* dip = (DIP*)malloc(sizeof(DIP));
    unwrap_DIP_Data(&ch[0], &dip[0]);
    free(dip);
    
    write(client_sockfd, &ch[0], read_len);
    close(client_sockfd);
  }
}
