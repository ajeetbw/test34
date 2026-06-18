#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_BUFF 1024
#define PORT_NO 65534
#define MAX_LINE_LEN 1024

// Structure to hold user data parsed from CDR file
struct user
{
    char msisdn[20];            // Mobile subscriber ISDN number
    char op_brand_name[64];     // Operator brand name
    char op_mmc[20];            // Operator MMC code
    char call_type[10];         // Type of call (MTC, MOC, SMS-MT, SMS-MO, GPRS)
    char duration[10];          // Call duration
    char download[10];          // Amount of data downloaded (for GPRS)
    char upload[10];            // Amount of data uploaded (for GPRS)
    char third_party_msisdn[10];// Third party ISDN number
    char third_party_mmc[10];   // Third party MMC code
};

// Structure to hold inter-operator billing data
typedef struct interop
{
    char op_mmc[20];        // Operator MMC code
    char op_name[20];       // Operator name
    long int incoming_call; // Total incoming call duration
    long int outgoing_call; // Total outgoing call duration
    long int incoming_sms;  // Total incoming SMS count
    long int outgoing_sms;  // Total outgoing SMS count
    long int downloaded;    // Total data downloaded (for GPRS)
    long int uploaded;      // Total data uploaded (for GPRS)
} IP;


// Process the data of CDR file for Customer billing.
void *cdr_customer_data_process(void *cargs)
{
    long int n = 0;	
    struct user *CS = (struct user*)cargs;
    FILE *p_fp;
    p_fp = fopen("cdrdata.txt", "r");
    if (p_fp == NULL)
    {
        // Print error message if file cannot be opened
        fprintf(stderr, "Error opening file cdrdata.txt\n");
        return NULL;
    }
    else
    {	
        char line[MAX_LINE_LEN] = {0,};
        int i = 0, j = 0, token_count = 0;
        char tokens[9][MAX_LINE_LEN] = {0,};
		
        // Read each line from the file
        while (fgets(line, MAX_LINE_LEN, p_fp) != NULL) 
        {	
            // Parse line by '|' delimiter to extract tokens
            for (i = 0; line[i] != '\0'; i++)
            {
                if (line[i] == '|')
                {
                    tokens[token_count][j] = '\0';
                    token_count++;
                    j = 0;
                    continue;
                }
                tokens[token_count][j] = line[i];
                j++;
            }

            n++;		
            // Copy parsed tokens into struct user array
            strcpy(CS[n - 1].msisdn, tokens[0]);
            strcpy(CS[n - 1].op_brand_name, tokens[1]);
            strcpy(CS[n - 1].op_mmc, tokens[2]);
            strcpy(CS[n - 1].call_type, tokens[3]);
            strcpy(CS[n - 1].duration, tokens[4]);
            strcpy(CS[n - 1].download, tokens[5]);
            strcpy(CS[n - 1].upload, tokens[6]);
            strcpy(CS[n - 1].third_party_msisdn, tokens[7]);
            strcpy(CS[n - 1].third_party_mmc, tokens[8]);

            token_count = 0;
            j = 0;
        }
        fclose(p_fp);
    }
    return NULL;
}

