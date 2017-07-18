#include <iostream>
#include "udp_socket.h"

using namespace std;

int main(int argc, char **argv) {

	if (argc < 3) {
		printf("Usage: ./udp_recv <server_address> <port>\n");
		return 0;
	}
  
  char *ip_address = *(argv + 1);
  uint16_t port_number = atoi(*(argv + 2));
  

	UDPSocket udp_socket_object(ip_address, port_number);
  	

	uint16_t init_error = udp_socket_object.init();
	if (init_error) {
	  fprintf(stderr, "ERROR, initalizing socket data...!!\n");
    fprintf(stderr, "errno: %d\n", init_error);
		return 0;
	}

	uint16_t start_recv_error = udp_socket_object.start_receiving();
	if (start_recv_error) {
    fprintf(stderr, "ERROR, starting recv thread...!!!\n");
    fprintf(stderr, "errno: %d\n", start_recv_error);
		return 0;
	} else {
    puts("Receiving Packets...");
    pthread_join(udp_socket_object.get_recv_thread_id(), nullptr);
  }

  puts("Goodbye...!!");
	return 0;
}
