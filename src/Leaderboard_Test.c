#include <iostream>

//Every person has a specified place in original array, so to show correct name besides the leaderboard,
// I will have to search the original leaderboard for the displayed balance, then use case statement to
// determine which element matches, and check who's element that is
//---------------------------------


//IF THE BALANCE IS THE SAME- Go through amount of repeated balances, and for each display a different name of someone with that balance


//In main function, create int returnldr and also add returnldr in the see function (displays the appropriate players data)
void Action(int returnLeaderboard, int PlayerBankData[15], int arrSize, std::string PlayerName[15])
{
    int SortedBankData[15];

    if (returnLeaderboard == 0)
    {
        //Temporarily stores the smaller of the two values checked, so it can be stored in i+1
        int temporaryData;

        for (int j = 0; j < arrSize; j++)
        {
            SortedBankData[j] = PlayerBankData[j];
        }

        for (int i = 0; i < arrSize; i++)
        {
            //k will always be one more than i, which allows comparison of two elements in the array
            for (int k = i + 1; k < arrSize; k++)
            {
                if (SortedBankData[i] < SortedBankData[k])
                {
                    temporaryData = SortedBankData[i];
                    SortedBankData[i] = SortedBankData[k];
                    SortedBankData[k] = temporaryData;
                }
            }
        }

        for (int n = 0; n < arrSize; n++)
        {
            //std::cout << PlayerName[n];
            //printf("%s\n", PlayerName[n]);
            printf(" % d\n", SortedBankData[n]);
        }
    }
}

int main()
{
    int PlayerBankData[15] = { 100, 30, 500, 50, 300, 29, 1001, 3883, 289398, 123, 12, 0, 3, 2, 4 };
    std::string PlayerName[15] = { "John", "Logan", "Matt", "Joe", "Bob", "Bart", "Homer", "Flower", "heh", "Test", "Test1", "Test2", "Test3", "Test4", "Test5" };
    int arrSize = sizeof(PlayerBankData) / sizeof(PlayerBankData[0]);
    int return_Leaderboard = 0;
    Action(return_Leaderboard, PlayerBankData, arrSize, PlayerName);
}