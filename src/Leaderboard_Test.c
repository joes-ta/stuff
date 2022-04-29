#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct player
{
    unsigned int playerid;
    int health;
    char Name[15];
    int wallet;
    long int bank;
};

typedef struct player Player;

//In main function, create int returnldr and also add returnldr in the see function (displays the appropriate players data)
void Action(int returnLeaderboard, int arrSize, Player* players)
{
    if (returnLeaderboard == 0)
    {
        //Temporarily stores the smaller of the two values checked, so it can be stored in i+1
        struct player tempPlayer;

        // allocate an array of players to match the size
        // of the incoming players
        Player* sortedPlayers = (Player*)malloc(sizeof(Player) * arrSize);

        if (sortedPlayers != NULL)
        {
            // Copy players
            memcpy(sortedPlayers, players, sizeof(Player) * arrSize);

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

            free(sortedPlayers);
        }
    }
}

int main()
{
    const int playerAmount = 15;
    int long PlayerBankData[15] = { 100, 30, 500, 50, 300, 29, 1001, 3883, 23432, 123, 100, 0, 3, 2, 4 };
    const char* Name[15] = { "Test1", "Test2", "Test3", "Test4", "Test5", "Test6", "Test7", "Test8","Test9", "Test10", "Test11", "Test12", "Test13", "Test14", "Test15" };

    struct player game_Players[15];

    int NameSize = sizeof(Name) / sizeof(Name[0]);
    //Populates game_Players structs with stored information
    for (int a = 0; a < playerAmount; a++)
    {
        game_Players[a].playerid = a;

        strcpy_s(game_Players[a].Name, NameSize, Name[a]);

        game_Players[a].bank = PlayerBankData[a];
    }
    //printf("%d", NameSize);
    int return_Leaderboard = 0;
    Action(return_Leaderboard, playerAmount, game_Players);
}