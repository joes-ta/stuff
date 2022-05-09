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
     char *inventory[30];
      int health[30];
       int wallet[30];
        long int bank[30];
        long long int xp[30];
        long int rep[30];
         SOCKET ClientSocket[30];
};
//Game Mechanics Happen Here
//Alternated Buffer. *Note:Will send back empty recvbuf for undeveloped mechanics
void action(int returninv,int returnhea,int returnwal,int returnbnk,int returnwrk,int returndtwrk, int returnquit,
                char *recvbuf,int *recvbuflen,char **inventory,int *health,int *wallet, long int *bank, long long int *xp, long int *rep, int *result){
    char str[512];
    memset(str,0,512);
    memset(recvbuf,0,512);
    if( returninv == 0) //Actions for inventory
    {
        //arguments needed
    }
    if( returnhea == 0) //Actions for health
    {
        //arguments needed
    }
    if( returnwal == 0) //Actions for wallet
    {
        unsigned local=*wallet;
        *wallet=0; *bank=local;
        printf("Money Deposited To Bank:%d\n",*bank);
        char *strltrl="You Deposited: $";
    sprintf(recvbuf,"%s%d",strltrl,local);
    *recvbuflen=(int)strlen(recvbuf);
    }    
        if( returnwrk == 0) //Work
    {
        int earnxp=rand()%1000;
        srand(time(NULL));
        int money=rand()%100;
        srand(time(NULL));
        int earnrep=rand()%100;
        srand(time(NULL));
        *wallet+=money;
        *xp+=earnxp;
        *rep+=earnrep;
    //    char *strltrlcash="You Got: $%d",money;
    //    char *strltrlexp="\nYou Earned: %d xp",earnxp;
    //    char *strltrlrespect="\nYou Earned: =%d rep",earnrep;
    //    sprintf(recvbuf,"%s/n%s/n%s",strltrlcash,strltrlexp,strltrlrespect);
    char *strltrlcash="You Got: $";
    char *strltrlexp="You Got: ";
    char *strltrlrespect="You Got: +";
    sprintf(recvbuf,"\n%s%d\n%s%d XP\n%s%d Reputation",strltrlcash,money,strltrlexp,earnxp,strltrlrespect,earnrep);
    *recvbuflen=(int)strlen(recvbuf);
    }  
        if( returndtwrk == 0) //Dirty Work
    {
        int earnxp=rand()%1000;
        srand(time(NULL));
        int money=rand()%100;
        srand(time(NULL));
        int earnrep=rand()%100;
        srand(time(NULL));
        *wallet+=money;
        *xp+=earnxp;
        *rep-=earnrep;

        if(*rep <= -100)
        {
           int loseyourmoney=rand()%10;
           srand(time(NULL));
           if(loseyourmoney == 8)

           {
             *wallet=0;
             printf("Badluck has caught up to you, your wallet now has: $%d\n", *wallet);
           }
        }
        
    //   char *strltrlcash="You Got: $%d",money;
    //   char *strltrlexp="\nYou Earned: %d xp",earnxp;
    //   char *strltrlrespect="\nYou Earned: -%d rep",earnrep;
    //   sprintf(recvbuf,"%s/n%s/n%s",strltrlcash,strltrlexp,strltrlrespect);
    char *strltrlcash="You Got: $";
    char *strltrlexp="You Got: ";
    char *strltrlrespect="You Got: -";
    sprintf(recvbuf,"\n%s%d\n%s%d XP\n%s%d Reputation",strltrlcash,money,strltrlexp,earnxp,strltrlrespect,earnrep);
    *recvbuflen=(int)strlen(recvbuf);
    }      
    if( returnbnk == 0)
    {
        //arguments needed
    }   
    if(returnquit == 0)
    {
        *result = 0;
    }           
};

