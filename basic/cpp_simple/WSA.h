#pragma once
#ifdef _WIN32
#include <Winsock2.h>
#endif /* _WIN32 */

#include <stdexcept>
#pragma comment(lib,"Ws2_32.lib")

struct WSA {
	WSA() {
		WORD version = MAKEWORD(2, 2);
		WSADATA wsaData = { 0 };
		if (WSAStartup(version, &wsaData) != 0) {
			throw std::runtime_error("WSAStartup() failed");
		}
	}
	~WSA() {
		try {
			WSACleanup();
		}
		catch (...) {}
	}
};