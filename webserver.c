/* webserver.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cnaiapi.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

#if defined(LINUX) || defined(SOLARIS)
#include <sys/time.h>
#endif

#define BUFFSIZE	99999
#define BYTE		1024

int recvln(connection, char *, int);
int main(int argc, char *argv[]) {

	connection	conn;
	int n,
	
	/* Add page, data, and port variables for extra functions. */
	page, data, port;
	char buff[BUFFSIZE], cmd[16], path[99999], vers[16],
	
	/* Add data_send and reqln arrays for flexability */
	data_send[BUFFSIZE], *reqln[3];
	
	char		*rootdir;
#if defined(LINUX) || defined(SOLARIS)
	struct timeval	tv;
#elif defined(WIN32)
	time_t          tv;
#endif


	/* Set the defaults for the root directory and the port number. */
	rootdir = getcwd(0, 0);
	port = 8080;

	
	/* Process the command line flags. This was done so to test
	multiple directories for sites instead of having one exe in
	each directory.	*/
	
	for(int i = 1; i < argc; i += 2) {
		if(strcmp(argv[i], "-d") == 0) {
			rootdir = malloc(strlen(argv[i + 1]));
			rootdir = argv[i + 1];
		}
		if(strcmp(argv[i], "-p") == 0) {
			port = atoi(argv[i + 1]);
		}
	}

	while(1) {

		conn = await_contact((appnum) port);
		if (conn < 0)
			exit(1);

		memset((void*) buff, (int)'\0', BUFFSIZE);
		
		
		n = recvln(conn, buff, BUFFSIZE);
		sscanf(buff, "%s %s %s", cmd, path, vers);
		
		/* Copy the command, path and protocol so that there is no error
		to the original string. */
		
		reqln[0] = cmd;
		reqln[1] = path;
		reqln[2] = vers;


		if (n < 1) {
			(void) send_eof(conn);
			continue;
		}
		
		if ((strcmp(vers, "HTTP/1.0")) && (strcmp(vers, "HTTP/1.1"))) {
			(void) send_eof(conn);
			continue;
		}
		
		
		/* Use the reqln array to get a more flexable command route to work with. */
		
		reqln[0] = strtok(buff, " \t\n");
		if(strncmp(reqln[0], "GET\0", 4) == 0) {
			
			reqln[1] = strtok(NULL, " \t");
			reqln[2] = strtok(NULL, " \t\n");
			printf("%s ===>> ", reqln[0]);
			
			/* If root directory is specified by '/' default to index.html*/
			
			if (strncmp(reqln[1], "/\0", 2) == 0) {
				reqln[1] = "/index.html";
			}
			
			/* Copy the root directory path to the file path and then append
			the actual file name, if there is one. */
			
			strcpy(path, rootdir);
			strcpy(&path[strlen(rootdir)], reqln[1]);
			printf("file: %s\n\n", path);
			
			/* If the file exists in the path, open it */
			
			if((page = open(path, O_RDONLY)) != -1) {
				send(conn, "HTTP/1.0 200 OK\n\n", 17, 0);
				
				
				/* While the file is being read, send the data that is read. */
				
				while((data = read(page, data_send, BUFFSIZE)) > 0) {
						send(conn, data_send, data, 0);
				}
			}
			
				/* Otherwise, read the 404 file for error by the same subroutine. */
			
			else {
				reqln[1] = "/404.html";
				strcpy(path, rootdir);
				strcpy(&path[strlen(rootdir)], reqln[1]);
				printf("file: %s\n\n", path);
				page = open(path, O_RDONLY);
				send(conn, "HTTP/1.0 200 OK\n\n", 17, 0);
				while((data = read(page, data_send, BUFFSIZE)) > 0) {
						send(conn, data_send, data, 0);
				}
			}
		 (void) send_eof(conn);
		}
	} 
	return 0; 
}
