#include <arpa/inet.h>
#include <assert.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"

#define BUFFER_MAX_SIZE 4096

/* Given a struct with server information, return a dynamically allocated
   string of the IP address in dotted decimal or colon hexadecimal notation.
   Consult Chapter 4, as well as /usr/include/netdb.h and
   /usr/include/arpa/inet.h as needed. Use inet_ntop() to get the formatted
   string based on the address's ai_addr field.
 */
char *
addr_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");
  else
  {
  	char* buffer = NULL;
  	
		if(server -> ai_family == AF_INET6)
		{
			struct sockaddr_in6 *addr = (struct sockaddr_in6 *)server -> ai_addr;
			
			// Allocate buffer for the IP address
			buffer = calloc(INET6_ADDRSTRLEN, sizeof(char));
			inet_ntop(AF_INET6, &addr->sin6_addr, buffer, INET6_ADDRSTRLEN);
		}
		else //if IP version is equal to IPv4
		{
			struct sockaddr_in *addr = (struct sockaddr_in *) server->ai_addr;
			
			// Allocate buffer for the IP address
			buffer = calloc(INET_ADDRSTRLEN, sizeof(char));
			inet_ntop(AF_INET, &addr->sin_addr, buffer, INET_ADDRSTRLEN);
		}
		return buffer;
  }
}

/* Given the server address info, return a dynamically allocated string with
   its transport-layer protocol, IP version, and IP address. For instance,
   querying otterbein.edu over TCP/IPv4 should return:
     "TCP IPv4 205.133.226.138"
   Use addr_string() to get the formatted address string, concatenate it to
   the string to return, then free the result from addr_string(). If the
   passed server parameter is NULL, use malloc() and strcpy() to return a
   dynamically allocated copy of "no address information".
 */
char *
serv_string (struct addrinfo *server)
{
  // Return safely in case server is NULL:
  if (server == NULL)
    return strdup ("no address information");

  // Allocate 10 bytes for protocol (TCP or UDP), IP version (IPv4 or
  // IPv6), along with spaces and the ending null byte. In addition,
  // include enough space for an IP address (either v4 or v6).
  
  char * str = addr_string(server);
  
  // Allocating 10 bytes + the length of the IP adress string
  char* buffer = (char *) calloc(10 + strlen(str), 1);
 
  // Now, use strncat() to build the string based on information from
  // the server struct.
  
  if(server->ai_socktype == SOCK_STREAM)
  	strcat(buffer, "TCP ");
  else
  	strcat(buffer, "UDP ");
  
  if(server->ai_family == AF_INET6)
		strcat(buffer, "IPv6 ");
  else
		strcat(buffer, "IPv4 ");
	
	strcat(buffer, str);
	free(str);
	
	return buffer;
}

/* Given a hostname string, use getaddrinfo() to query DNS for the specified
   protocol parameters. Boolean values indicate whether or not to use IPv6
   (as opposed to IPv4) or TCP (as opposed to UDP). For this assignment, the
   protocol will only be "http", though it could be others in a more complete
   implementation. Return the pointer to the linked list of server results.
 */
struct addrinfo *
get_server_list (const char *hostname, const char *proto, bool tcp, bool ipv6)
{
  struct addrinfo *server_list = NULL;
  struct addrinfo hints;
	memset (&hints, 0, sizeof (hints));
	
	if(tcp)
	{
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
	}
	else
	{
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
	}
	
	if(ipv6)
		hints.ai_family = AF_INET6;
	else
		hints.ai_family = AF_INET;
		
	getaddrinfo(hostname, proto, &hints, &server_list);	

	return server_list;
}

/* Use an HTTP/1.0 request to retrieve the requested file with a GET request.
   Return the first line of the header and the content type. To get the type,
   use strtok() to split the lines of the header at "\n". Then look for the
   line that beging "Content-Type: ". Set the call-by-reference paramter type
   to be the remainder of that line. Be sure to trim off the "\r" at the end.
 */
char *
web (const char *hostname, char *proto, const char *file, char **type,
     bool ipv6)
{
  // To start, call get_server_list(), then loop through the results trying
  // to create a socket connection. When a connection is successfully
  // established, send "GET file HTTP/1.0\r\n\r\n" for the file specified.
  // Write the request to the socket and read the result into the buffer.
  bool tcp = true;
  if(!strcmp(proto, "53") || !strcmp(proto, "67") || !strcmp(proto, "DNS") || !strcmp(proto, "DHCP"))
  {
  	tcp = false;
  } 
  struct addrinfo *server_list = get_server_list(hostname, proto, tcp, ipv6);
  struct addrinfo *server = NULL;
  int socketfd = 0;
 	int conn = 0;
 	
 	int ai_family = ipv6 ? AF_INET6 : AF_INET;
 	int ai_socktype = tcp ? SOCK_STREAM : SOCK_DGRAM;
 	
 	for (server = server_list; server != NULL; server = server->ai_next)
 	{
 		if (server->ai_family == ai_family && server->ai_socktype == ai_socktype)
 		{
 			break;
 		}
 	}
 	
 	if (ipv6)
 	{
 		socketfd = socket(AF_INET6, SOCK_STREAM, 0);
 	}
 	
 	else
 	{
 		socketfd = socket(AF_INET, SOCK_STREAM, 0);
 	}
 	conn = connect(socketfd, server->ai_addr, server->ai_addrlen);
 	freeaddrinfo(server_list);
 	
 	
 	if (socketfd < 0 || conn < 0)
 	{
 		return NULL;
 	}
 	
 	
 	int length = snprintf(NULL, 0, "GET %s HTTP/1.0\r\n\r\n", file);
  char * string = malloc(length+1);
  snprintf(string, length+1, "GET %s HTTP/1.0\r\n\r\n", file);
  
  write (socketfd, string, strlen(string));
  free(string);
  
  char buffer[BUFFER_MAX_SIZE];  // Should not need more that this
  memset (buffer, 0, sizeof (buffer));
  
  ssize_t bytes = read (socketfd, buffer, sizeof(buffer));
  
  if (bytes < 0)
  {
  	return NULL;
  }
  
  char* line = buffer;
  
  char *eol = strstr (line, "\r\n");
  eol[0] = '\0';
  char* result = strdup(line);
  
	//size_t body_length = 0;
	while (eol != NULL) // While there are more CRLFs
	{ 
		eol[0] = '\0'; // Null-terminate each line
		// Find content length
		
		if (! strncmp (line, "Content-Type: ", 14))
		{ 
			char *len = strchr (line, ' ') + 1;
			*type = strdup(strtok(len, "\r\n"));
		}
		line = eol + 2; // Move to the next line
		eol = strstr (line, "\r\n");
  }
  
  return result;
}
