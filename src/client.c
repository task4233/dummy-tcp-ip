#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>
#include "utils.h"

const uint BUF_SIZE = 1024;

int main(void)
{
  int sockfd ;
  int len ;
  struct sockaddr_un address ;
  int result ;

  char* ch = "\x1\x0\x0\x0\x2\x0\x0\x0\x3\x0\x0\x0\x4\x5\x6\x7\x8\x9\xa\xb\xc\xd\xe\xf\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1fhogefugapiyo\0";
  show_hexdump(ch); 

  char* recvCh = (char*)calloc(BUF_SIZE, sizeof(char));

  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  address.sun_family = AF_UNIX ;
  strcpy(address.sun_path , "server_socket");
  len = sizeof(address);

  result = connect(sockfd , (struct sockaddr *)&address , len);

  if ( result == -1 ) {
    perror("oops: client1");
    exit(1);
  }

  write(sockfd,&ch[0],BUF_SIZE);
  printf("char from server = %s \n",recvCh);
  close(sockfd);
  exit(0);
}