// Process the data of CDR file for Inter operator billing.
void *cdr_operator_data_process(void *oargs)
{
    IP *op = (IP*)oargs;
    FILE *p_fp;
    p_fp = fopen("cdrdata.txt", "r");
    if (p_fp == NULL)
    {
        // Print error message if file cannot be opened
        printf("\nFile error: cdrdata.txt\n");
        return NULL;
    }
    else
    {
        char line[MAX_LINE_LEN] = {0,};
        int i = 0, j = 0, token_count = 0;
        long int in = 0;
        char tokens[9][MAX_LINE_LEN] = {0,};
        struct user *us = (struct user *)malloc(1 * sizeof(struct user));
		
        // Read each line from the file
        while (fgets(line, MAX_LINE_LEN, p_fp) != NULL)
        {
            // Resize memory dynamically to accommodate new user data
            us = (struct user *)realloc(us, (in + 1) * sizeof(struct user));
            for (i = 0; line[i] != '\0'; i++)
            {
                if (line[i] == '|')
                {
                    tokens[token_count][j] = '\0';
                    token_count++;
                    j = 0;
                    continue;
                }
                tokens[token_count][j] = line[i];
                j++;
            }

            in++;
            // Copy parsed tokens into struct user array
            strcpy(us[in - 1].msisdn, tokens[0]);
            strcpy(us[in - 1].op_brand_name, tokens[1]);
            strcpy(us[in - 1].op_mmc, tokens[2]);
            strcpy(us[in - 1].call_type, tokens[3]);
            strcpy(us[in - 1].duration, tokens[4]);
            strcpy(us[in - 1].download, tokens[5]);
            strcpy(us[in - 1].upload, tokens[6]);
            strcpy(us[in - 1].third_party_msisdn, tokens[7]);
            strcpy(us[in - 1].third_party_mmc, tokens[8]);

            token_count = 0;
            j = 0;
        }

        // Assigning operator MMC codes
        strcpy(op[0].op_mmc, "42500");
        strcpy(op[1].op_mmc, "42501");
        strcpy(op[2].op_mmc, "42502");
        strcpy(op[3].op_mmc, "42503");
        strcpy(op[4].op_mmc, "42504");

        // Initialize inter-operator billing data
        int k = 0;
        while (k < 5)
        {
            bzero(op[k].op_name, 20);  // Clear op_name
            op[k].incoming_call = 0;
            op[k].outgoing_call = 0;
            op[k].incoming_sms = 0;
            op[k].outgoing_sms = 0;
            op[k].downloaded = 0;
            op[k].uploaded = 0;
            k++;
        }

        // Define call types for comparison
        char *mtc = "MTC", *moc = "MOC", *smsin = "SMS-MT", *smsout = "SMS-MO", *gprs = "GPRS";
        k = 0;
        while (k < in)
        {
            if (atol(us[k].op_mmc) == atol(op[0].op_mmc))
            {
                if (strcmp(us[k].call_type, mtc) == 0)
                {
                    op[0].incoming_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, moc) == 0)
                {
                    op[0].outgoing_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, smsin) == 0)
                {
                    op[0].incoming_sms++;
                }
                else if (strcmp(us[k].call_type, smsout) == 0)
                {
                    op[0].outgoing_sms++;
                }
                else if (strcmp(us[k].call_type, gprs) == 0)
                {
                    op[0].downloaded += atol(us[k].download);
                    op[0].uploaded += atol(us[k].upload);
                }

                strcpy(op[0].op_name, us[k].op_brand_name);
                k++;
            }

            else if (atol(us[k].op_mmc) == atol(op[1].op_mmc))
            {
                if (strcmp(us[k].call_type, mtc) == 1)
                {
                    op[1].incoming_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, moc) == 0)
                {
                    op[1].outgoing_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, smsin) == 0)
                {
                    op[1].incoming_sms++;
                }
                else if (strcmp(us[k].call_type, smsout) == 0)
                {
                    op[1].outgoing_sms++;
                }
                else if (strcmp(us[k].call_type, gprs) == 0)
                {
                    op[1].downloaded += atol(us[k].download);
                    op[1].uploaded += atol(us[k].upload);
                }

                strcpy(op[1].op_name, us[k].op_brand_name);
                k++;
            }
            else if (atol(us[k].op_mmc) == atol(op[2].op_mmc))
            {
                if (strcmp(us[k].call_type, mtc) == 0)
                {
                    op[2].incoming_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, moc) == 0)
                {
                    op[2].outgoing_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, smsin) == 0)
                {
                    op[2].incoming_sms++;
                }
                else if (strcmp(us[k].call_type, smsout) == 0)
                {
                    op[2].outgoing_sms++;
                }
                else if (strcmp(us[k].call_type, gprs) == 0)
                {
                    op[2].downloaded += atol(us[k].download);
                    op[2].uploaded += atol(us[k].upload);
                }
                strcpy(op[2].op_name, us[k].op_brand_name);
                k++;
            }
            else if (atol(us[k].op_mmc) == atol(op[3].op_mmc))
            {
                if (strcmp(us[k].call_type, mtc) == 0)
                {
                    op[3].incoming_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, moc) == 0)
                {
                    op[3].outgoing_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, smsin) == 0)
                {
                    op[3].incoming_sms++;
                }
                else if (strcmp(us[k].call_type, smsout) == 0)
                {
                    op[3].outgoing_sms++;
                }
                else if (strcmp(us[k].call_type, gprs) == 0)
                {
                    op[3].downloaded += atol(us[k].download);
                    op[3].uploaded += atol(us[k].upload);
                }
                strcpy(op[3].op_name, us[k].op_brand_name);
                k++;
            }
            else if (atol(us[k].op_mmc) == atol(op[4].op_mmc))
            {
                if (strcmp(us[k].call_type, mtc) == 0)
                {
                    op[4].incoming_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, moc) == 0)
                {
                    op[4].outgoing_call += atol(us[k].duration);
                }
                else if (strcmp(us[k].call_type, smsin) == 0)
                {
                    op[4].incoming_sms++;
                }
                else if (strcmp(us[k].call_type, smsout) == 0)
                {
                    op[4].outgoing_sms++;
                }
                else if (strcmp(us[k].call_type, gprs) == 0)
                {
                    op[4].downloaded += atol(us[k].download);
                    op[4].uploaded += atol(us[k].upload);
                }
                strcpy(op[4].op_name, us[k].op_brand_name);
                k++;
            }
        }
        free(us);
    }
    fclose(p_fp);
    return NULL;
}

