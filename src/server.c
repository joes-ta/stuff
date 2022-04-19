// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

DWORD WINAPI clientHandler( void *sd );

void workMining () {
    printf ("Experience Gained: %d xp\n", 25);
    printf ("Money Earned: %d\n", 5);
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
    clientInput=(char *)malloc(512);

	do {
        result = recv( clientSocket, recvbuf, recvbuflen, 0 );
        if( result > 0 ) {
            printf( "Bytes received: %d\n", result );
            strcpy(clientInput,recvbuf);
            printf ("%s", clientInput);
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