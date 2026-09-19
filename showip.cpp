#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: ./showip hostname\n";
    return 1;
  }
  
  addrinfo hints{};
  addrinfo* results = nullptr;

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  int status = getaddrinfo(
      argv[1],
      nullptr,
      &hints,
      &results
      );

  if (status != 0) {
    std::cerr << "getaddrinfo: "
      << gai_strerror(status)
      << '\n';

    return 1;
  }

  char ipstr[INET6_ADDRSTRLEN];

  for (addrinfo* p = results; p != nullptr; p = p->ai_next) {
    void* address;

    if (p->ai_family == AF_INET) {
      auto* ipv4 = reinterpret_cast<sockaddr_in*>(p->ai_addr);

      address = &(ipv4->sin_addr);

      inet_ntop(AF_INET, address, ipstr, sizeof(ipstr));

      std::cout << "IPv4: " << ipstr << '\n';
    }
    else if (p->ai_family == AF_INET6) {
      auto* ipv6 = reinterpret_cast<sockaddr_in6*>(p->ai_addr);

      address = &(ipv6->sin6_addr);

      inet_ntop(AF_INET6, address, ipstr, sizeof(ipstr));

      std::cout << "IPv6: " << ipstr << '\n';
    }
  }

  freeaddrinfo(results);

  return 0;
}
