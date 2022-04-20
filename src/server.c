// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

DWORD WINAPI clientHandler( void *sd );

void workMining ( int timesMined ) {
    int miningExperience=timesMined * .25;
    int money;
    if ( miningExperience < 5) {
        money=rand() % 51 + 1;
    }
    else if ( miningExperience >= 5 ) {
        money=rand() % 201 + 51;
    }
    printf ("Experience Gained: %d xp\n", 25);
    printf ("Money Earned: %d dollars\n", money);
    printf ("Mining Level: %d\n", miningExperience);
}

void workSmithing ( int timesSmithed ) {
    int smithingExperience=timesSmithed * .25;
    int money;
    if ( smithingExperience < 5) {
        money=rand() % 251 + 51;
    }
    else if ( smithingExperience >= 5 ) {
        money=rand() % 501 + 201;
    }
    printf ("Experience Gained: %d xp\n", 25);
    printf ("Money Earned: %d dollars\n", money);
    printf ("Mining Level: %d\n", smithingExperience);
}

int main( void ) {
    WSADATA wsaData;
    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;
    struct addrinfo *hostAddrInfo = NULL, hintsAddrInfo;
    int result;
    
    result = WSAStartup( MAKEWORD(2,2), &wsaData );
    if( result != 0 ) {
        printf( "WSAStartup failed with error: %d\n", result );
        return -1;
    }

    ZeroMemory( &hintsAddrInfo, sizeof hintsAddrInfo );
    hintsAddrInfo.ai_family = AF_INET;
    hintsAddrInfo.ai_socktype = SOCK_STREAM;
    hintsAddrInfo.ai_protocol = IPPROTO_TCP;
    hintsAddrInfo.ai_flags = AI_PASSIVE;

    result = getaddrinfo( "localhost", "12345", &hintsAddrInfo, &hostAddrInfo );
    if ( result != 0 ) {
        printf( "getaddrinfo failed with error: %d\n", result );
        WSACleanup( );
        return -2;
    }

    listenSocket = socket( hostAddrInfo->ai_family, hostAddrInfo->ai_socktype, hostAddrInfo->ai_protocol );
    if( listenSocket == INVALID_SOCKET ) {
        printf( "socket failed with error: %ld\n", WSAGetLastError( ) );
        freeaddrinfo( hostAddrInfo );
        WSACleanup( );
        return -3;
    }

    result = bind( listenSocket, hostAddrInfo->ai_addr, (int)hostAddrInfo->ai_addrlen );
    if( result == SOCKET_ERROR ) {
        printf( "bind failed with error: %d\n", WSAGetLastError( ) );
        freeaddrinfo( hostAddrInfo );
        closesocket( listenSocket );
        WSACleanup( );
        return -4;
    }

    freeaddrinfo( hostAddrInfo );

    result = listen( listenSocket, SOMAXCONN );
    if( result == SOCKET_ERROR ) {
        printf( "listen failed with error: %d\n", WSAGetLastError( ) );
        closesocket( listenSocket );
        WSACleanup( );
        return -5;
    }
	
    SOCKADDR_IN sinRemote;
    int nAddrSize = sizeof(sinRemote);
	DWORD threadID;

	while( 1 ) {
		clientSocket = accept( listenSocket, (SOCKADDR *)&sinRemote, &nAddrSize );
		if( clientSocket == INVALID_SOCKET ) {
			printf( "accept failed with error: %d\n", WSAGetLastError( ) );
			closesocket( listenSocket );
			WSACleanup( );
			return -6;
		}
		CreateThread(0, 0, clientHandler, (void*)clientSocket, 0, &threadID);	
	}

//    closesocket( listenSocket );

    return 0;
}

DWORD WINAPI clientHandler( void *sd ) {
	SOCKET clientSocket = (SOCKET)sd;
    int iSendResult;
    char recvbuf[ 512 ];
    int recvbuflen = 512;
	int result = 0;
    char* clientInput;
    int returnWork;
    int workNotSpecified;
    int timesMined=0;
    int timesSmithed=0;
    char* outputTest;
    clientInput=(char *)malloc(512);
    outputTest=(char *)malloc (24);

	do {
        result = recv( clientSocket, recvbuf, recvbuflen, 0 );
        if( result > 0 ) {
            // new code
            printf( "Bytes received: %d\n", result );
            outputTest=recvbuf;
            workNotSpecified=strcmp(outputTest, "$work");
            if (workNotSpecified == 0) {
                printf ("Pick from one of the following:\n $work.Mining\n $work.Teaching\n $work.Smithing\n");
            }
            returnWork=strcmp(outputTest, "$work.Mining");
            if (returnWork == 0) {
                timesMined=timesMined + 1;
                workMining( timesMined );
            }
            returnWork=strcmp(outputTest, "$work.Smithing");
            if (returnWork == 0) {
                timesSmithed=timesSmithed + 1;
                workSmithing( timesSmithed );
            }
            // end of new code
            iSendResult = send( clientSocket, recvbuf, result, 0 );
            if( iSendResult == SOCKET_ERROR ) {
                printf( "send failed with error: %d\n", WSAGetLastError( ) );
                closesocket( clientSocket );
                WSACleanup( );
                return -7;
            }
            printf( "Bytes sent: %d\n", iSendResult );
        }
        else if( result == 0 )
            printf( "Connection closing...\n" );
        else  {
            printf( "recv failed with error: %d\n", WSAGetLastError( ) );
            closesocket( clientSocket );
            WSACleanup( );
            return -8;
        }

    } while( result > 0 );

    result = shutdown( clientSocket, SD_BOTH );
    if( result == SOCKET_ERROR ) {
        printf( "shutdown failed with error: %d\n", WSAGetLastError( ) );
        closesocket( clientSocket );
        WSACleanup( );
        return -9;
    }

    closesocket( clientSocket );
//    WSACleanup( );
	
	return result;
}