#include <stdio.h>
#include <string.h>

struct player
{
    unsigned int playerid;
    int health;
    char Name[15];
    int wallet;
    long int bank;
};

//In main function, create int returnldr and also add returnldr in the see function (displays the appropriate players data)
void Action(int returnLeaderboard, int arrSize, player players[15])
{
    player sortedPlayers[15];

    if (returnLeaderboard == 0)
    {
        //Temporarily stores the smaller of the two values checked, so it can be stored in i+1
        player tempPlayer;

        bool isSame=true;
        for (int j = 0; j < arrSize; j++)
        {
            sortedPlayers[j] = players[j];
        }
                                                                                                                                                                                                                                                  
        for (int i = 0; i < arrSize; i++)
        {
            //k will always be one more than i, which allows comparison of two elements in the array
            for (int k = i + 1; k < arrSize; k++)
            {
                //Sorts player struct based on balances
                if (sortedPlayers[i].bank < sortedPlayers[k].bank)
                {
                    tempPlayer = sortedPlayers[i];
                    sortedPlayers[i] = sortedPlayers[k];
                    sortedPlayers[k] = tempPlayer;
                }
            }
        }


        printf("Leaderboard:\n+---------------------------------------------------+\n");
        for (int i = 0; i < arrSize; i++)
        {
            printf("|Balance: %-10d|PlayerID: %-5d|Name: %-8s |\n", sortedPlayers[i].bank, sortedPlayers[i].playerid, sortedPlayers[i].Name);
        }
        printf("+---------------------------------------------------+\n");
    }
}

int main()
{
    int const playerAmount = 15;
    int long PlayerBankData[15] = { 100, 30, 500, 50, 300, 29, 1001, 3883, 23432, 123, 100, 0, 3, 2, 4 };
    const char* Name[15] = {"Test1", "Test2", "Test3", "Test4", "Test5", "Test6", "Test7", "Test8","Test9", "Test10", "Test11", "Test12", "Test13", "Test14", "Test15"};
    
    int arrSize = playerAmount;
    
    struct player game_Players[playerAmount];
    //Populates game_Players structs with stored information
    for (int a = 0; a < playerAmount; a++)
    {
        game_Players[a].playerid = a;
        strcpy_s(game_Players[a].Name, Name[a]);
        game_Players[a].bank = PlayerBankData[a];
    }

    int return_Leaderboard = 0;
    Action(return_Leaderboard, arrSize, game_Players);
}