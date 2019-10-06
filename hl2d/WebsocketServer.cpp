#include "WebsocketServer.h"
#include "util.h"
#include <iostream>
#include <thread>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

WebsocketServer::WebsocketServer() {
	Socket* generalSocket = new Socket();
	//function<void(int, string)> acceptHandler = std::bind(&WebsocketServer::_acceptClient, this);
	//generalSocket->on("accept", acceptHandler);
	
	function<void(int, char*, int)> acceptHandler =
	[this](int clientSocket, char* message, int length) {

		this->_acceptClient(clientSocket, message, length);
	};
	generalSocket->on("accept", acceptHandler);
	
	cout << "Server started" << endl;
}

WebsocketServer::~WebsocketServer() {
	//Send close connection frame to every client
	for (Socket* clientSocket : _clients) {
		cout << "Closing connection with " << clientSocket->getId() << endl;
	}
	_sendCloseMessageAll();

	cout << "Server stopped" << endl;
}

void WebsocketServer::removeClient(int clientSocket) {
	_clients.remove_if([clientSocket](Socket* element) {
		bool equals = element->getId() == clientSocket;
		if (equals) {
			element->closeConnection();
		}
		return equals;
	});

	_emit("clientDisconnect", clientSocket, "");
}

//Argument order is weird for historical reasons (means I'm too lazy to fix it)
void WebsocketServer::sendMessageAll(string messageString) {
	sendMessageAll(messageString, 0x1);
}
void WebsocketServer::sendMessageAll(string messageString, int opcode) {
	char* message = _encodeMessage(messageString, opcode);
	size_t messageLength = _getEncodedMessageLength(messageString);
	for (Socket* clientSocket : _clients) {
		clientSocket->sendMessage(message, messageLength);
	}
}

void WebsocketServer::sendMessage(string messageString, int clientId) {
	sendMessage(messageString, clientId, 0x1);
}
void WebsocketServer::sendMessage(string messageString, int clientId, int opcode) {
	char* message = _encodeMessage(messageString, opcode);
	size_t messageLength = _getEncodedMessageLength(messageString);
	for (Socket* clientSocket : _clients) {
		if (clientSocket->getId() == clientId) {
			clientSocket->sendMessage(message, messageLength);

			/*
			cout << messageLength << endl;
			for (int i = 0; i < messageLength; i++) {
				cout << (int)message[i] << " ";
			}
			*/
		}
	}
}

void WebsocketServer::_sendCloseMessage(int clientId) {
	char* closeCode1000 = new char[3] {3, -24, 0};
	sendMessage(closeCode1000, clientId, 0x8);
}

void WebsocketServer::_sendCloseMessageAll() {
	char* closeCode1000 = new char[3] {3, -24, 0};
	sendMessageAll(closeCode1000, 0x8);
}

char* WebsocketServer::_encodeMessage(string message, int opcode) {
	char* result;
	uint64_t messageSize = message.size();
	if (messageSize < 126) {
		result = new char[messageSize + 2];
		result[1] = messageSize;

		const char* messageData = message.data();
		for (size_t i = 0; i < messageSize; i++) {
			result[i + 2] = messageData[i];
		}
	}
	else if (messageSize < 65535) {
		result = new char[messageSize + 4];
		result[1] = 126;
		result[2] = messageSize >> 8 & 0xff;
		result[3] = messageSize >> 0 & 0xff;

		const char* messageData = message.data();
		for (size_t i = 0; i < messageSize; i++) {
			result[i + 4] = messageData[i];
		}
	}
	else {
		result = new char[messageSize + 10];
		result[1] = 127;
		
		for (int i = 0; i < 8; i++) {
			result[2 + i] = messageSize >> (56 - i*8) & 0xff;
		}

		const char* messageData = message.data();
		for (size_t i = 0; i < messageSize; i++) {
			result[i + 10] = messageData[i];
		}
	}
	result[0] = 0x80 | opcode;

	return result;
}

size_t WebsocketServer::_getEncodedMessageLength(string message) {
	uint64_t messageSize = message.size();
	if (messageSize < 126) {
		return messageSize + 2;
	}
	else if (messageSize < 65535) {
		return messageSize + 4;
	}
	else {
		return messageSize + 10;
	}
}

void WebsocketServer::_acceptClient(int clientSocket, char* request, int length) {
	string response = _generateWsHandshakeResponse(request);

	//generateWsHandshakeResponse returns "" if there was an error
	if (response.size() == 0) {
		removeClient(clientSocket);
		return;
	}

	Socket* clientSocketWrapper = _addClient(clientSocket);
	int result =
		clientSocketWrapper->sendMessage(response.data(), response.length());

	if (result == SOCKET_ERROR) {
		cerr << "Couldn't send handshake response" << endl;
		removeClient(clientSocket);
		return;
	}

	_emit("clientConnect", clientSocket, "");
}

