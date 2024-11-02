/* fpont 8/99 */

/* 
 * input : IP address of a interface on the same network
 * output : MAC address of this interface if found if arp_cache
 *
 * OS tested : linux 2.2.9
 */

#include <stdlib.h>
#include <stdio.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
  struct sockaddr_in sin = { 0 };
  struct arpreq myarp = { { 0 } };
  int sockfd;
  unsigned char *ptr;
  
  if(argc!=2) {
    printf("usage: %s <IP address>\n",argv[0]);
    exit(0);
  }

  sin.sin_family = AF_INET;
  if(inet_aton(argv[1], &sin.sin_addr)==0) {
    printf("%s: IP address '%s' not valid\n",argv[0],argv[1]);
    exit(0);
  }
  
  
  memcpy(&myarp.arp_pa, &sin, sizeof myarp.arp_pa);         
  strcpy(myarp.arp_dev, "eth0");
  
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    printf("%s: cannot open socket\n",argv[0]);
    exit(0);
  }
  
  if (ioctl(sockfd, SIOCGARP, &myarp) == -1) {
    printf("%s: no entry in arp_cache for '%s'\n",argv[0],argv[1]);
    exit(0);
  }
  
  ptr = &myarp.arp_ha.sa_data[0];
  printf("%s: MAC address for '%s' is : ",argv[0],argv[1]);
  printf("%x:%x:%x:%x:%x:%x\n",*ptr, *(ptr+1),*(ptr+2),
	 *(ptr+3),*(ptr+4),*(ptr+5));
  
  return 1;
}
