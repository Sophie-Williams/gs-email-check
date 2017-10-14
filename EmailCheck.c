/*
	Email Check for GameSpy
	(C) 2017 Arves100
*/

#define _CRT_SECURE_NO_WARNINGS
#include "Common.h"
#include "Socket.h"

SOCKET sock = INVALID_SOCKET;

#define GPSP_IP "gpsp.gamespy.com"
#define GPSP_PORT 29901

int main(int argc, char **argv)
{
	char *email = 0, response[GAMESPY_BUFLEN] = { 0 };
	char *send1 = "\\valid\\\\email\\", *send2 = "\\partnerid\\0\\gamename\\gslive\\final\\", send[110] = { 0 } ;
	int RecvOut = 0;
	
	if (argc < 2)
	{
		printf("Usage:\n%s <email>\n", argv[0]);
		return 0;
	}
	
	email = argv[1];
	printf("Checking Email: %s\n", email);

	if (!socket_init())
		return 1;
	
	printf("Connecting...\n");
	if (!socket_connect(GPSP_IP, GPSP_PORT, &sock))
		return 1;
	printf("Connected!\n");
	
	strcpy_s(send, _countof(send), send1);
	strcat(send, email);
	strcat(send, send2);
	
	printf("Sending data...\n");
	if (!socket_send(sock, send, (unsigned)(strlen(send) + 1)))
		return 1;
	
	printf("Receiving data...\n");
	if (!socket_recv(sock, response, &RecvOut))
		return 1;
	
	if (RecvOut <= 0)
		return 1;

	if (strcmp(response, "\\vr\\1\\final\\") == 0)
	{
		printf("Email is valid!\n");
	}
	else if (strcmp(response, "\\vr\\0\\final\\") == 0)
	{
		printf("Email is not valid!\n");
	}
	else
	{
		printf("Unknown response: %s\n", response);
	}
	
	shutdown(sock, SB_BOTH);
	closesocket(sock);
	
	socket_destroy();
	
	return 0;
}