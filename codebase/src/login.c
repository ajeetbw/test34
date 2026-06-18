#include<cdrheader.h>

// Validate the login credentials using user_details.txt file.
int cdr_login(char user_name[], char password[])
{
    char exist_user_name[MAX_LINE_LEN];
    char exist_password[MAX_LINE_LEN];
    FILE *fp = fopen("../data/user_details.txt", "r");
    


    
    if (fp == NULL)
    {
        printf("\nSorry !!! Try again Later\n");
        exit(EXIT_FAILURE);
    }
    
    int flag = 0;
    
    while (fscanf(fp, "%s %s\n", exist_user_name, exist_password) == 2)
    {
        if (strcmp(user_name, exist_user_name) == 0 && strcmp(password,exist_password)==0)
        {
            flag = 1;
            break;
	}
    }
    
    fclose(fp);
    return flag;
}