void see(int returninv,int returnhea,int returnwal, int returnbnk,int returnid, int returnxp, int returnrep,
        char *recvbuf,int *recvbuflen, char *inventory, int health,int wallet, long int bank, long long int xp, long int rep, unsigned char playerid){
    char str[512];
     memset(str,0,512);
     memset(recvbuf,0,512);
    if( returnid == 0) //Checking Who I Am
    {
     printf("Your ID:%d\n", playerid); //replace with csock id?
      char *strltrl="ID:";
       sprintf(recvbuf,"%s%d",strltrl,playerid); //Creates Text "ID:(ID#)"//Prints to recvbuf
       *recvbuflen=(int)strlen(recvbuf);
       printf("recvbuf:%s\n",recvbuf);
      printf("Sizeof:%d\n",*recvbuflen);
       
    }
    if( returninv == 0) //Checking Player Inventory
    {
     printf("Your Inventory:%s\n",inventory);
        strncpy(recvbuf,inventory,(int)strlen(inventory));
             *recvbuflen=(int)strlen(recvbuf);
       printf("recvbuf:%s\n",recvbuf);
      printf("Sizeof:%d\n",*recvbuflen);
    }
    if( returnhea == 0) //Checking Player Health
    {
    char *strltrl=" health";
     sprintf(recvbuf,"%d%s",health,strltrl);
       *recvbuflen=(int)strlen(recvbuf);
       printf("Your Health:%s\n",str);
      printf("recvbuf:%s\n",recvbuf);
     printf("Sizeof:%d\n",*recvbuflen);    
  }
    if( returnwal == 0) //Checking Player Wallet
    {
    printf("Your Wallet: $%d\n",wallet);
    char *strltrl="Wallet: $";
    sprintf(recvbuf,"%s%d",strltrl,wallet);
    *recvbuflen=(int)strlen(recvbuf);
    printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",*recvbuflen);
    }
    if( returnbnk == 0) //Checking Player Bank
    {
    printf("Your bank: $%ld\n",bank);
    char *strltrl="Bank: $";
  sprintf(recvbuf,"%s%d",strltrl,bank);
    *recvbuflen=(int)strlen(recvbuf);  
  printf("recvbuf:%s\n",recvbuf);
            printf("Sizeof:%d\n",*recvbuflen);
    }
    if( returnxp == 0) //Checking Player XP
    {
    char *strltrl=" xp";
     sprintf(recvbuf,"%lld%s",xp,strltrl);
       *recvbuflen=(int)strlen(recvbuf);
       printf("Your xp:%s\n",str);
      printf("recvbuf:%s\n",recvbuf);
     printf("Sizeof:%d\n",*recvbuflen);    
  }
      if( returnrep == 0) //Checking Player Reputation
    {
    char *strltrl=" Reputation";
     sprintf(recvbuf,"%d%s",rep,strltrl);
       *recvbuflen=(int)strlen(recvbuf);
       printf("Your Reputation:%s\n",str);
      printf("recvbuf:%s\n",recvbuf);
     printf("Sizeof:%d\n",*recvbuflen);    
  }
};//End Of Game Mechanics

