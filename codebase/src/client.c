#include<cdrheader.h>

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
#define DEBUG_DEBUG(msg) fprintf(stderr, "[DEBUG] %s\n", msg)
#else
#define DEBUG_DEBUG(msg)
#endif



int main(int argc, char const * argv[]) {
    int client_fd,fflag=0;
    struct sockaddr_in serv_addr;

    char  msg[MAX_BUFF];

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_NO);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("\n SOCKET CREATION ERROR \n");
      return -1;
    }

    if ((connect(client_fd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr))) < 0) {
      printf("connection failed \n");
      return -1;
    }
    DEBUG_INFO("\n Client connected to the server \n");

   while(1){
       system("clear");
		bzero(msg,MAX_BUFF);
		printf("------------------> CALL DATA RECORD <------------------\n");
		printf("\n\n1. Signup\n2. Login\n3. exit\nchoice: ");

		bzero(msg,MAX_BUFF);
		scanf("%s",msg);
		if(atoi(msg)==1)      // signup section
		{
			write(client_fd,msg,strlen(msg));
			bzero(msg,MAX_BUFF);
			read(client_fd,msg,MAX_BUFF);     //enter username
			printf("\n\n%s ",msg);
			bzero(msg,MAX_BUFF);
			scanf("%s",msg);
			write(client_fd,msg,strlen(msg));
			bzero(msg,MAX_BUFF);
			read(client_fd,msg,MAX_BUFF);      //Enter password
			printf("\n%s ",msg);
			bzero(msg,MAX_BUFF);
			scanf("%s",msg);
			write(client_fd,msg,strlen(msg));
			bzero(msg,MAX_BUFF);
			read(client_fd,msg,MAX_BUFF);     //success or fail
			if(strcmp(msg,"1")==0)
			{
				DEBUG_INFO("\n\nSign up successful!\n\n");
				printf("\nPlease login now!\n\n");
			}
			else
			{
				DEBUG_ERROR("\n\nSign up failed!\n\n");
			}
			sleep(3);
		}
		else if(atoi(msg)==2)   //login section
		{
			write(client_fd,msg,strlen(msg));
			bzero(msg,MAX_BUFF);
			read(client_fd,msg,MAX_BUFF);  // enter username
			printf("\n\n%s ",msg);
			bzero(msg,MAX_BUFF);
			scanf("%s",msg);
			write(client_fd,msg,strlen(msg));
			bzero(msg,MAX_BUFF);
			read(client_fd,msg,MAX_BUFF);     //enter password
			printf("\n%s ",msg);
			bzero(msg,MAX_BUFF);
			scanf("%s",msg);
			write(client_fd,msg,strlen(msg));
			bzero(msg,MAX_BUFF);
			read(client_fd,msg,MAX_BUFF);     // success  or  fail
			if(atoi(msg)==1)
			{
				DEBUG_INFO("\n\nLog in successful!\n\n");
				sleep(3);
				while(1)
				{
					system("clear");
					printf("\n---------------> PROCESSING MENU <----------------");
					printf("\n\n1.Process CDR file\n2.Print/Search for Billing Information\n3.Logout\nchoice:");
					bzero(msg,MAX_BUFF);
					scanf("%s",msg);
					if(atoi(msg)==1 || atoi(msg)==2)
					{
						if(atoi(msg)==1)    // data processing
						{
						
							write(client_fd,msg,strlen(msg));
							read(client_fd,msg,MAX_BUFF);  //data processing status
							if(atoi(msg)==1)
							{
								fflag=1;
								DEBUG_INFO("\n\nProcessing data successful!\n\n");
								sleep(3);
							}
							else if(atoi(msg)==2)
							{
								DEBUG_ERROR("\n\nFailed to process the data!\n\n");
								sleep(3);
							}
							else
							{
								fflag=1;
								DEBUG_INFO("\n\nData already processed!\n\n");
								sleep(3);
							}
						}
						else       //Search/print section
						{
							if(fflag==0)
							{
								DEBUG_WARNING("\n\nPlease process the data first!\n\n");
								sleep(3);
							}
							else
							{
								write(client_fd,msg,strlen(msg));
								bzero(msg,MAX_BUFF);
								read(client_fd,msg,MAX_BUFF);  //Customer or inter
								printf("%s",msg);
								bzero(msg,MAX_BUFF);
								scanf("%s",msg);
								if(atoi(msg)==1)    //customer section
								{
									write(client_fd,msg,strlen(msg));
									bzero(msg,MAX_BUFF);
									read(client_fd,msg,MAX_BUFF);    // customer billing options
									system("clear");
									printf("%s",msg);
									bzero(msg,MAX_BUFF);
									scanf("%s",msg);
									write(client_fd,msg,strlen(msg));
									if(atoi(msg)==1)  //display section
									{
										bzero(msg,MAX_BUFF);
										read(client_fd,msg,MAX_BUFF); // enter msisdn
										printf("%s ",msg);
										bzero(msg,MAX_BUFF);
										scanf("%s",msg);
										write(client_fd,msg,strlen(msg));
										bzero(msg,MAX_BUFF);
										read(client_fd,msg,MAX_BUFF);   // output of customerbilling function
										if(strcmp(msg,"User not exist!")==0)
										{
											DEBUG_ERROR("\n\nUser not exist!\n\n");
											sleep(3);
										}
										else
										{
											system("clear");
											printf("\n\nCustomer Billing:\n");
											printf("\n\n%s\n\n",msg);
											sleep(10);
										}
									}	
									else if(atoi(msg)==2)   //download section
									{
										bzero(msg,MAX_BUFF);
										read(client_fd,msg,MAX_BUFF);   // output of customerbillingfile function
										if(strcmp(msg,"failed")==0)
										{
											printf("\n\nWrong MSISDN.\n\n");
											sleep(3);
										}
										else
										{
											system("clear");
											printf("\n\nCustomer Billing file Downloaded successfully!\n\n");
											sleep(5);
										}
									}
									else
									{
										DEBUG_WARNING("\n\nWrong choice!\n\n");
										sleep(3);
									}
								}
								else if(atoi(msg)==2)   //inter section
								{
									write(client_fd,msg,strlen(msg));
									bzero(msg,MAX_BUFF);
									read(client_fd,msg,MAX_BUFF);   // inter billing options
									system("clear");
									printf("%s",msg);
									bzero(msg,MAX_BUFF);
									scanf("%s",msg);
									write(client_fd,msg,strlen(msg));
									if(atoi(msg)==1)  //display section
									{
										bzero(msg,MAX_BUFF);
										read(client_fd,msg,MAX_BUFF);      //Enter operator code
										printf("%s ",msg);
										bzero(msg,MAX_BUFF);
										scanf("%s",msg);
										write(client_fd,msg,strlen(msg));
										bzero(msg,MAX_BUFF);
										read(client_fd,msg,MAX_BUFF);     // output of interoperatorbilling function
										if(strcmp(msg,"Inter operator not found!")==0)
										{
											printf("\n\nInter operator not Found!\n\n");
											sleep(3);
										}
										else
										{
											system("clear");
											printf("\n\nInter operator Billing:\n");
											printf("%s\n\n",msg);
											sleep(10);
										}
									}
									else if(atoi(msg)==2)  //download section
									{
										bzero(msg,MAX_BUFF);
										read(client_fd,msg,MAX_BUFF);    // output of interoperatorbillingfile function
										if(strcmp(msg,"no")==0)

										{
											printf("\n\nFailed!\n\n");
											sleep(3);
										}
										else
										{
											system("clear");
											printf("\n\nInter Operator Billing Downloaded Successful!\n\n");
											sleep(5);
										}
									}
									else
									{
										DEBUG_WARNING("\n\nWrong Inter operator code.\n\n");
										sleep(3);
									}
								}
								else
								{
									DEBUG_WARNING("\n\nWrong choice!\n\n");
									bzero(msg,MAX_BUFF);
									sleep(3);
								}
							}
						}
					}
					else if(atoi(msg)==3)   // logout section
					{
						write(client_fd,msg,MAX_BUFF);
						DEBUG_INFO("\n\nLog out successful!\n\n");
						bzero(msg,MAX_BUFF);
						sleep(3);
						system("clear");
						break;
					}
					else
					{
						DEBUG_WARNING("\n\nWrong choice!\n\n");
						sleep(3);
						bzero(msg,MAX_BUFF);
						system("clear");
					}
				}
			}
			else
			{
				DEBUG_ERROR("\n\nLogin failed!\n\n");
				sleep(3);
				bzero(msg,MAX_BUFF);
				system("clear");
			}
		}
		else if(atoi(msg)==3)    //exit section
		{
			write(client_fd,msg,strlen(msg));
			printf("\n\nThankyou!\n\n");
			system("clear");
			break;
		}
		else
		{
			bzero(msg,MAX_BUFF);
			DEBUG_WARNING("\n\nWrong choice!\n\n");
			sleep(3);
			system("clear");
		}
   }

    close(client_fd);
    return 0;
}
