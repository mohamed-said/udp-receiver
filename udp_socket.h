#ifndef UDP_SOCKET
#define UDP_SOCKET

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

class UDPSocket {

private:
	int udp_socket_fd;
	uint16_t udp_port_number;
	struct sockaddr_in udp_socket_data;
	struct sockaddr_in sender_udp_socket_data;
	char *server_name;	
	pthread_t recv_thread_id;
	static void *run_recv_thread(UDPSocket*);	
  bool stop;
public:
	
	UDPSocket(char*, uint16_t);
  ~UDPSocket();
	int init();
	int start_receiving();
  bool get_stop_status();
  void stop_receiving();
  pthread_t get_recv_thread_id();
};

#endif
