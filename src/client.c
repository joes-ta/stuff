// https://beej.us/guide/bgnet/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>

int main( void ) {
	int hostSock;
	struct addrinfo *hostAddrInfo = NULL, *attemptAddrInfo = NULL, hintsAddrInfo;
    char *sendbuf = "GET / HTTP/1.1\n\n";
    char recvbuf[ 512 ];
    int recvbuflen = 512;
	int result;
	
	memset( &hintsAddrInfo, 0, sizeof (hintsAddrInfo) );
	hintsAddrInfo.ai_family = AF_UNSPEC;
	hintsAddrInfo.ai_socktype = SOCK_STREAM;
    hintsAddrInfo.ai_protocol = IPPROTO_TCP;

	result = getaddrinfo( "google.com", "80", &hintsAddrInfo, &hostAddrInfo);
	if( result != 0 ) {
		perror( "getaddrinfo failed with error: " );
		return -2;
    }

	for( attemptAddrInfo = hostAddrInfo; attemptAddrInfo != NULL; attemptAddrInfo = attemptAddrInfo->ai_next) {
		hostSock = socket(attemptAddrInfo->ai_family, attemptAddrInfo->ai_socktype, attemptAddrInfo->ai_protocol);
		if( hostSock == -1 ) {
			perror("socket failed with error: ");
			continue;
		}
		
		result = connect( hostSock, attemptAddrInfo->ai_addr, attemptAddrInfo->ai_addrlen );
		if( result == -1 ) {
			close( hostSock );
			continue;
		}
		break;
	}

	freeaddrinfo( hostAddrInfo );

	if( attemptAddrInfo == NULL ) {
		printf( "Unable to connect to server!\n");
		return -4;
	}
	
	result = send( hostSock, sendbuf, (int)strlen(sendbuf), 0);
	if ( result == -1) {
		perror("send failed with error: ");
		close( hostSock );
	}

    printf("Bytes Sent: %d\n", result);
	
    result = shutdown( hostSock, SHUT_WR );
    if( result == -1 ) {
		perror( "shutdown failed with error: " );
        close( hostSock );
        return -6;
    }

	int preview = 0, total = 0;
    do {

        result = recv( hostSock, recvbuf, recvbuflen, 0 );
        if ( result > 0 ) {
			total += result;
			if( preview < 1000 ) { fwrite( recvbuf, 1, result, stdout ); preview += result; }
        } else if( result == 0 )
            printf( "Connection closed\n" );
        else
            perror( "recv failed with error: " );

    } while( result > 0 );
    printf( "\n\nBytes received: %d\n", total );
	
	close( hostSock );
	return 0;
}
