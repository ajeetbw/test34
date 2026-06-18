#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for exit()

#define MAX_LINE_LENGTH 80

// Function to validate the login credentials using userdetails.txt file
int cdr_login(char user_name[], char password[])
{
    char exist_user_name[MAX_LINE_LENGTH];
    char exist_password[MAX_LINE_LENGTH];
    FILE *fp = fopen("../data/userdetails.txt", "r");

    if (fp == NULL)
    {
        printf("\nSorry !!! Unable to open userdetails.txt file.\n");
        exit(EXIT_FAILURE);
    }

    int flag = 0;

    while (fscanf(fp, "%s %s\n", exist_user_name, exist_password) == 2)
    {
        if (strcmp(user_name, exist_user_name) == 0 && strcmp(password, exist_password) == 0)
        {
            flag = 1;
            break;
        }
    }

    fclose(fp);
    return flag;
}

int main()
{
    char username[MAX_LINE_LENGTH];
    char password[MAX_LINE_LENGTH];

    while (1) // Infinite loop until login is successful
    {
        printf("Enter username: ");
        if (scanf("%s", username) != 1)
        {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        printf("Enter password: ");
        if (scanf("%s", password) != 1)
        {
            printf("Invalid input. Please try again.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        // Validate login credentials
        int authenticated = cdr_login(username, password);

        if (authenticated)
        {
            printf("Login successful!\n");
            break; // Exit the loop if login is successful
        }
        else
        {
            printf("Invalid username or password. Please try again.\n");
        }
    }

    return 0;
}

