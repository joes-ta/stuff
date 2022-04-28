#include <stdio.h>
#include <string.h>

char* usernameRead;
char username[24]="testuser1";
char userCheck[96];
char* firstName;

int main () {
    FILE *usernames=fopen( "projectUsers.txt", "r");
    fread(username, sizeof(char), 30, usernames);
    fgets(line, 96, usernames);
    printf ("User Check: %s\n", userCheck);
    usernameRead=strstr(username,userCheck);
    if (usernameRead == 0) {
        printf ("Using username testuser1, if you are not testuser1, please logout.\n");
    }
    else {
        printf ("Username not in use, values will be defaulted to 0.\n");
    }
    printf ("%s", username);
}