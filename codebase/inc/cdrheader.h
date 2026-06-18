#ifndef cdrheader_h
#define cdrheader_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_BUFF 1024      // Maximum size for buffer
#define PORT_NO 65534      // Port number for socket communication
#define MAX_LINE_LEN 1024  // Maximum length of a line

// Structure to hold user CDR data
struct user
{
    char msisdn[20];                // Mobile number
    char op_brand_name[64];         // Operator brand name
    char op_mmc[20];                // Operator MMC
    char call_type[10];             // Type of call (e.g., local, international)
    char duration[10];              // Call duration
    char download[10];              // Data download amount
    char upload[10];                // Data upload amount
    char third_party_msisdn[10];    // Third-party mobile number
    char third_party_mmc[10];       // Third-party MMC
};

// Structure to hold inter-operator CDR data
typedef struct interop
{
    char op_mmc[20];        // Operator MMC
    char op_name[20];       // Operator name
    long int incoming_call; // Count of incoming calls
    long int outgoing_call; // Count of outgoing calls
    long int incoming_sms;  // Count of incoming SMS
    long int outgoing_sms;  // Count of outgoing SMS
    long int downloaded;    // Amount of data downloaded
    long int uploaded;      // Amount of data uploaded
} IP;

// Function declarations
int cdr_signup(char[], char[]);                             // Sign up a CDR user
int cdr_login(char[], char[]);                              // Login for CDR user
void cdr_dataprocess(struct user *, IP *);                  // Process user CDR data
char *cdr_customer_billing(struct user *, char[]);          // Generate customer billing
char *cdr_customer_billing_file(struct user *);     // Generate customer billing and store in file
char *cdr_interoperator_billing(IP *, char[]);              // Generate inter-operator billing
char *cdr_interoperator_billing_file(IP *);                 // Generate inter-operator billing and store in file

#endif
