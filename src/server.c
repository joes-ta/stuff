// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

#include <ws2tcpip.h>
#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

#pragma comment (lib, "Ws2_32.lib")

DWORD WINAPI clientHandler(void* player);

struct player
{
    unsigned char playerid[30];
     char *inventory;
      int health;
       int wallet;
        long int bank;
         SOCKET ClientSocket[30];
         int sockid;
};
//Game Mechanics Happen Here
//Alternated Buffer. *Note:Will send back empty recvbuf for undeveloped mechanics
void action(int returninv,int returnhea,int returnwal,int returnbnk,int returnwrk,
                char **inventory,int *health,int *wallet, long int *bank,char *message){
    if( returninv == 0)
    {
        //arguments needed
    }
    if( returnhea == 0)
    {
        //arguments needed
    }
    if( returnwal == 0)
    {
        unsigned local=*wallet;
        *wallet=0; *bank=local;
        printf("Money Deposited To Bank:%d\n",*bank);
    }    
    if( returnwrk == 0)
    {
        int money=rand()%100;
        srand(time(NULL));
        *wallet+=money;
        printf("You got:$%d\n",money);
    }    
    if( returnbnk == 0)
    {
        //arguments needed
    }              
    };

void see(int returninv,int returnhea,int returnwal, int returnbnk,int returnid,
        char *recvbuf,int *recvbuflen, char *inventory, int health,int wallet, long int bank,unsigned char playerid){
   char str[512];
     memset(str,0,512);
     memset(recvbuf,0,512);
    if( returnid == 0)
    {
        printf("ID:%d\n", playerid);
        char *strltrl="ID:";
    sprintf(str,"%s%d",strltrl,playerid);
    strncpy(recvbuf,str,sizeof(str));
        *recvbuflen=sizeof(&str)+2;
        //*recvbuflen=sizeof(&strltrl)+sizeof(&playerid); 
          printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",*recvbuflen);
       
    }
    if( returninv == 0)
    {
    printf("Your Inventory:%s\n",inventory);
       
            *recvbuflen=sizeof(&inventory)+1;
            strncpy(recvbuf,inventory,sizeof(inventory)+1);
            printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",*recvbuflen);
    }
    if( returnhea == 0)
    {
    char *strltrl=" health";
    sprintf(str,"%d%s",health,strltrl);
    strncpy(recvbuf,str,sizeof(str));
            *recvbuflen=sizeof(health)+sizeof(strltrl)+2;
    printf("Your Health:%s\n",str);
  printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",*recvbuflen);
    
  }
    if( returnwal == 0)
    {
    printf("Your Wallet: $%d\n",wallet);
    /*char *strltrl="Wallet:$";
    sprintf(str,"%s%d",strltrl,wallet);
    strncpy(recvbuf,str,sizeof(str));
            *recvbuflen=sizeof();
  printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",*recvbuflen);*/
    }
    if( returnbnk == 0)
    {
    printf("Your bank: $%ld\n",bank);
    /*char *strltrl="Bank:$";
    sprintf(str,"%s%d",strltrl,bank);
    strncpy(recvbuf,str,sizeof(str));
            *recvbuflen=sizeof(bank)+sizeof(*strltrl);
  printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",*recvbuflen);*/
    }
};//End Of Game Mechanics

