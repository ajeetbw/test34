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

#define MAX_BUFF 1024
#define PORT_NO 65534
#define MAX_LINE_LEN 1024

struct user
{
    char msisdn[20];
    char op_brand_name[64];
    char op_mmc[20];
    char call_type[10];
    char duration[10];
    char download[10];
    char upload[10];
    char third_party_msisdn[10];
    char third_party_mmc[10];
};

typedef struct interop
{
    char op_mmc[20];
    char op_name[20];
    long int incoming_call;
    long int outgoing_call;
    long int incoming_sms;
    long int outgoing_sms;
    long int downloaded;
    long int uploaded;
} IP;

int cdr_signup(char[], char[]);
int cdr_login(char[], char[]);
void cdr_dataprocess(struct user *, IP *);
char *cdr_customer_billing(struct user *, char[]);
char *cdr_customer_billing_file(struct user *, char[]);
char *cdr_interoperator_billing(IP *, char[]);
char *cdr_interoperator_billing_file(IP *);

#endif

