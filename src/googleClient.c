// https://docs.microsoft.com/en-us/windows/win32/winsock/winsock-client-application

#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

int main( void ) {
    WSADATA wsaData; // Information about the WINSOCK implementation
    SOCKET hostSock = INVALID_SOCKET;
    struct addrinfo *hostAddrInfo = NULL, *attemptAddrInfo = NULL, hintsAddrInfo;
    char *sendbuf = "GET / HTTP/1.1\n\n";
    char recvbuf[ 512 ];
    int recvbuflen = 512;
    int result;

    result = WSAStartup( MAKEWORD( 2,2 ), &wsaData ); // Initiates the use of WINSOCK
    // MAKEWORD(2,2) indicates that Version 2.2 of WINSOCK is the highest version that can be used
    if ( result != 0 ) {
        printf( "WSAStartup failed with error: %d\n", result );
        return -1;
    }

    ZeroMemory( &hintsAddrInfo, sizeof(hintsAddrInfo) );
    hintsAddrInfo.ai_family = AF_UNSPEC; // AF_UNSPEC indicates any protocol will be accepted to connect
    hintsAddrInfo.ai_socktype = SOCK_STREAM; // SOCK_STREAM signifies that it's a TCP connection
    hintsAddrInfo.ai_protocol = IPPROTO_TCP; // Further signifies the TCP connection

    result = getaddrinfo( "google.com", "80", &hintsAddrInfo, &hostAddrInfo ); // hints indicates socket type, AddrInfo contains response information
    if ( result != 0 ) {
        printf( "getaddrinfo failed with error: %d\n", result );
        WSACleanup( );
        // terminates the use of WINSOCK features, pretty much the opposite of startup
        return -2;
    }

    for( attemptAddrInfo = hostAddrInfo; attemptAddrInfo != NULL ; attemptAddrInfo = attemptAddrInfo->ai_next ) {

        hostSock = socket( attemptAddrInfo->ai_family, attemptAddrInfo->ai_socktype, 
            attemptAddrInfo->ai_protocol );
        // socket(), creates a socket connection
        // first parameter is what types of connections can access
        // second parameter is how the connection is upheld, UDP, TCP, etc...
        // third parameter is the protocol being used
        if( hostSock == INVALID_SOCKET ) {
            printf( "socket failed with error: %ld\n", WSAGetLastError( ) );
            WSACleanup( );
            // terminates the use of WINSOCK features, pretty much the opposite of startup
            return -3;
        }

        result = connect( hostSock, attemptAddrInfo->ai_addr, (int)attemptAddrInfo->ai_addrlen);
        // connect(), establishes a connection to a socket
        // first parameter is the socket to be connected to
        // second parameter is the socket address
        // third is the length of the socket address
		printf( "hostAddrInfo: %d\n", result );
        if( result == SOCKET_ERROR ) {
            closesocket( hostSock );
            hostSock = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo( hostAddrInfo ); // frees address information that is no longer needed now that a connection is established

    if( hostSock == INVALID_SOCKET ) {
        printf( "Unable to connect to server!\n" );
        WSACleanup( );
        // terminates the use of WINSOCK features, pretty much the opposite of startup
        return -4;
    }

    result = send( hostSock, sendbuf, (int)strlen(sendbuf), 0 );
    // send(), sends information on a connected socket
    // first parameter is the socket
    // second parameter is a buffer with the information
    // third parameter is the length of the buffer
    // final parameter is the flags, 0 for none
    if( result == SOCKET_ERROR ) {
        printf( "send failed with error: %d\n", WSAGetLastError( ) );
        closesocket( hostSock );
        WSACleanup( );
        // terminates the use of WINSOCK features, pretty much the opposite of startup
        return -5;
    }

    printf("Bytes Sent: %ld\n", result);

    result = shutdown( hostSock, SD_SEND );
    // shutdown(), disables sends, receives, or both on a socket
    // first parameter is the socket
    // second parameter is what is to be shutdown, SD_SEND, SD_RECEIVE, SD_BOTH
    if( result == SOCKET_ERROR ) {
        printf( "shutdown failed with error: %d\n", WSAGetLastError( ) );
        closesocket( hostSock );
        WSACleanup( );
        // terminates the use of WINSOCK features, pretty much the opposite of startup
        return -6;
    }

	int preview = 0, total = 0;
    do {

        result = recv( hostSock, recvbuf, recvbuflen, 0 );
        // recv(), receives data on a connected socket
        // first parameter is the socket
        // second parameter is the buffer of information
        // third parameter is the length of the buffer
        // fourth parameter is any flags, 0 for none
        if ( result > 0 ) {
			total += result;
            // += operator increments the total amount of bytes received by, in this case, result
			if( preview < 1000 ) { fwrite( recvbuf, 1, result, stdout ); preview += result; }
            // prints the information received on the socket
        } else if( result == 0 )
            printf( "Connection closed\n" );
            // if there is no more information, closes the socket
        else
            printf( "recv failed with error: %d\n", WSAGetLastError( ) );

    } while( result > 0 );
    printf( "\n\nBytes received: %d\n", total );

    closesocket( hostSock );
    WSACleanup( );
    // terminates the use of WINSOCK features, pretty much the opposite of startup

    return 0;
}