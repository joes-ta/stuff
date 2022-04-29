// https://docs.microsoft.com/en-us/windows/win32/winsock/complete-server-code

#include <ws2tcpip.h>
#include <stdio.h>
#include <time.h>

#pragma comment (lib, "Ws2_32.lib")

DWORD WINAPI clientHandler( void *sd );

struct player {
    char playerUsername[30][24];
    int wallet[30];
    int bankMoney[30];
    int timesSmithed[30];
    int timesTaught[30];
    int timesMined[30];
} player1;

int workMining ( int timesMined, int currentMoney ) {
    int miningExperience=timesMined * .25;
    int money;
    timesMined=timesMined+1;
    int moneyExperienceModifier=timesMined * 10;
    if ( miningExperience <= 24) {
        money=rand() %  moneyExperienceModifier + 20;
    }
    else if ( miningExperience >= 25 ) {
        money=rand() % moneyExperienceModifier + 101;
    }
    printf ("Experience Gained: %d xp\n", 25);
    printf ("Money Earned: %d dollars\n", money);
    printf ("Mining Level: %d\n", miningExperience);
    int wallet=currentMoney + money;
    return wallet;
}

int workSmithing ( int timesSmithed, int currentMoney ) {
    int smithingExperience=timesSmithed * .25;
    int money;
    int smithingExperienceModifier=timesSmithed * 12;
    if ( smithingExperience <= 24) {
        money=rand() % smithingExperienceModifier + 51;
    }
    else if ( smithingExperience >= 25 ) {
        money=rand() % smithingExperienceModifier + 201;
    }
    printf ("Experience Gained: %d xp\n", 25);
    printf ("Money Earned: %d dollars\n", money);
    printf ("Smithing Level: %d\n", smithingExperience);
    int wallet=currentMoney + money;
    return wallet;
}

int workTeaching ( int timesTaught, int currentMoney ) {
    int teachingExperience=timesTaught * .25;
    int money;
    int teachingExperienceModifier=timesTaught * 11;
    if ( teachingExperience <= 24) {
        money=rand() % teachingExperienceModifier + 26;
    }
    else if ( teachingExperience >= 25 ) {
        money=rand() % teachingExperienceModifier + 151;
    }
    printf ("Experience Gained: %d xp\n", 25);
    printf ("Money Earned: %d dollars\n", money);
    printf ("Teaching Level: %d\n", teachingExperience);
    int wallet=currentMoney + money;
    return wallet;
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
    int timesTaught=0;
    char* outputTest;
    int timeSince;
    int bankMoney=0;
    int invalidDeposit=0;
    int withdrawMoney=0;
    int withdrawPrevious=0;
    int depositPrevious=0;
    int arrayCopy;
    char* previous;
    char* bankOption;
    int wallet=0;
    time_t start, end;
    start=0;
    end=0;
    clientInput=(char *)malloc(512);
    outputTest=(char *)malloc (24);
    srand((unsigned) time(NULL));

	do {
        result = recv( clientSocket, recvbuf, recvbuflen, 0 );
        FILE* usernames=fopen("projectUsers.txt", "r");
        char previousUsername[24];
        char username[24];
        if( result > 0 ) {
            // new code
            // usernames
            if (recvbuf != NULL) {
                while (fgets(username, sizeof(username), usernames) != NULL) fgets(username, sizeof(username), usernames);
                memcpy(player1.playerUsername[0], username, sizeof(player1.playerUsername[0]));
            }
            // usernames end
            // start work
            printf( "Bytes received: %d\n", result );
            outputTest=recvbuf;
            workNotSpecified=strcmp(outputTest, "$work");
            if (workNotSpecified == 0) {
                printf ("Pick from one of the following:\n $work.Mining\n $work.Teaching\n $work.Smithing\n");
            }
            returnWork=strcmp(outputTest, "$work.Mining");
            if (returnWork == 0) {
                end=time(NULL);
                timeSince=end - start;
                if ( timeSince >= 7200 ) {
                    timesMined=timesMined + 1;
                    wallet=workMining( timesMined, wallet );
                    start=time(NULL);
                }
                else {
                    timeSince=7200 - timeSince;
                    printf ( "You are still tired, you can't mine for: %d seconds\n", timeSince );
                }
            }
            returnWork=strcmp(outputTest, "$work.Smithing");
            if (returnWork == 0) {
                end=time(NULL);
                timeSince=end - start;
                if ( timeSince >= 14400 ) {
                    timesSmithed=timesSmithed + 1;
                    wallet=workSmithing( timesSmithed, wallet );
                    start=time(NULL);
                }
                else {
                    timeSince=14400 - timeSince;
                    printf ("You are still tired, you can't start smithing again for: %d seconds\n", timeSince );
                }
            }
            returnWork=strcmp(outputTest, "$work.Teaching");
            if (returnWork == 0) {
                end=time(NULL);
                timeSince=end - start;
                if ( timeSince >= 10800 ) {
                    timesTaught=timesTaught + 1;
                    wallet=workTeaching( timesTaught, wallet );
                    start=time(NULL);
                }
                else {
                    timeSince=10800 - timeSince;
                    printf ("You are still tired, you can't start teaching again for: %d seconds\n", timeSince );
                }
            }
            // end of work
            // start of wallet
            returnWork=strcmp(outputTest, "$wallet");
            if (returnWork == 0) {
                printf ("You currently have %d dollars in your wallet.\n", wallet);
            }
            // end of wallet 
            // start of bank
            returnWork=strcmp(outputTest, "$bank");
            if (returnWork == 0) {
                printf ("The teller asks if you would like to:\n   1) Deposit Money\n   2) Withdraw Money\n   3) Check Your Balance\nPlease enter $1, $2, or $3.\n");
            }
            returnWork=strcmp(outputTest, "$1");
            if (returnWork == 0) {
                printf ("How much would you like to deposit?\n The maximum amount you can deposit is: %d\n", wallet);
                depositPrevious=1;
            }
            if (depositPrevious == 1) {
                invalidDeposit=atoi(outputTest);
                returnWork=strcmp(outputTest, "$1");
                if (invalidDeposit > wallet) {
                    printf ("You cannot deposit more money than you have.\n");
                }
                else if (returnWork != 0) {
                    bankMoney=invalidDeposit+bankMoney;
                    wallet=wallet - invalidDeposit;
                    printf ("You currently have %d dollars in the bank.\n", bankMoney);
                    depositPrevious=0;
                }
            }
            returnWork=strcmp(outputTest, "$2");
            if (returnWork == 0) {
                if (bankMoney == 0) {
                    printf ("You have no money in the bank.");
                }
                else {
                    printf ("How much would you like to withdraw?\nThe most you can withdraw is: %d\n", bankMoney);
                    withdrawPrevious=1;
                }
            }
            if (withdrawPrevious == 1) {
                withdrawMoney=atoi(outputTest);
                returnWork=strcmp(outputTest, "$2");
                if (bankMoney < withdrawMoney) {
                    printf ("You cannot withdraw more than you have in the bank.\n");
                }
                else if (returnWork != 0) {
                    bankMoney=bankMoney - withdrawMoney;
                    wallet=wallet + withdrawMoney;
                    printf ("You now have %d in the bank.\n", bankMoney);
                    withdrawPrevious=0;
                }
            }
            returnWork=strcmp(outputTest, "$3");
            if (returnWork == 0) {
                printf ("Your balance is: %d\n", bankMoney);
            }
            // end of bank
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