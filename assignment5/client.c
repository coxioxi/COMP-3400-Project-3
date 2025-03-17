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
  	char* buffer = calloc(INET6_ADDRSTRLEN, sizeof(char));
  	
		if(server -> ai_family = AF_INET6)
		{
			struct sockaddr_in6 *addr = (struct sockaddr_in6 *)server -> ai_addr;
			assert(inet_ntop(AF_INET6, &addr->sin6_addr, buffer, INET6_ADDRSTRLEN) != NULL);
			
			return buffer;
		}
		
		if (server->ai_family == AF_INET)
		{
			struct sockaddr_in *addr = (struct sockaddr_in *) server->ai_addr;
			buffer = inet_ntoa(addr->sin_addr);
			
			return buffer;
		}
  }

  return NULL;
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

  // Now, use strncat() to build the string based on information from
  // the server struct.
  return NULL;
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
  // struct addrinfo *server_list = NULL;
  // struct addrinfo hints;
  // memset (&hints, 0, sizeof (hints));
  return NULL;
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
  char buffer[BUFFER_MAX_SIZE];  // Should not need more that this
  memset (buffer, 0, sizeof (buffer));
  return NULL;
}
