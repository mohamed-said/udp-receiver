#include "udp_socket.h"

UDPSocket::UDPSocket(char *p_server_name, uint16_t p_udp_port_number) {
  server_name = new char[100];
  strcpy(server_name, p_server_name);
  udp_port_number = p_udp_port_number;
  stop = false;
}

UDPSocket::~UDPSocket() {
  delete server_name;
  server_name = nullptr;
}

int UDPSocket::init() {
  udp_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (udp_socket_fd < 0) {
    fprintf(stderr, "ERROR, creating UDP socket..!!!\n");
    return errno;
  }
  
  struct timeval recv_timeout;
  recv_timeout.tv_sec = 0;
  recv_timeout.tv_usec = 10;
  setsockopt(udp_socket_fd, SOL_SOCKET, SO_RCVTIMEO, &recv_timeout, sizeof(recv_timeout));

  memset(&udp_socket_data, 0, sizeof(udp_socket_data));
  memset(&sender_udp_socket_data, 0, sizeof(sender_udp_socket_data));

  udp_socket_data.sin_port = htons(udp_port_number);
  udp_socket_data.sin_family = AF_INET;
  udp_socket_data.sin_addr.s_addr = INADDR_ANY;

  uint16_t bind_error = bind(udp_socket_fd, (sockaddr*) &udp_socket_data, sizeof(udp_socket_data));
  if (bind_error == -1) {
    fprintf(stderr, "ERROR, binding socket to address...!!!\n");
    return errno;
  }

  return 0;
}

void *UDPSocket::run_recv_thread(UDPSocket *__sock_obj) {
  char message_buffer[65];
  socklen_t socket_length = sizeof(__sock_obj->udp_socket_data);
  while (!__sock_obj->get_stop_status()) {
    memset(message_buffer, 0, sizeof(message_buffer));
    uint16_t recv_error = recvfrom(__sock_obj->udp_socket_fd, message_buffer, 
        64, MSG_CONFIRM, 
        (sockaddr*) &__sock_obj->sender_udp_socket_data, 
        &socket_length);
    if (recv_error == -1) {
      fprintf(stderr, " ** ERROR, receiving message from peer..!!\n");
      printf(" ** Error Number: %d\n", errno);
    } else {
      if (!strcmp(message_buffer, "")) {
        continue;
      } else {
        printf("%s: %s", inet_ntoa(__sock_obj->sender_udp_socket_data.sin_addr), message_buffer);
      }
    }
  }
}


int UDPSocket::start_receiving() {
  uint16_t thread_error = pthread_create(&recv_thread_id, nullptr, 
      (void*(*)(void*))run_recv_thread, this);
  if (thread_error) {
    fprintf(stderr, "ERROR, creating udp thread..!!\n");
    return -1;	
  }
  return 0;
}

bool UDPSocket::get_stop_status() {
  return stop;
}

void UDPSocket::stop_receiving() {
  stop = true;
}

pthread_t UDPSocket::get_recv_thread_id() {
  return recv_thread_id;
}
