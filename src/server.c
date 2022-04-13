// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

int main( void ) {
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo *hostAddrInfo = NULL, hintsAddrInfo;
    int iSendResult;
    char recvbuf[ 512 ];
    int recvbuflen = 512;
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

    ListenSocket = socket( hostAddrInfo->ai_family, hostAddrInfo->ai_socktype, hostAddrInfo->ai_protocol );
    if( ListenSocket == INVALID_SOCKET ) {
        printf( "socket failed with error: %ld\n", WSAGetLastError( ) );
        freeaddrinfo( hostAddrInfo );
        WSACleanup( );
        return -3;
    }

    result = bind( ListenSocket, hostAddrInfo->ai_addr, (int)hostAddrInfo->ai_addrlen );
    if( result == SOCKET_ERROR ) {
        printf( "bind failed with error: %d\n", WSAGetLastError( ) );
        freeaddrinfo( hostAddrInfo );
        closesocket( ListenSocket );
        WSACleanup( );
        return -4;
    }

    freeaddrinfo( hostAddrInfo );

    result = listen( ListenSocket, SOMAXCONN );
    if( result == SOCKET_ERROR ) {
        printf( "listen failed with error: %d\n", WSAGetLastError( ) );
        closesocket( ListenSocket );
        WSACleanup( );
        return -5;
    }

    ClientSocket = accept( ListenSocket, NULL, NULL );
    if( ClientSocket == INVALID_SOCKET ) {
        printf( "accept failed with error: %d\n", WSAGetLastError( ) );
        closesocket( ListenSocket );
        WSACleanup( );
        return -6;
    }

    closesocket( ListenSocket );

    do {

        result = recv( ClientSocket, recvbuf, recvbuflen, 0 );
        if( result > 0 ) {
printf("Message received:%s\0",&recvbuf);
            printf("\nFrom Client\n");
            iSendResult = send( ClientSocket, recvbuf, result, 0 );
            if( iSendResult == SOCKET_ERROR ) {
                printf( "send failed with error: %d\n", WSAGetLastError( ) );
                closesocket( ClientSocket );
                WSACleanup( );
                return -7;
            }
            printf( "Bytes sent: %d\n", iSendResult );
            printf("From Server\n");
        }
        else if( result == 0 )
            printf( "Connection closing...Closing Client Socket\n" );
        else  {
            printf( "recv failed with error: %d\n", WSAGetLastError( ) );
            closesocket( ClientSocket );
            WSACleanup( );
            return -8;
        }

    } while( result > 0 );

    result = shutdown( ClientSocket, SD_SEND );
    if( result == SOCKET_ERROR ) {
        printf( "shutdown failed with error: %d\n", WSAGetLastError( ) );
        closesocket( ClientSocket );
        WSACleanup( );
        return -9;
    }

    closesocket( ClientSocket );
    WSACleanup( );

    return 0;
}