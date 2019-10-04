#include "Socket.h"
#include <thread>
#include <iostream>
#include <functional>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;
using std::thread;
using std::function;

Socket::Socket() {
	//Create general socket
	_socket = _createGeneralSocket();
	_listener = std::bind(&Socket::_acceptClient, this);
	_id = -1;

	_startListening();
}

Socket::Socket(int client) {
	//Create client listener socket
	_socket = client;
	_listener = std::bind(&Socket::_handleClient, this);
	_id = client;

	_startListening();
}

Socket::~Socket() {

}

int Socket::getId() {
	return _id;
}

void Socket::closeConnection() {
	_stopped = true;
}

int Socket::sendMessage(const char* message, int length) {
	int result = send(_socket, message, length, 0);

	if (result == SOCKET_ERROR) {
		cerr << "Socket error while sending message" << endl;
		closesocket(_socket);
		return SOCKET_ERROR;
	}

	return 0;
}

int Socket::_createGeneralSocket() {
	struct addrinfo* addr = 0;

	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int result = getaddrinfo(0, "8013", &hints, &addr);
	if (result != 0) {
		cerr << "getaddrinfo failed: " << result << endl;
		throw result;
	}

	int sock = socket(
		addr->ai_family,
		addr->ai_socktype,
		addr->ai_protocol
	);
	if (sock == INVALID_SOCKET) {
		cerr << "Error at socket: " << WSAGetLastError() << endl;
		throw sock;
	}

	result = bind(sock, addr->ai_addr, (int)addr->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cerr << "bind failed with error: " << WSAGetLastError() << endl;
		throw result;
	}

	result = listen(sock, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		cerr << "listen failed with error: " << WSAGetLastError() << endl;
		throw result;
	}

	return sock;
}

//Quality code
void Socket::_startListening() {
	thread t(std::bind(&Socket::_listen, this));
	t.detach();
}

void Socket::_listen() {
	while (true) {
		if (_stopped) {
			return;
		}

		_listener();
	}
}

void Socket::_acceptClient() {
	struct sockaddr* addr = 0;
	int* addrLen = 0;
	int clientSocket = accept(_socket, addr, addrLen);

	if (clientSocket == INVALID_SOCKET) {
		cerr << "accept failed: " << WSAGetLastError() << endl;
		throw clientSocket;
	}

	char* message;
	int length;

	_getMessage(clientSocket, &message, &length);

	_emit("accept", clientSocket, message, length);
}

void Socket::_handleClient() {
	char* message;
	int length;

	_getMessage(_socket, &message, &length);

	_emit("message", _socket, message, length);
}

void Socket::_getMessage(int socket, char** buffer, int* length) {
	const int SIZE = 2048;
	char* request = new char[SIZE];

	int result = recv(socket, request, SIZE, 0);

	if (result == SOCKET_ERROR) {
		cerr << "recv failed: " << result << endl;
		throw result;
	}
	request[result] = '\0';

	buffer[0] = request;
	length[0] = result;
}