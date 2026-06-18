// C file to handle server which basically includes the socket creation, binding and then do the process of listening and accept.
#include <cdrheader.h>

#include<time.h>

#define LOG_FILE_PATH "../data/server_log.txt"

// Debug levels
#define DEBUG_LEVEL_FATAL 1
#define DEBUG_LEVEL_INFO  2
#define DEBUG_LEVEL_WARNING 3
#define DEBUG_LEVEL_DEBUG  4
 



// Set the current debug level here
#define DEBUG_LEVEL DEBUG_LEVEL_DEBUG
 
// Debug macros
#if DEBUG_LEVEL >= DEBUG_LEVEL_ERROR
#define DEBUG_ERROR(msg) fprintf(stderr, "[ERROR] %s\n", msg)
#else
#define DEBUG_ERROR(msg)
#endif
 
#if DEBUG_LEVEL >= DEBUG_LEVEL_INFO
#define DEBUG_INFO(msg) fprintf(stderr, "[INFO] %s\n", msg)
#else
#define DEBUG_INFO(msg)
#endif
 
#if DEBUG_LEVEL >= DEBUG_LEVEL_WARNING
#define DEBUG_WARNING(msg) fprintf(stderr, "[WARNING] %s\n", msg)
#else
#define DEBUG_WARNING(msg)
#endif
 
#if DEBUG_LEVEL >= DEBUG_LEVEL_DEBUG
#define DEBUG_DEBUG(fmt, ...) fprintf(stderr, "[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
#define DEBUG_DEBUG(fmt, ...)
#endif

void write_to_log(const char* msg){
        FILE *lf=fopen(LOG_FILE_PATH,"a+");
        if(lf !=NULL){
                time_t c_time;
                char* time_string;
                c_time=time(NULL);
                time_string=ctime(&c_time);
                time_string[strlen(time_string)-1]='\0';
                fprintf(lf,"[%s] %s\n",time_string,msg);
                fclose(lf);
        }
        else{
                perror("Failed to open log file");
                exit(EXIT_FAILURE);
        }
}


