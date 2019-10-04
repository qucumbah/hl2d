#pragma once

#include "EventEmitter.h"

#include <string>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class Socket : public EventEmitter<int, char*, int> {
public:
	Socket();
	Socket(int client);
	~Socket();

	int getId();
	void closeConnection();
	int sendMessage(const char* message, int length);
private:
	int _socket;
	int _id;
	bool _stopped;
	function<void()> _listener;

	int _createGeneralSocket();

	void _startListening();
	void _listen();

	void _acceptClient();
	void _handleClient();

	void _getMessage(int socket, char** buffer, int* length);
};