//Listening and Accepting Of Clients Happens Here
//Assigning of Client Sockets Happens Here
//Preparations For Client ID happens here;
int main(void) {
    WSADATA wsaData;
    struct addrinfo *hostAddrInfo = NULL, hintsAddrInfo;
    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    int result;
    
    struct player *player;
          player=(struct player *)malloc(sizeof(*player));
         
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

        result = getaddrinfo( "209.23.10.125", "12345", &hintsAddrInfo, &hostAddrInfo );
    
            if ( result != 0 ) {
                    printf( "getaddrinfo failed with error: %d\n", result );
                    WSACleanup( );
                return -2;
    }
            for (int i = 0; i <= 30; ){ //Set All ID's to 0     
        player->playerid[i] = 0;
        i++;
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
    
    SOCKADDR_IN sinRemote;
    int nAddrSize =sizeof(sinRemote);
    DWORD threadID;
    memset(player->ClientSocket,0,30);
        while(1){
            ClientSocket = accept( ListenSocket, (SOCKADDR *)&sinRemote,&nAddrSize);
        
        for(int i=0;i<30;){
        printf("I: %d Socket#: %d\n",i,player->ClientSocket[i]);
        if(player->ClientSocket[i]==0){
        player->ClientSocket[i]=ClientSocket;
        printf("CSock: %d | player->csock: %d | I : %d\n",ClientSocket,player->ClientSocket[i],i);
        break;}
        i++;
        }
        if( ClientSocket == INVALID_SOCKET ) {
                printf( "accept failed with error: %d\n", WSAGetLastError( ) );
                closesocket( ListenSocket );
                WSACleanup( );
            return -6;
            }
    CreateThread(0,0,clientHandler, (void *)player,0,&threadID);
        }
               closesocket( ListenSocket );
       return;
}
    
    //Sending And Reciving Happens Here
    //Assignment of Player Ids Happens Here
    //Manipulation Of Client Message Happens Here
    DWORD WINAPI clientHandler(struct player *player){
    SOCKET ClientSocket = (SOCKET)player;
    int iSendResult;
     //char recvbuf[512];
      //int recvbuflen = 512;
	   int result = 0;
      int max_clients=30;
     int id;
    int i;
    
    for ( i = 0; i < max_clients;)
    {
        if (player->playerid[i]==0)
        {
            player->playerid[i]=player->ClientSocket[i]; //Register Id in PlayerId array
            id=i; //Store Id Array Location
            break;
        }
        i++;
    }

        player->inventory="A Joe";
        player->health=100;
        player->wallet=1000;
        player->bank=0;
        do { 
            char recvbuf[512];
      int recvbuflen = 512;
            result = recv( player->ClientSocket[id],recvbuf, recvbuflen, 0 );
            
        if( result > 0 ) {
        char *message;
         int returninv;
          int returnhea;
           int returnid;
           int returnwal;
          int returnwrk;
         int returnbnk;
        int quitret;
         int recvbuflen=(int)strlen(recvbuf);    
            message=(char *)malloc(512);
                strcpy(message,recvbuf);
                sscanf("%s",message); //Copies data of format string from variable (message)
                printf("Message received:%s\n",message);
                switch (recvbuf[0])
                {
                case '?':
                returninv=strcmp(message, "?inventory");
                returnhea=strcmp(message, "?health");
                returnwal=strcmp(message, "?wallet");
                returnbnk=strcmp(message, "?bank");
                returnid=strcmp(message,  "?me");
            see(returninv,returnhea,returnwal,returnbnk,returnid,recvbuf,&recvbuflen,
                player->inventory,player->health,player->wallet,player->bank,player->playerid[id]);
                break;
                case '$':
                returninv=strcmp(message, "$inventory");
                returnwrk=strcmp(message, "$work");
                returnwal=strcmp(message, "$wallet");
                returnbnk=strcmp(message, "$bank");
            action(returninv,returnhea,returnwal,returnbnk,returnwrk,
                &player->inventory,&player->health,&player->wallet,&player->bank,message);
                break;
                case 'q':
                closesocket(player->ClientSocket[id]);
                
                break;
                default: printf("Error\n");
                    break;}
            //printf("\nFrom Client\n");
            printf("Ids In Use:");
            for ( i = 0; i<31; i++)
            {
                printf("Id of[#%d]: %d\t",i,player->playerid[i]);
            if (player->playerid[i+1]==0)
                break;   
            }
           printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",recvbuflen);
    
   
            iSendResult = send(player->ClientSocket[id], recvbuf, recvbuflen, 0 );
        
        if( iSendResult == SOCKET_ERROR ) {
                printf( "send failed with error: %d\n", WSAGetLastError( ) );
                closesocket( player->ClientSocket[id] );
                WSACleanup( );
            return -7;
            }
                printf( "Bytes sent: %d\n", iSendResult);
                //printf("From Server\n");
        }
        
        else if( result == 0 )
                printf( "Connection closing...Closing Client Socket\n" );
        
        else  {
                printf( "recv failed with error: %d\n", WSAGetLastError( ) );
                closesocket( player->ClientSocket[id] );
                WSACleanup( );
            return -8;
        }

    }   while( result > 0 );
           /*result = shutdown( ClientSocket, SD_SEND );
        if( result == SOCKET_ERROR ) {
                printf( "shutdown failed with error: %d\n", WSAGetLastError( ) );
                closesocket( player->ClientSocket );
                WSACleanup( );
            return -9;
    }*/
                closesocket( player->ClientSocket[id] );
                player->playerid[id]=0; //set Place Of Id to 0 if disconnect so other players can take spot;
                player->ClientSocket[id]=0;
//free(player);
              //  WSACleanup( );
            return result;
    
}