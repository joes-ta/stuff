// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

#include <ws2tcpip.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

struct player
{
    char playerid[30];
    bool inuse;
    char *inventory;
        int health;
        int wallet;
        long int bank;
};
#pragma comment (lib, "Ws2_32.lib")
void action(int returninv,int returnhea,int returnwal,int returnbnk,int returnwrk,
                char **inventory,int *health,int *wallet, long int *bank,char *message){
    if( returninv == 0)
    {
    
    }
    if( returnhea == 0)
    {
    
    }
    if( returnwal == 0)
    {
        void *temp=&wallet;
        (*wallet)+=0; bank=temp;
        printf("%p",&bank);
    }    
    if( returnwrk == 0)
    {
    int money=rand()%100;
    srand(time(NULL));
    *wallet+=money;
    printf("You got:$%d\n",money);
        }    
    /* if( returnwal == 0)
    {
        int *temp=&wallet;
        (*wallet)+=0; *bank=temp;
        printf("%ld",*bank);
    } */
    if( returnbnk == 0)
    {
    }
                  };

void see(int returninv,int returnhea,int returnwal, int returnbnk,
            char *inventory, int health,int wallet, long int bank){
    printf("bef:%d\n",returninv);
                printf("%d\n",returnhea);
                printf("%d\n",returnwal);
    
    if( returninv == 0)
    {
    printf("Your Inventory:%s\n",inventory);
    }
    if( returnhea == 0)
    {
    printf("Your Health:%d\n",health);
    }
    if( returnwal == 0)
    {
    printf("Your Wallet:$%d\n",wallet);
    }
    if( returnbnk == 0)
    {
    printf("Your bank:$%ld\n",bank);
    }
};


int main() {
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;
    struct addrinfo *hostAddrInfo = NULL, hintsAddrInfo;
    //struct player *player,playerid;
    struct player player;
    int iSendResult;
    char recvbuf[ 512 ];
    int recvbuflen = 512;
    int result;
     int communicate=0;
        player.inventory="A Joe";
        player.health=100;
        player.wallet=1000;
        player.bank=0;
do{
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
        char *message;
        int returninv;
        int returnhea;
        int returnwal;
        int returnwrk;
        int returnbnk;
        int quitret;
            message=(char *)malloc(512);
                strcpy(message,recvbuf);
                sscanf("%s",message); //Copies data of format string from variable (message)
            printf("Message received:%s\n",message);
                switch (recvbuf[0])
                {
                case '$':
                returninv=strcmp(message, "$inventory");
                returnhea=strcmp(message, "$health");
                returnwal=strcmp(message, "$wallet");
                returnbnk=strcmp(message, "$bank");
                see(returninv,returnhea,returnwal,returnbnk,
                player.inventory,player.health,player.wallet,player.bank);
                break;
                case '!':
                  returninv=strcmp(message, "!inventory");
                returnwrk=strcmp(message, "!work");
                returnwal=strcmp(message, "!wallet");
                returnbnk=strcmp(message, "!bank");
                action(returninv,returnhea,returnwal,returnbnk,returnwrk,
                &player.inventory,&player.health,&player.wallet,&player.bank,message);
                break;
                case 'q':
                communicate=1;
                closesocket(ClientSocket);
                WSACleanup();
                return 0;
                break;
                default: printf("Error\n");
                    break;}
                    printf("invret:%d\n",returninv);
                printf("hearet:%d\n",returnhea);
                printf("walret:%d\n",returnwal);
                printf("bnkret:%d\n",returnbnk);
                printf("wrkret:%d\n",returnwrk);
            //printf("\nFrom Client\n");
            iSendResult = send( ClientSocket, recvbuf, result, 0 );
        
        if( iSendResult == SOCKET_ERROR ) {
                printf( "send failed with error: %d\n", WSAGetLastError( ) );
                closesocket( ClientSocket );
                WSACleanup( );
            return -7;
            }
                printf( "Bytes sent: %d\n", iSendResult);
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

    }   while( result > 0 );
            
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
}while (communicate=0);
}