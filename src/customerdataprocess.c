#include<cdrheader.h>

//Process the data of CDR file for Customer billing.
void *cdr_customer_data_process(void *cargs)
{
	long int n=0;
	struct user *CS=(struct user*)cargs;
	FILE *p_fp;
	p_fp=fopen("cdrdata.txt","r");
	if(p_fp == NULL)
	{
		return NULL;
	}
	else
	{

		char line[MAX_LINE_LEN]={0,};
		int i=0,j=0,token_count = 0;
		char tokens[9][MAX_LINE_LEN]={0,};

   	 	while (fgets(line,MAX_LINE_LEN,p_fp) != NULL)
		{
			for(i=0;line[i]!='\0';i++)
			{
				if(line[i]=='|')
				{
					tokens[token_count][j]='\0';
					token_count++;
					j=0;
					continue;
				}
				tokens[token_count][j]=line[i];
				j++;
			}

			n++;
        	strcpy(CS[n-1].msisdn, tokens[0]);
      	    strcpy(CS[n-1].op_brand_name, tokens[1]);
        	strcpy(CS[n-1].op_mmc, tokens[2]);
        	strcpy(CS[n-1].call_type, tokens[3]);
        	strcpy(CS[n-1].duration, tokens[4]);
        	strcpy(CS[n-1].download, tokens[5]);
        	strcpy(CS[n-1].upload, tokens[6]);
        	strcpy(CS[n-1].third_party_msisdn, tokens[7]);
        	strcpy(CS[n-1].third_party_mmc, tokens[8]);
			token_count=0;
			j=0;
    	}
    fclose(p_fp);
   }
   return NULL;
}

