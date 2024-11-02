#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

#define SERVER_PORT 1500
#define MAX_MSG 100
int *arr1;
int vector_size;

int arr[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int arr2[20] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int worker_count = 4;
long sum = 0;
const char *SERVER_IP = "192.168.192.128";
const char *IP[4] = {"192.168.192.128", "10.2.70.224", "10.2.69.420", "10.30.65.666"};
void *distribute(void *arg)
{
  int worker_id = (*(int *)arg);
  int temp_sum = 0;
  int x = (*(int *)arg) * vector_size / worker_count;
  int y = ((*(int *)arg) + 1) * vector_size / worker_count;
  int temp_arr[10];
  for (int i = 0; i < 5; i++)
  {
    temp_arr[i] = arr[i + worker_id * 5];
  }
  for (int i = 5; i < 10; i++)
  {
    temp_arr[i] = arr2[worker_id * 5 + i % 5];
  }

  int sd, rc, i;
  struct sockaddr_in localAddr, servAddr;
  struct hostent *h;
  h = gethostbyname(IP[0]);
  if (h == NULL)
  {
    std::cerr << ": unknown host '" << SERVER_IP << "'\n";
    exit(1);
  }
  servAddr.sin_family = h->h_addrtype;
  memcpy((char *)&servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  servAddr.sin_port = htons(SERVER_PORT);
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if (sd < 0)
  {
    perror("cannot open socket ");
    exit(1);
  }
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(0);
  rc = bind(sd, (struct sockaddr *)&localAddr, sizeof(localAddr));
  if (rc < 0)
  {
    std::cerr << "cannot bind port TCP " << SERVER_PORT << "\n";
    perror("error ");
    exit(1);
  }
  rc = connect(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
  if (rc < 0)
  {
    perror("cannot connect ");
    exit(1);
  }
  rc = send(sd, &temp_arr, sizeof(temp_arr), 0);
  if (rc < 0)
  {
    perror("cannot send data ");
    close(sd);
    exit(1);
  }
  int received_data[1];
  rc = recv(sd, &received_data, sizeof(received_data), 0);
  if (rc < 0)
  {
    perror("cannot receive data ");
    close(sd);
    exit(1);
  }
  close(sd);

  pthread_mutex_lock(&mut);
  sum = sum + received_data[0];
  pthread_mutex_unlock(&mut);
  pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
  pthread_t threads[worker_count];
  int threadID[worker_count];
  for (int i = 0; i < worker_count; i++)
  {
    threadID[i] = i;
    pthread_create(&threads[i], NULL, distribute, (void *)&threadID[i]);
  }
  for (int i = 0; i < worker_count; i++)
  {
    pthread_join(threads[i], NULL);
  }
  std::cout << "Dot product = " << sum << "/";
  return 0;
}
