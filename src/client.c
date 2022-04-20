// https://docs.microsoft.com/en-us/windows/win32/winsock/winsock-client-application

#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment (lib, "Ws2_32.lib")

int main( void ) {
    WSADATA wsaData;
    SOCKET hostSock = INVALID_SOCKET;
    struct addrinfo *hostAddrInfo = NULL, *attemptAddrInfo = NULL, hintsAddrInfo;
    char recvbuf[ 512 ];
    int recvbuflen = 512;
    char *input= (char *)malloc(512);
    int result;
    char *quit="quit";
    int ret;    
        
        result = WSAStartup( MAKEWORD( 2,2 ), &wsaData );
    
    if ( result != 0 ) {
        printf( "WSAStartup failed with error: %d\n", result );
        return -1;
    }

    ZeroMemory( &hintsAddrInfo, sizeof(hintsAddrInfo) ); //memset( &hintsAddrInfo, 0(intialize to 1),sizeof(hintsAddrInfo) )
        hintsAddrInfo.ai_family = AF_UNSPEC;
        hintsAddrInfo.ai_socktype = SOCK_STREAM;
        hintsAddrInfo.ai_protocol = IPPROTO_TCP;

        result = getaddrinfo( "localhost", "12345", &hintsAddrInfo, &hostAddrInfo );//Attempt to contact "localhost" at port 12345//List of address structures
    
    if ( result != 0 ) {
        printf( "getaddrinfo failed with error: %d\n", result );
        WSACleanup( );
        return -2;
    }

    for( attemptAddrInfo = hostAddrInfo; attemptAddrInfo != NULL ; attemptAddrInfo = attemptAddrInfo->ai_next )//ai_next=next address 
    {
        hostSock = socket( attemptAddrInfo->ai_family, attemptAddrInfo->ai_socktype, 
            attemptAddrInfo->ai_protocol );
        
    if( hostSock == INVALID_SOCKET ) {
            printf( "socket failed with error: %ld\n", WSAGetLastError( ) );
            WSACleanup( );
        return -3;
        }

        result = connect( hostSock, attemptAddrInfo->ai_addr, (int)attemptAddrInfo->ai_addrlen); //-> member of
    
    if( result == SOCKET_ERROR ) {
            closesocket( hostSock );
        hostSock = INVALID_SOCKET;
        continue;
        }
        break;
    }

            freeaddrinfo( hostAddrInfo );

    if( hostSock == INVALID_SOCKET ) {
            printf( "Unable to connect to server!\n" );
            WSACleanup( );
        return -4;
    }
    do{
            memset(input,0,512); //allocate 512 bit for input and store them as 0's
            scanf("%511[^\n]",input); //scan through 511 bits for *c / message; looking for anything that is [^\n](Not a newline)  
            scanf("%*c");//temporary place holder of current character.
        ret=strcmp(quit,input);
    switch (ret)
        {
          case 0:
           closesocket( hostSock );
            WSACleanup( );
           return 0;
          break;
          case !0:
          
          result = send( hostSock, input, (int)strlen(input)+1, 0 ); //(Socket,message,sizeof(message),flags)
          
          break;
      }
       
    if( result == SOCKET_ERROR ) {
            printf( "send failed with error: %d\n", WSAGetLastError( ) );
            closesocket( hostSock );
            WSACleanup( );
        return -5;
    }
            //printf("Message Sent:%s\n", input);   
           // printf("Bytes Sent: %d\n", result);//printf("From Client\n");
            //printf("\nMessage Recieved:\n");
        
    int preview = 0;int total=0;

        result = recv( hostSock, recvbuf, recvbuflen, 0 );
    
    if ( result > 0 ) {
    	
        total += result;
	
    if( preview < 1000 && ret!=0) {
        printf("Message Check: ");
        fwrite(recvbuf, 1, result, stdout);
        printf("\n");
         preview += result; } //Preview limits to 1000 characters printed.
            //printf("\n\nThis Was From The Server\n\n");
        
        } 
    else if( result == 0 )
            printf( "Connection Gets Closed Here\n" );    
        
    else
            printf( "recv failed with error: %d\n", WSAGetLastError( ) );

    
          //  printf( "Bytes Recieved: %ld\n", total);
            //printf( "To Client\n");
    
    }while(ret!=0);
    
   /* result = shutdown( hostSock, SD_SEND );
    
    if( result == SOCKET_ERROR ) {
            printf( "shutdown failed with error: %d\n", WSAGetLastError( ) );
            closesocket( hostSock );
            WSACleanup( );
            return -6;}
     */   
            closesocket( hostSock );
            //WSACleanup( );
        return;
}