//Create the server for the CDR application.
int main()
{
	int sfd = 0, retValue=0, csfd=0;
	unsigned int clientAddlen = 0;


	struct user *CS=NULL;
	IP *IOS=NULL;
	
	int dataFlag=0;

	struct sockaddr_in serv_address, client_address;

	char msg[MAX_BUFF] = {0,};

	sfd = socket(AF_INET,SOCK_STREAM,0);

	if(sfd < 0)
	{
		perror("socket() ");
		write_to_log("failed to create socket\n");
		exit(EXIT_FAILURE);
	}
	write_to_log("Server started\n");
	DEBUG_INFO("\nSocket created\n");


	memset(&serv_address,'\0',sizeof(serv_address));

	serv_address.sin_family = AF_INET;
	serv_address.sin_port = htons(PORT_NO);
	serv_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	retValue = bind(sfd, (struct sockaddr *)&serv_address,sizeof(serv_address));
	
	if(retValue < 0)
	{
		perror("bind()");
		write_to_log("failed to bind socket\n");
		exit(EXIT_FAILURE);
	}

	DEBUG_INFO("\nBinded the server\n");
	write_to_log("Server Binded successfully\n");

	retValue = listen(sfd, 5);
	
	if(retValue < 0)
	{
		perror("listen() ");
		write_to_log("listen failure");
		exit(EXIT_FAILURE);
	}
	
	DEBUG_INFO("\nListening to the clients\n");
	write_to_log("Listening to the clients\n");
	clientAddlen=sizeof(client_address);

	while(1)
	{
		csfd = accept(sfd,(struct sockaddr *)&client_address,&clientAddlen);

		if(csfd < 0)
		{
			perror("accept() ");
			 write_to_log("client accept failed\n");
			exit(EXIT_FAILURE);
		}
		DEBUG_INFO("\nGot a new client connection.\n");
		 write_to_log("Got a new client connection.\n");
		char uid[MAX_BUFF]={0,};
		char upass[MAX_BUFF]={0,};
		
		while(1)
		{
			bzero(msg,MAX_BUFF);
			read(csfd,msg,MAX_BUFF);
			DEBUG_DEBUG("Message received from client: %s", msg);
			if(atoi(msg)==3)
			{
				close(csfd);
				break;
			}
			else if(atoi(msg) ==1)
			{
				bzero(msg,MAX_BUFF);
				strcpy(msg,"Enter username:");
				write(csfd,msg,strlen(msg));
				bzero(msg,MAX_BUFF);
				read(csfd,uid,MAX_BUFF);
				bzero(msg,MAX_BUFF);
				strcpy(msg,"Enter password:");
				write(csfd,msg,strlen(msg));
				bzero(msg,MAX_BUFF);
				read(csfd,upass,MAX_BUFF);
				FILE *fp = NULL;
				fp = fopen("../data/user_details.txt","a+");
				if(fp == NULL)
				{
					printf("Sorry,,please try after some time");
					 write_to_log("user details file opening error\n");
					exit(EXIT_FAILURE);
				}
				if(cdr_signup(uid,upass))
				{
					fprintf(fp,"%s %s\n",uid,upass);
					bzero(uid,MAX_BUFF);
					bzero(upass,MAX_BUFF);
					bzero(msg,MAX_BUFF);
					strcpy(msg,"1");
					fclose(fp);
					write(csfd,msg,strlen(msg));
				}
				else
				{
					bzero(msg,MAX_BUFF);
					bzero(uid,MAX_BUFF);
					bzero(upass,MAX_BUFF);
					strcpy(msg,"0");	
					fclose(fp);
					write(csfd,msg,strlen(msg));
				}
			}
			else if(atoi(msg)==2)
			{
				strcpy(msg,"Enter username:");
				write(csfd,msg,strlen(msg));
				bzero(msg,MAX_BUFF);
				read(csfd,uid,MAX_BUFF);
				bzero(msg,MAX_BUFF);
				strcpy(msg,"Enter password:");
				write(csfd,msg,strlen(msg));
				read(csfd,upass,MAX_BUFF);
				int k=cdr_login(uid,upass);
				bzero(uid,MAX_BUFF);
				bzero(upass,MAX_BUFF);
				if(k==1)
				{
					bzero(msg,MAX_BUFF);
					strcpy(msg,"1");
					write(csfd,msg,strlen(msg));
					bzero(msg,MAX_BUFF);
					while(1)
					{
						read(csfd,msg,MAX_BUFF);
						if(atoi(msg)==1)
						{
							bzero(msg,MAX_BUFF);

							if(dataFlag==0)
							{
								
								CS=(struct user*)malloc(100000*(sizeof(struct user)));
								IOS=(IP *)malloc(5*(sizeof(IP)));
								cdr_dataprocess(CS,IOS);
								if(CS!=NULL && IOS!=NULL)
								{
									dataFlag=1;
									strcpy(msg,"1");
									write(csfd,msg,strlen(msg));
								}
								else
								{
									strcpy(msg,"2");
									write(csfd,msg,strlen(msg));
								}
							}
							else
							{
								strcpy(msg,"3");
								write(csfd,msg,strlen(msg));
							}
						}
						else if(atoi(msg)==2)
						{
							bzero(msg,MAX_BUFF);
							strcpy(msg,"\n\n1.Customer Billing\n2.Inter operator  Billing\nChoice: ");
							write(csfd,msg,strlen(msg));
							bzero(msg,MAX_BUFF);
							read(csfd,msg,MAX_BUFF);
							if(atoi(msg)==1)
							{
								bzero(msg,MAX_BUFF);
								strcpy(msg,"\n---------------> CUSTOMER BILLING <--------------\n\n");
								strcat(msg,"1.Display on screen\n2.Download the  CB.txt\nChoice: ");
								write(csfd,msg,strlen(msg));
								bzero(msg,MAX_BUFF);
								read(csfd,msg,MAX_BUFF);
								if(atoi(msg)==1)
								{
									bzero(msg,MAX_BUFF);
									strcpy(msg,"\n\nEnter MSISDN:");
									write(csfd,msg,strlen(msg));
									bzero(msg,MAX_BUFF);
									read(csfd,msg,MAX_BUFF);
									char msisdn[MAX_BUFF]={0,};
									strcpy(msisdn,msg);
									bzero(msg,MAX_BUFF);
									char * customerAck=cdr_customer_billing(CS,msisdn);
									bzero(msisdn,MAX_BUFF);
									strcpy(msg,customerAck);
									free(customerAck);
									write(csfd,msg,strlen(msg));

								}
								else if(atoi(msg)==2)
								{
									bzero(msg,MAX_BUFF);
									char * customerAck=cdr_customer_billing_file(CS);
									strcpy(msg,customerAck);
									free(customerAck);
									write(csfd,msg,strlen(msg));
								}

							}
							else if(atoi(msg)==2)
							{
								bzero(msg,MAX_BUFF);
								strcpy(msg,"\n----------------> INTER OPERATOR BILLING <---------------");
								strcat(msg,"\n\n1.Display result on the user screen\n2.Download  IOSB.txt\nChoice : ");
								write(csfd,msg,strlen(msg));
								bzero(msg,MAX_BUFF);
								read(csfd,msg,MAX_BUFF);
								if(atoi(msg)==1)
								{
									bzero(msg,MAX_BUFF);
									strcpy(msg,"\n\nEnter Operator Code :");
									write(csfd,msg,strlen(msg));
									bzero(msg,MAX_BUFF);
									read(csfd,msg,MAX_BUFF);
									char msisdn[MAX_BUFF]={0,};
									strcpy(msisdn,msg);
									bzero(msg,MAX_BUFF);
									char * customerAck=cdr_interoperator_billing(IOS,msisdn);
									strcpy(msg,customerAck);
									bzero(msisdn,MAX_BUFF);
									free(customerAck);
									write(csfd,msg,strlen(msg));

								}
								else if(atoi(msg)==2)
								{
									bzero(msg,MAX_BUFF);
									char * customerAck=cdr_interoperator_billing_file(IOS);
									strcpy(msg,customerAck);
									free(customerAck);
									write(csfd,msg,strlen(msg));
								}
								
							}
						}
						else if(atoi(msg)==3)
						{
							free(CS);
							free(IOS);
							CS=NULL;
							IOS=NULL;
							break;
						}
					}
				}
			
				else
				{
					bzero(msg,MAX_BUFF);
					strcpy(msg,"2");
					write(csfd,msg,strlen(msg));
				}
			
			}
		}

	}

	return 0;
}