string WebsocketServer::_generateWsHandshakeResponse(char* request) {
	//const char* request = requestStr.data();
	if (!strstr(request, "GET /ws")) {
		cerr << "Wrong handshake signature:" << endl;
		cout << request << endl;
		return "";
	}

	//Handle handshake
	const char* wsKeyHeader = strstr(request, "Sec-WebSocket-Key: ");
	if (wsKeyHeader == 0) {
		cerr << "Incorrect request:" << endl;
		cout << request << endl;
		return "";
	}

	//Get 24-character key that starts at index 19 of key header
	char* wsKey = new char[61]{ 0 };
	for (int i = 0; i < 24; i++) {
		wsKey[i] = wsKeyHeader[19 + i];
	}

	//Append guid
	char GUID[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	strcat_s(wsKey, 61, GUID);

	//Apply SHA-1
	string hashed = util::SHA1(wsKey);
	//Encode with Base64
	string encoded = util::base64Encode(hashed);

	string response =
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: ";

	for (unsigned int i = 0; i < encoded.length(); i++) {
		response.push_back(encoded[i]);
	}

	response += "\r\n\r\n";

	return response;
}

Socket* WebsocketServer::_addClient(int clientSocket) {
	Socket* clientSocketWrapper = new Socket(clientSocket);
	function<void(int, char*, int)> messageHandler =
		[this](int clientSocket, char* message, int length) {
		this->_acceptMessage(clientSocket, message, length);
	};
	clientSocketWrapper->on("message", messageHandler);
	_clients.push_back(clientSocketWrapper);

	cout << "New connection: " << clientSocket << endl;
	return clientSocketWrapper;
}

void WebsocketServer::_acceptMessage(int clientSocket, char* request, int length) {
	//cout << "accepting message from " << clientSocket << ":" << endl;
	
	//const char* request = requestStr.data();
	/*
	cout << endl << endl;
	for (int i = 0; i < sizeof(request); i++) {
		cout << (int)request[i] << " ";
	}
	cout << endl << endl;
	*/
	char opcode = request[0] & 0xf;
	if (opcode == 0x8) {
		cerr << "Client " << clientSocket << " closed the connection" << endl;
		removeClient(clientSocket);
		_emit("clientDisconnect", clientSocket, "");
		return;
	}

	bool fin = (request[0] >> 8) & 1;
	if (!fin) {
		//Dont support fragmented messages
		cerr << "Client tried to send fragmented frame" << endl;
		removeClient(clientSocket);
		return;
	}

	bool hasMask = (request[1] >> 8) & 1;
	if (!hasMask) {
		cerr << "Client message didn't have mask" << endl;
		return;
	}

	unsigned int bodyLength = request[1] & 0x7f;
	char* body;
	if (bodyLength < 126) {
		body = new char[bodyLength + 1]{ 0 };

		char mask[4];
		for (int i = 0; i < 4; i++) {
			mask[i] = request[2 + i];
		}

		for (unsigned int i = 0; i < bodyLength; i++) {
			body[i] = request[6 + i] ^ mask[i % 4];
		}
	}
	else if (bodyLength == 126) {
		//length==126 is a code that means that the real length is in range
		//[126; 65535]. Actual number is contained in bytes 2 and 3

		bodyLength = (((unsigned char)request[2]) << 8) + (unsigned char)request[3];
		cout << "Body length: " << bodyLength << endl;
		cout << (int)request[2] << "," << (int)request[3] << endl;
		body = new char[bodyLength + 1]{ 0 };

		char mask[4];
		for (int i = 0; i < 4; i++) {
			mask[i] = request[4 + i];
		}

		for (unsigned int i = 0; i < bodyLength; i++) {
			body[i] = request[8 + i] ^ mask[i % 4];
		}
	}
	else if (bodyLength == 127) {
		//Real length is greater than 65536; Actual number is in bytes 2..9
		//Dont support this length
		cerr << "Client's message is longer than 65535" << endl;
		return;
	}
	else {
		//Unknown length
		cerr << "Client's message has incorrect length" << endl;
		return;
	}

	switch (opcode) {
	case 0x1:
		//Text data
		//cout << "Body: " << body << endl;
		/*
		for (int i = 0; i < requestStr.size(); i++) {
			cout << (int)request[i] << " ";
		}
		cout << endl;
		*/
		/*
		for (int i = 0; i < bodyLength; i++) {
			cout << (int)body[i] << " ";
		}
		cout << endl;
		*/
		break;
	case 0x2:
		//Binary data
		break;
	case 0x8:
		//Close connection
		break;
	case 0xa:
		//Pong
		break;
	default:
		//Reserved, ping or fragmented frame requests
		break;
	}

	/*
	sendMessage("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", clientSocket);
	*/

	_emit("clientMessage", clientSocket, body);
}