// Perform the data processing for both customer and inter operator using multi threading.
void cdr_data_process(struct user *CS, IP *IOS)
{
    pthread_t CDthread, IDthread;
    pthread_create(&CDthread, NULL, cdr_customer_data_process, (void *)CS);
    pthread_join(CDthread, NULL);
    pthread_cancel(CDthread);
    pthread_create(&IDthread, NULL, cdr_operator_data_process, (void *)IOS);
    pthread_join(IDthread, NULL);
    pthread_cancel(IDthread);
}

int main()
{
    // Allocate memory for user and interoperator structures
    struct user *users = (struct user *)malloc(11 * sizeof(struct user)); // 11 user entries present in test data
    IP *interoperators = (IP *)malloc(5 * sizeof(IP));  // 5 interoperators present

    if (users == NULL || interoperators == NULL)
    {
        // Print error if memory allocation fails
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    // Initialize the structures 
    for (int i = 0; i < 10; ++i)
    {
        users[i].msisdn[0] = '\0';  // Initialize msisdn to mark it as empty
    }

    for (int i = 0; i < 5; ++i)
    {
        strcpy(interoperators[i].op_mmc, "42500");  // Initialize op_mmc for interoperators
        interoperators[i].op_name[0] = '\0';        // Initialize op_name
        interoperators[i].incoming_call = 0;
        interoperators[i].outgoing_call = 0;
        interoperators[i].incoming_sms = 0;
        interoperators[i].outgoing_sms = 0;
        interoperators[i].downloaded = 0;
        interoperators[i].uploaded = 0;
    }

    // Call the data processing function
    cdr_data_process(users, interoperators);

    // Print the results for users
    printf("User data:\n");
    for (int i = 0; i < 11; ++i)
    {
        if (users[i].msisdn[0] == '\0')  // Assume empty msisdn indicates end of data
            break;
        printf("User %d:\n", i + 1);
        printf("MSISDN: %s\n", users[i].msisdn);
        printf("Operator Brand Name: %s\n", users[i].op_brand_name);
        printf("Operator MMC: %s\n", users[i].op_mmc);
        printf("Call Type: %s\n", users[i].call_type);
        printf("Duration: %s\n", users[i].duration);
        printf("Download: %s\n", users[i].download);
        printf("Upload: %s\n", users[i].upload);
        printf("Third Party MSISDN: %s\n", users[i].third_party_msisdn);
        printf("Third Party MMC: %s\n", users[i].third_party_mmc);
        printf("\n");
    }

    // Print the results for interoperators
    printf("Interoperator data:\n");
    for (int i = 0; i < 5; ++i)
    {
        printf("Interoperator %d:\n", i + 1);
        printf("Operator MMC: %s\n", interoperators[i].op_mmc);
        printf("Operator Name: %s\n", interoperators[i].op_name);
        printf("Incoming Call: %ld\n", interoperators[i].incoming_call);
        printf("Outgoing Call: %ld\n", interoperators[i].outgoing_call);
        printf("Incoming SMS: %ld\n", interoperators[i].incoming_sms);
        printf("Outgoing SMS: %ld\n", interoperators[i].outgoing_sms);
        printf("Downloaded: %ld\n", interoperators[i].downloaded);
        printf("Uploaded: %ld\n", interoperators[i].uploaded);
        printf("\n");
    }

    // Free allocated memory
    free(users);
    free(interoperators);

    return 0;
}