//Process the data of CDR file for Ineter operator billing.
void *cdr_operator_data_process(void *oargs)
{
	IP *op=(IP*)oargs;
	FILE *p_fp;
	p_fp=fopen("cdrdata.txt","r");
	if(p_fp == NULL)
	{
		printf("\nfile error\n");
		return NULL;
	}
	else
	{

		char line[MAX_LINE_LEN]={0,};
		int i=0,j=0,token_count = 0;
		long int in=0;
		char tokens[9][MAX_LINE_LEN]={0,};
		struct user *us=(struct user *)malloc(1*(sizeof(struct user)));
		while (fgets(line,MAX_LINE_LEN,p_fp) != NULL)
		{

			us=(struct user *)realloc(us,(in+1)*sizeof(struct user));
			for(i=0;line[i]!='\0';i++)
			{
				if(line[i]=='|')
				{
					tokens[token_count][j]='\0';
					token_count++;
					j=0;
					continue;
				}
				tokens[token_count][j]=line[i];
				j++;
			}

			in++;
			strcpy(us[in-1].msisdn, tokens[0]);
      	   	strcpy(us[in-1].op_brand_name, tokens[1]);
        	strcpy(us[in-1].op_mmc, tokens[2]);
        	strcpy(us[in-1].call_type, tokens[3]);
        	strcpy(us[in-1].duration, tokens[4]);
        	strcpy(us[in-1].download, tokens[5]);
        	strcpy(us[in-1].upload, tokens[6]);
        	strcpy(us[in-1].third_party_msisdn, tokens[7]);
        	strcpy(us[in-1].third_party_mmc, tokens[8]);

			token_count=0;
			j=0;
    	}

        //assigning operator codes
		strcpy(op[0].op_mmc,"42500");
		strcpy(op[1].op_mmc,"42501");
		strcpy(op[2].op_mmc,"42502");
		strcpy(op[3].op_mmc,"42503");
		strcpy(op[4].op_mmc,"42504");
		int k=0;
		while(k<5)
		{
			bzero(op[k].op_name,20);
			op[k].incoming_call=0;
			op[k].outgoing_call=0;
			op[k].incoming_sms=0;
			op[k].outgoing_sms=0;
			op[k].downloaded=0;
			op[k].uploaded=0;
			k++;
		}
	    char *mtc="MTC", *moc="MOC", *smsin="SMS-MT", *smsout="SMS-MO", *gprs="GPRS";
		k=0;
		while(k<in)
		{
			if(atol(us[k].op_mmc)==atol(op[0].op_mmc))
			{
				if(strcmp(us[k].call_type,mtc)==0)
				{
					op[0].incoming_call=op[0].incoming_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,moc)==0)
				{
					op[0].outgoing_call=op[0].outgoing_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,smsin)==0)
				{
					op[0].incoming_sms=op[0].incoming_sms+1;
				}
				else if(strcmp(us[k].call_type,smsout)==0)
				{
					op[0].outgoing_sms=op[0].outgoing_sms+1;
				}
				else if(strcmp(us[k].call_type,gprs)==0)
				{
					op[0].downloaded=op[0].downloaded+atol(us[k].download);
					op[0].uploaded=op[0].uploaded+atol(us[k].upload);
				}

				strcpy(op[0].op_name,us[k].op_brand_name);
				k++;
			}

			else if(atol(us[k].op_mmc)==atol(op[1].op_mmc))
			{
				if(strcmp(us[k].call_type,mtc)==1)
				{
					op[1].incoming_call=op[1].incoming_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,moc)==0)
				{
					op[1].outgoing_call=op[1].outgoing_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,smsin)==0)
				{
					op[1].incoming_sms=op[1].incoming_sms+1;
				}
				else if(strcmp(us[k].call_type,smsout)==0)
				{
					op[1].outgoing_sms=op[1].outgoing_sms+1;
				}
				else if(strcmp(us[k].call_type,gprs)==0)
				{
					op[1].downloaded=op[1].downloaded+atol(us[k].download);
					op[1].uploaded=op[1].uploaded+atol(us[k].upload);
				}

				strcpy(op[1].op_name,us[k].op_brand_name);
				k++;
			}
			else if(atol(us[k].op_mmc)==atol(op[2].op_mmc))
			{
				if(strcmp(us[k].call_type,mtc)==0)
				{
					op[2].incoming_call=op[2].incoming_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,moc)==0)
				{
					op[2].outgoing_call=op[2].outgoing_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,smsin)==0)
				{
					op[2].incoming_sms=op[2].incoming_sms+1;
				}
				else if(strcmp(us[k].call_type,smsout)==0)
				{
					op[2].outgoing_sms=op[2].outgoing_sms+1;
				}
				else if(strcmp(us[k].call_type,gprs)==0)
				{
					op[2].downloaded=op[2].downloaded+atol(us[k].download);
					op[2].uploaded=op[2].uploaded+atol(us[k].upload);
				}
				strcpy(op[2].op_name,us[k].op_brand_name);
				k++;
			}
			else if(atol(us[k].op_mmc)==atol(op[3].op_mmc))
			{
				if(strcmp(us[k].call_type,mtc)==0)
				{
					op[3].incoming_call=op[3].incoming_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,moc)==0)
				{
					op[3].outgoing_call=op[3].outgoing_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,smsin)==0)
				{
					op[3].incoming_sms=op[3].incoming_sms+1;
				}
				else if(strcmp(us[k].call_type,smsout)==0)
				{
					op[3].outgoing_sms=op[3].outgoing_sms+1;
				}
				else if(strcmp(us[k].call_type,gprs)==0)
				{
					op[3].downloaded=op[3].downloaded+atol(us[k].download);
					op[3].uploaded=op[3].uploaded+atol(us[k].upload);
				}
				strcpy(op[3].op_name,us[k].op_brand_name);
				k++;
			}
			else if(atol(us[k].op_mmc)==atol(op[4].op_mmc))
			{
				if(strcmp(us[k].call_type,mtc)==0)
				{
					op[4].incoming_call=op[4].incoming_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,moc)==0)
				{
					op[4].outgoing_call=op[4].outgoing_call+atol(us[k].duration);
				}
				else if(strcmp(us[k].call_type,smsin)==0)
				{
					op[4].incoming_sms=op[4].incoming_sms+1;
				}
				else if(strcmp(us[k].call_type,smsout)==0)
				{
					op[4].outgoing_sms=op[4].outgoing_sms+1;
				}
				else if(strcmp(us[k].call_type,gprs)==0)
				{
					op[4].downloaded=op[4].downloaded+atol(us[k].download);
					op[4].uploaded=op[4].uploaded+atol(us[k].upload);
				}
				strcpy(op[4].op_name,us[k].op_brand_name);
				k++;
			}

		}
		free(us);
	}
    fclose(p_fp);
	return NULL;

}

//Perform the data processing for both customer and inter operator using multi threading.
void cdr_data_process(struct user *CS,IP *IOS)
{

	pthread_t CDthread,IDthread;
	pthread_create(&CDthread,NULL,cdr_customer_data_process,(void *)CS);
	pthread_join(CDthread,NULL);
	pthread_cancel(CDthread);
	pthread_create(&IDthread,NULL,cdr_operator_data_process,(void *)IOS);
	pthread_join(IDthread,NULL);
	pthread_cancel(IDthread);

}