//Listening and Accepting Of Clients Happens Here
//Assigning of Client Sockets Happens Here
//Preparations For Client ID happens here;
int main(void) {
    printf ("Welcome to the GTA6: Joe's Classroom Server. Please be Responsible.\n");
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

        result = getaddrinfo( "localhost", "12345", &hintsAddrInfo, &hostAddrInfo );
    
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
                    //WSACleanup( );
                return -3;
    }

        result = bind( ListenSocket, hostAddrInfo->ai_addr, (int)hostAddrInfo->ai_addrlen );
    
        if( result == SOCKET_ERROR ) {
                printf( "bind failed with error: %d\n", WSAGetLastError( ) );
                freeaddrinfo( hostAddrInfo );
                closesocket( ListenSocket );
                //WSACleanup( );
            return -4;
    }
                freeaddrinfo( hostAddrInfo );

            result = listen( ListenSocket, SOMAXCONN );
        
        if( result == SOCKET_ERROR ) {
                printf( "listen failed with error: %d\n", WSAGetLastError( ) );
                closesocket( ListenSocket );
                //WSACleanup( );
            return -5;
    }
    
    SOCKADDR_IN sinRemote;
    int nAddrSize =sizeof(sinRemote);
    DWORD threadID;
    memset(player,0,sizeof(struct player));
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
                //WSACleanup( );
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
    //memset(player->wallet,0,30); 
    //memset(player->bank,0,30);
    for ( i = 0; i < max_clients;)
    {
        if (player->playerid[i]==0)
        {
            player->playerid[i]=player->ClientSocket[i]; //Register Id in PlayerId array
            id=i; //Store Id Array Location
            player->wallet[id];//Create Player Wallet
            player->bank[id]; //Create Player Bank
            player->health[id]; //Create Player Health
            player->inventory[id]; //Create Player Inventory
            player->xp[id]; //Create Player XP
            player->rep[id]; //Create Player Reputation
            break;
        }
        i++;
    }
     printf("CSock: %d | player->csock: %d | I : %d\n",ClientSocket,player->ClientSocket[i],id);
        //Generic Values Set Here //To Be Replaced By Saved Data
        player->inventory[id]="A Joe";
        player->health[id]=100;
        player->wallet[id]=1000;
        player->bank[id]=0;
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
        int returnxp;
        int returnrep;
        int returndtwrk;
        int returnquit;
         int recvbuflen=(int)strlen(recvbuf);    
            message=(char *)malloc(512);
             strcpy(message,recvbuf);
              sscanf("%s",message); //Copies data of format string from variable (message)
               printf("Message received:%s\n",message);
                switch (recvbuf[0])
                {
                case '?':
                returninv=strcmpi(message, "?inventory");
                returnhea=strcmpi(message, "?health");
                returnwal=strcmpi(message, "?wallet");
                returnbnk=strcmpi(message, "?bank");
                returnid=strcmpi(message,  "?me");
                returnxp=strcmpi(message, "?xp");
                returnrep=strcmpi(message, "?rep");
            see(returninv,returnhea,returnwal,returnbnk,returnid,returnxp,returnrep,recvbuf,&recvbuflen,
                player->inventory[id],player->health[id],player->wallet[id],player->bank[id],player->xp[id],player->rep[id],player->playerid[id]);
                break;
                case '$':
                returninv=strcmpi(message, "$inventory");
                returnwrk=strcmpi(message, "$work");
                returnwal=strcmpi(message, "$wallet");
                returnbnk=strcmpi(message, "$bank");
                returndtwrk=strcmpi(message, "$dirtywork");
                returnquit=strcmpi(message, "$quit");
            action(returninv,returnhea,returnwal,returnbnk,returnwrk,returndtwrk,returnquit,recvbuf,&recvbuflen,
                &player->inventory[id],&player->health[id],&player->wallet[id],&player->bank[id],&player->xp[id],&player->rep[id], &result);
                break;
                case 'q':
                closesocket(player->ClientSocket[id]);
                
                break;
                default: //Broadcast Could Happen Here
                    break;}
            //printf("\nFrom Client\n");
            printf("Ids In Use:");
            for ( i = 0; i<31; i++)
            {
                printf("Id of[#%d]: %d\t",i,player->playerid[i]);
            if (player->playerid[i+1]==0)
                break;   
            }
           //printf("recvbuf:%s\n",recvbuf);
            //printf("Sizeof Message:%d\n",recvbuflen);
   
            iSendResult = send(player->ClientSocket[id], recvbuf, recvbuflen, 0 );
        
        if( iSendResult == SOCKET_ERROR ) {
                printf( "send failed with error: %d\n", WSAGetLastError( ) );
                closesocket( player->ClientSocket[id] );
                //WSACleanup( );
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
                //WSACleanup( );
            return -8;
        }

    }   while( result > 0 );
           result = shutdown( player->ClientSocket[id], SD_SEND );
        if( result == SOCKET_ERROR ) {
                printf( "shutdown failed with error: %d\n", WSAGetLastError( ) );
                closesocket( player->ClientSocket[id] );
                //WSACleanup( );
            return -9;
    } else
                closesocket( player->ClientSocket[id] );
                player->playerid[id]=0; //Free Player ID Slot upon client exit.
                player->ClientSocket[id]=0; //Free Client Socket upon client exit.
               //free(player);
              //  WSACleanup( );
            return result;
    
}
