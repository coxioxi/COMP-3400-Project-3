#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_LENGTH 8192

#define CRLF "\r\n"

/* Used to execute a given CGI program in a separate process. Uses the
   version parameter to build the HTTP header. Other parameters are used
   for the FULL implementation only as follows:

     if method is "GET":
       Use query as the value of the QUERY_STRING environment variable.

     if method is "POST":
       Use boundary to split the body of the request (see the project
       page for an example). The size parameter is only used if the POST
       request is for uploading a file, and is the size of the data to
       be uploaded.
 */
char *
cgi_response (char *uri, char *version, char *method, char *query,
              ssize_t size, char *boundary, char *body)
{
	
  // TODO [PART]: If the URI exists and is executable, run it as a separate
  // process, redirecting its STDOUT back to this process. You can then use
  // that resulting string to determine the Content-Length to send back. As
  // an example, here would be the full response to return for
  // cgi-bin/hello.cgi (assuming HTTP/1.0 and CRLF means "\r\n"). Don't print
  // the quotes:
  //   "HTTP/1.0 200 OK" CRLF
  //   "Content-Type: text/html; charset: UTF-8" CRLF
  //   "Content-Length: 95" CRLF
  //   CRLF
  //   "<html>\n"
  //   "<head>\n"
  //   "  <title>Hello world demo</title>\n"
  //   "</head>\n"
  //   "\n"
  //   "<h2>Hello world!</h2>\n"
  //   "</body>\n"
  //   "</html>\n"
  
  char* response = NULL;
  if (uri != NULL)
	{
	
		int pipes[2];
		pipe(pipes);
		pid_t child = fork();
		
		if (child == 0)
		{
			close(pipes[0]);
			dup2(pipes[1], STDOUT_FILENO);
			
			if (!strcmp(method, "GET"))
			{
				if (query)
				{
					char *env = getenv ("QUERY_STRING");
					char queryString[1024]; 
					strcpy(queryString, "QUERY_STRING=");
					strcat(queryString, query);
					execle(queryString, queryString, NULL, env);
				}
				else
				{
					execl(uri, uri, NULL);
				}
			}
			
			else if (!strcmp(method, "POST"))
			{
				
				
			}
			//execl(uri, uri, NULL);
			exit(1);
			
		}
		
		close(pipes[1]);
		
		char buffer[BUFFER_LENGTH];
  	memset (&buffer, 0, BUFFER_LENGTH);
  		
		ssize_t bytes = read(pipes[0], buffer, BUFFER_LENGTH);
		
		if (bytes < 0)
		{
			return NULL;
		}
		ssize_t length;
		
		if (!strcmp(version, "HTTP/1.0"))
		{
			length = snprintf(NULL, 0, "%s 200 OK\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: %ld\r\n\r\n", version, bytes);
				
			response = malloc(length + 1);
				
			snprintf(response, length+1, "%s 200 OK\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: %ld\r\n\r\n", version, bytes);
		}
		else if (!strcmp(version, "HTTP/1.1"))
		{
			length = snprintf(NULL, 0, "%s 200 OK\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: %ld\r\n"
			"Connection: close\r\n\r\n", version, bytes);
				
			response = malloc(length + 1);
				
			snprintf(response, length+1, "%s 200 OK\r\n"
			"Content-Type: text/html; charset=UTF-8\r\n"
			"Content-Length: %ld\r\n"
			"Connection: close\r\n\r\n", version, bytes);
		}
		char contents[BUFFER_LENGTH];
		strcpy(contents, buffer);
		
		response = realloc(response, length + bytes + 1);
		response = strncat(response, contents, length + bytes + 1);
		
	}
	else
	{
		response = strdup ("HTTP/1.0 404 Not Found" CRLF CRLF);
	}
  return response;

  // TODO [FULL]: Set the environment variables needed for the CGI programs
  // located in cgi-bin. To do this, you will need to use either execve()
  // or execle() when running the CGI program, using an array of string
  // pairs. For example, the following array would set the db and record
  // environment variables:
  //
  //   char *env[] = { "db=foo.txt", "record=2", NULL };
  //
  // If the request is a GET request, you should only set the QUERY_STRING
  // variable to be the query parameter. For POST requests, you will need
  // to look through the body of the HTTP request, splitting based on the
  // boundary values (see the project description for an example).
  
}
