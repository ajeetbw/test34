// C file to handle server which basically includes the socket creation, binding and then do the process of listening and accept.

#include <cdrheader.h>


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


//Create the server for the CDR application.
int main()
{
	int sfd = 0, retValue=0, csfd=0;
	unsigned int clientAddlen = 0;


	struct User *CS=NULL;
	IP *IOS=NULL;
	
	int dataFlag=0;

	struct sockaddr_in serv_address, client_address;

	char msg[MAXBUFF] = {0,};

	sfd = socket(AF_INET,SOCK_STREAM,0);

	if(sfd < 0)
	{
		perror("socket() ");
		exit(EXIT_FAILURE);
	}
	DEBUG_INFO("\nSocket created\n");


	memset(&serv_address,'\0',sizeof(serv_address));

	serv_address.sin_family = AF_INET;
	serv_address.sin_port = htons(PORTNO);
	serv_address.sin_addr.s_addr = inet_addr("127.0.0.1");

	retValue = bind(sfd, (struct sockaddr *)&serv_address,sizeof(serv_address));
	
	if(retValue < 0)
	{
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	DEBUG_INFO("\nBinded the server\n");

	retValue = listen(sfd, 5);
	
	if(retValue < 0)
	{
		perror("listen() ");
		exit(EXIT_FAILURE);
	}
	
	DEBUG_INFO("\nListening to the clients\n");
	clientAddlen=sizeof(client_address);

	while(1)
	{
		csfd = accept(sfd,(struct sockaddr *)&client_address,&clientAddlen);

		if(csfd < 0)
		{
			perror("accept() ");
			exit(EXIT_FAILURE);
		}
		DEBUG_INFO("\nGot a new client connection.\n");
		char uid[MAXBUFF]={0,};
		char upass[MAXBUFF]={0,};
		
		while(1)
		{
			bzero(msg,MAXBUFF);
			read(csfd,msg,MAXBUFF);
			DEBUG_DEBUG("Message received from client: %s", msg);
      			if(atoi(msg)==3)
			{
				close(csfd);
				break;
			}
			else if(atoi(msg) ==1)
			{
		
			}
			else if(atoi(msg)==2)
			{

      } 
    }
  }
  return 0;
}
