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
  int result;

  unsigned char* ch = "\x4\x0\x0\x0\x2\x0\x0\x0\x3\x0\x0\x0\xa\x0\x0\x0\xc\x0\x0\x0\xda\x77\x62\x0e\xbd\xfb\x99\xd6\x65\x4b\x93\xae\x7f\x26\x4f\x67hogefugapiyo\0";
  show_hexdump(ch, 37); 

  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  address.sun_family = AF_UNIX ;
  strcpy(address.sun_path , "server_socket");
  len = sizeof(address);

  result = connect(sockfd , (struct sockaddr *)&address , len);
  if ( result == -1 ) {
    perror("oops: client1");
    exit(1);
	}

  uint size = write(sockfd,&ch[0],BUF_SIZE);
	printf("[client] write size: %d\n", size);

  unsigned char* recvCh = (unsigned char*)calloc(BUF_SIZE, sizeof(unsigned char));
	read(sockfd, &recvCh[0], BUF_SIZE);
	// printf("char from server = %s \n",recvCh);
  close(sockfd);
  exit(0);
}
