#include <iostream>
#include <sstream>
#include <WS2tcpip.h>
#include <string>
#include <chrono>
#include <stdlib.h> 
#include <thread>
#include <charconv>


#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);

	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096] = "33\r\n";
	srand(time(0));
	int random = rand() % 100;
	int number = 1234;
	char num_char[4096];
	ZeroMemory(num_char, 4096);
	std::to_chars(num_char, num_char + 4096, random);



	while (true)
	{
		// ZeroMemory(buf, 4096);

		// Echo message back to client

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		send(clientSocket, num_char,4096, 0);
		random = rand() % 100;
		ZeroMemory(num_char, 4096);
		std::to_chars(num_char, num_char + 4096, random);


	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}