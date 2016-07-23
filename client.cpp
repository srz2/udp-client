#include <iostream>		//Main IO Functions
#include <string.h>		//Enables usage of bzero
#include <stdio.h>		//Enables printf()

#include <sys/socket.h>		//Socket Functions
#include <sys/types.h>
#include <netinet/in.h>		//Enables usage of struct sockaddr_in and setsocketopt
#include <arpa/inet.h>		//Enables inet_addr
#include <unistd.h>		//Makes closing socket possible
#include <cstdlib>		//Makes exit possible
#include <errno.h>		//Enables errno

using namespace std;

#define BUF_SIZE	100

int main()
{
	int sock = -1;
	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		cout << "Cannot create socket" << endl;
		exit(1);
	}

	struct sockaddr_in local;
	bzero(&local, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	local.sin_port = htons(1234);

	struct ip_mreq mreq;
	bzero(&mreq, sizeof(mreq));
	mreq.imr_multiaddr.s_addr=inet_addr("238.0.0.1");
	mreq.imr_interface.s_addr=htonl(INADDR_ANY);

	int optValue = 1;
	if(setsockopt(sock, SOL_SOCKET, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		cout << "Could not set IP_ADD_MEMBERSHIP" << endl;
		exit(1);
	}

	if (bind(sock, (struct sockaddr *)&local, sizeof(local)) < 0)
	{
		cout << "Cannot bind socket" << endl;
		exit(1);
	}

	char * buffer = new char[BUF_SIZE];

	while(1)
	{
		memset(buffer, '\0', BUF_SIZE);

		cout << "Waiting...";
		cout.flush();

		socklen_t sockaddr_size = sizeof(local);
		int receivedSize = recvfrom(sock, &buffer, sizeof(&buffer), 0, (struct sockaddr *)&local, &sockaddr_size);
		if (receivedSize < 0)
		{
			cout << "recvfrom size is invalid " << receivedSize << endl;
			break;
		}
		cout << "Received!" << endl;
	}

	close(sock);
}



























