// gameplatform.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
using namespace std;


int main(int argc, char** argv)
{
	cout << "argc=" << argc << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << "argv[" << i << "]= " << argv[i] << endl;
	}

	const WORD socketVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(socketVersion, &wsaData) != 0)
	{
		cerr << "socket startup error!\n";
		return -1;
	}

	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//TCP
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error!\n");
		return 0;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(HOST_MESSAGE_PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (const sockaddr*)&sin, (int)sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error!\n");
		return -1;
	}
	if (listen(slisten, 1))
	{
		printf("listen error!\n");
		return -1;
	}
	threadpool poo{ MAXCLIENT };

	SOCKET sclient;
	int len = sizeof(SOCKADDR);
	sockaddr_in clientaddr;
	in_addr clientadd;

	while (true)
	{
		if (poo.idlCount == 0)
		{
			_sleep(10000);
			continue;
		}
		sclient = accept(slisten, (sockaddr*)&clientaddr, &len);
		clientadd.S_un.S_addr = ntohl(clientaddr.sin_addr.S_un.S_addr);
		printf("Receive a connection from %s\n", inet_ntoa(clientadd));
		poo.commit(clientcontrol, sclient);
	}


	system("pause");
	return 0;
}