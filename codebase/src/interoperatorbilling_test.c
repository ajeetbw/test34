
//#include<my_header.h>

/*

#ifndef cdrheader_h

#define cdrheader_h

*/

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

}IP;
 
void structure_input(IP op[])

{

     strcpy(op[0].op_mmc,"42504");

    strcpy(op[0].op_name,"4");

    op[0].incoming_call=862576;

    op[0].outgoing_call=855646;

    op[0].incoming_sms=3905;

    op[0].outgoing_sms=3998;

    op[0].downloaded=435195;

    op[0].uploaded= 422003;

    strcpy(op[1].op_mmc,"42503");

    strcpy(op[1].op_name,"Airtel");

    op[1].incoming_call=853500;

    op[1].outgoing_call=882069;

    op[1].incoming_sms=3990;

    op[1].outgoing_sms=3986;

    op[1].downloaded=429235;

    op[1].uploaded= 432627;

     strcpy(op[2].op_mmc,"42501");

    strcpy(op[2].op_name,"Partner");

    op[2].incoming_call=883043;

    op[2].outgoing_call=851048;

    op[2].incoming_sms=3940;

    op[2].outgoing_sms=4017;

    op[2].downloaded=439487;

    op[2].uploaded= 428699;

}
 
//Display the Inter operator billing data.

char * cdr_interoperator_billing(IP IOS[], char mmc[])

{

	int k=0;                                             // for indexing

	int flag=0;

	char * temp_datastorage=(char *)malloc(MAX_BUFF);    //for storing the formatted strings until usage

	bzero(temp_datastorage,MAX_BUFF);                    // Initialize temp_datastorage buffer with zeros  

	while(k<3)

	{

		if(atol(IOS[k].op_mmc)==atol(mmc))               // Check if operator MMC matches given mmc

		{

			// Construct the formatted string for the operator's billing data

			char dum[MAX_BUFF]={0,};

			strcat(temp_datastorage,"\n\nOperator Brand : ");

			strcat(temp_datastorage,IOS[k].op_name);

			strcat(temp_datastorage," (");

			strcat(temp_datastorage,mmc);

			strcat(temp_datastorage,")");

			strcat(temp_datastorage,"\nIncoming voice call duration: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",IOS[k].incoming_call);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nOutgoing voice call duration: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",IOS[k].outgoing_call);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nIncoming SMS messages: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",IOS[k].incoming_sms);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nOutgoing SMS messages: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",IOS[k].outgoing_sms);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nMB downloaded: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",IOS[k].downloaded);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage," | MB uploaded: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",IOS[k].uploaded);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\n\n");
 
			flag=1;                                                            // Set flag to indicate operator was found

			break;

		}

		k++;                                                                   // Move to the next element in IOS array

	}

	if(flag==1)

	{

			return temp_datastorage;                                       // Return formatted billing data

	}

	else

	{

		strcpy(temp_datastorage,"Operator not found!");                   // Set message if operator was not found
 
	 	return temp_datastorage;                                           // Return error message

	}

	return temp_datastorage;

}
 
//Download the processed Inter operator data in IOSB.txt file.

char * cdr_interoperator_billing_file(IP *op)

{

	FILE *p_fp=NULL;       //file pointer

	p_fp=fopen("demo_test.txt","w+");                            //opening file in writting mode

    char * temp_datastorage=(char *)malloc(2048);                // initializing variable for temporary storage

	bzero(temp_datastorage,2048);                                

	int flag=0;

	if(p_fp==NULL)

	{

		perror("fopen()");

	}

	else

	{

		int z=0;

		strcpy(temp_datastorage,"\n\nInter operator billing!\n");

		strcat(temp_datastorage,"<----------------------------->\n");

		while(z<3)                               

		{

		    // Constructing the formatted string for each operator

			char dum[MAX_BUFF]={0,};

			strcat(temp_datastorage,"\n\nOperator Brand : ");

			strcat(temp_datastorage,op[z].op_name);

			strcat(temp_datastorage," (");

			strcat(temp_datastorage,op[z].op_mmc);

			strcat(temp_datastorage,")");

			strcat(temp_datastorage,"\nIncoming voice call duration: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",op[z].incoming_call);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nOutgoing voice call duration: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",op[z].outgoing_call);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nIncoming SMS messages: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",op[z].incoming_sms);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nOutgoing SMS messages: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",op[z].outgoing_sms);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\nMB downloaded: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",op[z].downloaded);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage," | MB uploaded: ");

			bzero(dum,MAX_BUFF);

			sprintf(dum,"%ld",op[z].uploaded);

			strcat(temp_datastorage,dum);

			strcat(temp_datastorage,"\n\n");

			z++;

			flag=1;

		}

	}

	if(flag==1)                                            // Write formatted string to file

	{

		fprintf(p_fp,"%s",temp_datastorage);             // Write the content of temp_datastorage to the file pointed by p_fp

		bzero(temp_datastorage,MAX_BUFF);                 // Clear the temp_datastorage buffer

		strcpy(temp_datastorage,"yes");                  // Set temp_datastorage to "yes"
 
		fclose(p_fp);

		return temp_datastorage;

	}

	else

	{

		fclose(p_fp);
 
		bzero(temp_datastorage,MAX_BUFF);                  // Clear the temp_datastorage buffer               

		strcpy(temp_datastorage,"no");                    // Set temp_datastorage to "no"

		return temp_datastorage;                           // Return temp_datastorage

	}
 
	return temp_datastorage;
 
}
 
 
int main()

{

    FILE *p_fp=NULL;

    IP op[3];

    char *str1;

   // bzero(str1,)

    structure_input(op);

      str1= cdr_interoperator_billing(op,"42501");

    printf("\n\n %s\n", str1);

     str1= cdr_interoperator_billing(op,"42503");

    printf("\n\n %s\n", str1);

    str1= cdr_interoperator_billing(op,"00000");

    printf("\n\n %s\n", str1);

    char *str=cdr_interoperator_billing_file(op);

    printf("\n %s \n", str);

    p_fp= fopen("demo_test.txt", "r+");

    char *dum = (char *)malloc(2048);

    bzero(dum, 2048);
 
    if (p_fp == NULL) 

    { 

        printf("Cannot open file \n"); 

        exit(0); 

    } 

    // Read contents from file 

    while (fgets(dum,2048,p_fp)!= NULL) 

    { 

        printf ("%s", dum); 

        dum = fgets(dum,2048,p_fp);

    }

    fclose(p_fp);

 
}
 
