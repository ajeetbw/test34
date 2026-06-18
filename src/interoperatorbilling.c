#include<my_header.h>

 
//Display the Inter operator billing data.

char * cdr_interoperator_billing(IP *IOS, char mmc[])

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
 
 
 
