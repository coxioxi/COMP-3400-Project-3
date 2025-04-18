#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "utils.h"

#define WHITESPACE " \t\r\n"

/* Basic framework for a web server than can handle a single request, then
   shuts down. Looks for files in the srv_root directory.
 */
char *
serve_web (char *protocol)
{
  // Pass the protocol (should be a port number) to
  // setup_server() to get the socket.
  int socketfd = setup_server (protocol);
  if (socketfd < 0)
    return NULL;

  // Indicate (for debugging) that the server is running
  fprintf (stderr, "Server is started\n");

  int connection = -1;
  char *address = get_connection (socketfd, &connection);
  if (address == NULL)
    return NULL;
  printf ("Received incoming request from %s\n", address);

  // TODO: Read the request from the socket into the buffer provided above.
  // If the call to read() fails, shutdown and close both file descriptors
  // and return NULL.
  char buffer[BUFFER_SIZE];
  memset (&buffer, 0, BUFFER_SIZE);

//ssize_t bytes = 0;
	ssize_t bytes = read(connection, buffer, BUFFER_SIZE);
	
	if (bytes < 0)
	{
		shutdown (socketfd, SHUT_RDWR);
		close(socketfd);
		
		shutdown (connection, SHUT_RDWR);
		close(connection);
		return NULL;
	}

  // TODO: Look at the first line of the request to get the URI and the
  // HTTP version. For MIN: anything other than a GET request for
  // "/index.html" should be considered a non-existent file that will
  // later cause a 404 Not Found. For FULL: allow the URI to be "/" (which
  // you will convert to "/index.html") or "/bootstrap.html". HINT: You
  // can use strtok() to tokenize the line read (i.e., break it into parts).
  // Specifically, the first call to strtok (buffer, " "); would return a
  // pointer to "GET". Then, calling it as strtok (NULL, " "); would return
  // a pointer to the URI. For the third call, replace the second argument
  // with "\r", as you want to split the buffer at the carriage return.
  
  char *version = NULL;
  char *uri = NULL;

	char *token = strtok (buffer, WHITESPACE);
	
	if (!strncmp (token, "GET", 4))
  {
  	token = strtok (NULL, WHITESPACE);
  	uri = token;
  	
  	token = strtok (NULL, "\r");
  	version = token;
  }
  
  if (!strcmp(uri, "/"))
  	{
  		uri = "/index.html";
  	}
  
  printf ("GET Request for %s using %s\n", uri, version);
 
  // Prepend "srv_root" to the beginnging of the URI to get the full
  // URI location, such as srv_root/index.html.
  char *uri_loc = strdup ("srv_root");
  size_t length = strlen (uri_loc) + strlen (uri);
  uri_loc = realloc (uri_loc, length + 1);
  strncat (uri_loc, uri, length - strlen (uri_loc));

  char *contents = NULL;
  char *header = NULL;
  
  header = build_response (uri_loc, version, &contents);
  
  
  // TODO: If the value returned from build_response() is not NULL, then
  // write the header and contents into the socket. If the returned header
  // is NULL, send a 404 based on the HTTP version (HTTP/1.1 only required
  // for FULL):
  //      "HTTP/1.0 404 Not Found\r\n\r\n";
  //      "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";

	if (header)
	{
		write(connection, header, strlen(header));
		write(connection, contents, strlen(contents));
	}
	else
	{
		if (!strcmp(version, "HTTP/1.0"))
		{
		 	header = strdup("HTTP/1.0 404 Not Found\r\n\r\n");
		 	write(connection, header, strlen(header));
  	}
  	else
  	{
  		header = strdup("HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n");
  		write(connection, header, strlen(header));
  	}
	}
  // TODO: Free the headers and contents, shutdown and close both
  // sockets, return the URI of the request (possibly modified to
  // include "index.html", but not including the srv_root).
  
  free(header);
  free(contents);
  
  shutdown (socketfd, SHUT_RDWR);
  close(socketfd);
	
	shutdown (connection, SHUT_RDWR);
  close(connection);
  
  
  return strdup(uri);
}
