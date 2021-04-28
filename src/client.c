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

  unsigned char* ch = "\x6\x0\x0\x0\x1\x0\x0\x0\x40\x0\x0\x0\xa\x0\x0\x0\xc\x0\x0\x0\xda\x77\x62\x0e\xbd\xfb\x99\xd6\x65\x4b\x93\xae\x7f\x26\x4f\x67hogefugapiyo\0";
  puts("[client]=>[server]");
  show_hexdump(ch, 50); 

  sockfd = socket(AF_UNIX,SOCK_STREAM,0);

  address.sun_family = AF_UNIX ;
  strcpy(address.sun_path , "server_socket");
  len = sizeof(address);

  result = connect(sockfd , (struct sockaddr *)&address , len);
  if ( result == -1 ) {
    perror("oops: client1");
    exit(1);
	}

  uint size = write(sockfd,&ch[0],50);

  unsigned char* recvCh = (unsigned char*)calloc(BUF_SIZE, sizeof(unsigned char));
	uint read_len = read(sockfd, &recvCh[0], BUF_SIZE);
	// puts("[client]");
  // show_hexdump(&recvCh[0], read_len);
  close(sockfd);
  exit(0);
}
