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

int workMining ( char recvbuf[512], int timesMined, int currentMoney ) {
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
    sprintf ( recvbuf, "Experience Gained: %d xp\nMoney Earned: %d dollars\nMining Level: %d\n", 25, money, miningExperience);
    int wallet=currentMoney + money;
    return wallet;
}

int workSmithing ( char recvbuf[512], int timesSmithed, int currentMoney ) {
    int smithingExperience=timesSmithed * .25;
    int money;
    int smithingExperienceModifier=timesSmithed * 12;
    if ( smithingExperience <= 24) {
        money=rand() % smithingExperienceModifier + 51;
    }
    else if ( smithingExperience >= 25 ) {
        money=rand() % smithingExperienceModifier + 201;
    }
    sprintf ( recvbuf, "Experience Gained: %d xp\nMoney Earned: %d dollars\nSmithing Level: %d\n", 25, money, smithingExperience);
    int wallet=currentMoney + money;
    return wallet;
}

int workTeaching ( char recvbuf[512], int timesTaught, int currentMoney ) {
    int teachingExperience=timesTaught * .25;
    int money;
    int teachingExperienceModifier=timesTaught * 11;
    if ( teachingExperience <= 24) {
        money=rand() % teachingExperienceModifier + 26;
    }
    else if ( teachingExperience >= 25 ) {
        money=rand() % teachingExperienceModifier + 151;
    }
    sprintf ( recvbuf, "Experience Gained: %d xp\nMoney Earned: %d dollars\nTeaching Level: %d\n", 25, money, teachingExperience);
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
    char recvbuf[512];
    int recvbuflen=100;
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
    int initalizeUsernames=0;
    int ranOnce=0;
    int arrayCopy;
    int emptyCheck=0;
    int usernameMatch=0;
    int usernameCheck=0;
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
            if (clientSocket != INVALID_SOCKET) {
                for (emptyCheck=0; emptyCheck < 25; emptyCheck++) {
                    while (fgets(username, 24, usernames) != NULL) fgets(username, 24, usernames);
                    if (strcmp(player1.playerUsername[emptyCheck], username) == 0) {
                        usernameMatch=1;
                        break;
                    }
                    if (strcmp(player1.playerUsername[emptyCheck], "") == 0 && ranOnce != 1 && usernameMatch != 1) {
                        strncpy(player1.playerUsername[emptyCheck], username, 24);
                        ranOnce=1;
                        break;
                    }
                }
            }
            returnWork=strcmp(outputTest, "$usernameTest");
            if (returnWork == 0) {
                for (int count=0; count < 25; count++) printf ("User[%d]: %s\n", count, player1.playerUsername[count]);
            }
            // usernames end
            // start work
            printf( "Bytes received: %d\n", result );
            outputTest=recvbuf;
            workNotSpecified=strcmp(outputTest, "$work");
            if (workNotSpecified == 0) {
                sprintf (recvbuf, "Pick from one of the following:\n $work.Mining\n $work.Teaching\n $work.Smithing\n");
            }
            returnWork=strcmp(outputTest, "$work.Mining");
            if (returnWork == 0) {
                end=time(NULL);
                timeSince=end - start;
                if ( timeSince >= 7200 ) {
                    player1.timesMined[emptyCheck]=player1.timesMined[emptyCheck] + 1;
                    player1.wallet[emptyCheck]=workMining( recvbuf, player1.timesMined[emptyCheck], player1.wallet[emptyCheck] );
                    start=time(NULL);
                }
                else {
                    timeSince=7200 - timeSince;
                    sprintf (recvbuf, "You are still tired, you can't mine for: %d seconds\n", timeSince );
                }
            }
            returnWork=strcmp(outputTest, "$work.Smithing");
            if (returnWork == 0) {
                end=time(NULL);
                timeSince=end - start;
                if ( timeSince >= 14400 ) {
                    player1.timesSmithed[emptyCheck]=player1.timesSmithed[emptyCheck] + 1;
                    player1.wallet[emptyCheck]=workSmithing( recvbuf, player1.timesSmithed[emptyCheck], player1.wallet[emptyCheck] );
                    start=time(NULL);
                }
                else {
                    timeSince=14400 - timeSince;
                    sprintf (recvbuf, "You are still tired, you can't start smithing again for: %d seconds\n", timeSince );
                }
            }
            returnWork=strcmp(outputTest, "$work.Teaching");
            if (returnWork == 0) {
                end=time(NULL);
                timeSince=end - start;
                if ( timeSince >= 10800 ) {
                    player1.timesTaught[emptyCheck]=player1.timesTaught[emptyCheck] + 1;
                    player1.wallet[emptyCheck]=workTeaching( recvbuf,player1.timesTaught[emptyCheck], player1.wallet[emptyCheck] );
                    start=time(NULL);
                }
                else {
                    timeSince=10800 - timeSince;
                    sprintf (recvbuf, "You are still tired, you can't start teaching again for: %d seconds\n", timeSince );
                }
            }
            // end of work
            // start of wallet
            returnWork=strcmp(outputTest, "$wallet");
            if (returnWork == 0) {
                sprintf (recvbuf, "You currently have %d dollars in your wallet.\n", player1.wallet[emptyCheck]);
            }
            // end of wallet 
            // start of bank
            returnWork=strcmp(outputTest, "$bank");
            if (returnWork == 0) {
                sprintf (recvbuf, "The teller asks if you would like to:\n   1) Deposit Money\n   2) Withdraw Money\n   3) Check Your Balance\nPlease enter $1, $2, or $3.\n");
            }
            returnWork=strcmp(outputTest, "$1");
            if (returnWork == 0) {
                sprintf (recvbuf, "How much would you like to deposit?\n The maximum amount you can deposit is: %d\n", player1.wallet[emptyCheck]);
                depositPrevious=1;
            }
            if (depositPrevious == 1) {
                invalidDeposit=atoi(outputTest);
                returnWork=strcmp(outputTest, "$1");
                if (invalidDeposit > player1.wallet[emptyCheck]) {
                    sprintf (recvbuf, "You cannot deposit more money than you have.\n");
                }
                else if (returnWork != 0) {
                    player1.bankMoney[emptyCheck]=invalidDeposit+player1.bankMoney[emptyCheck];
                    player1.wallet[emptyCheck]=player1.wallet[emptyCheck] - invalidDeposit;
                    sprintf (recvbuf, "You currently have %d dollars in the bank.\n", player1.bankMoney[emptyCheck]);
                    depositPrevious=0;
                }
            }
            returnWork=strcmp(outputTest, "$2");
            if (returnWork == 0) {
                if (bankMoney == 0) {
                    sprintf (recvbuf, "You have no money in the bank.");
                }
                else {
                    sprintf (recvbuf, "How much would you like to withdraw?\nThe most you can withdraw is: %d\n", player1.bankMoney[emptyCheck]);
                    withdrawPrevious=1;
                }
            }
            if (withdrawPrevious == 1) {
                withdrawMoney=atoi(outputTest);
                returnWork=strcmp(outputTest, "$2");
                if (player1.bankMoney[emptyCheck] < withdrawMoney) {
                    sprintf (recvbuf, "You cannot withdraw more than you have in the bank.\n");
                }
                else if (returnWork != 0) {
                    player1.bankMoney[emptyCheck]=player1.bankMoney[emptyCheck] - withdrawMoney;
                    player1.wallet[emptyCheck]=player1.wallet[emptyCheck] + withdrawMoney;
                    sprintf (recvbuf, "You now have %d in the bank.\n", player1.bankMoney[emptyCheck]);
                    withdrawPrevious=0;
                }
            }
            returnWork=strcmp(outputTest, "$3");
            if (returnWork == 0) {
                sprintf (recvbuf, "Your balance is: %d\n", player1.bankMoney[emptyCheck]);
            }
            // end of bank
            // end of new code
            iSendResult = send( clientSocket, recvbuf, recvbuflen, 0 );
            strncpy(recvbuf, "", 512);
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