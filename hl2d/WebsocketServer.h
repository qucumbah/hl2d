#pragma once

#include <vector>
#include "Socket.h"
#include "EventEmitter.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using std::string;

class WebsocketServer : public EventEmitter<int, string> {
public:
	WebsocketServer();
	~WebsocketServer();

	void removeClient(int clientSocket);

	void sendMessageAll(string message);
	void sendMessageAll(string message, int opcode);
	void sendMessage(string message, int clientId);
	void sendMessage(string message, int clientId, int opcode);
private:
	std::list<Socket*> _clients;

	void _sendCloseMessage(int clientId);
	void _sendCloseMessageAll();

	char* _encodeMessage(string message, int opcode);
	size_t _getEncodedMessageLength(string message);

	void _acceptClient(int clientSocket, char* request, int length);
	string _generateWsHandshakeResponse(char* request);

	Socket* _addClient(int clientSocket);

	void _acceptMessage(int clientSocket, char* request, int length);
};
