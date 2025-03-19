/*
 * COMP 3400: Template project driver
 *
 * Name: Samuel Costa and An Tran
 */

#include <assert.h>
#include <getopt.h>
#include <inttypes.h>
#include <netdb.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"

int cmdline (int, char **, char **, bool *, char **, short int *);

void
usage (void)
{
  printf ("Usage: net [options] domain\n");
  printf (" Options are:\n");
  printf ("  -w F    Connect to the domain and get file F with HTTP/1.0\n");
  printf ("  -6      Use IPv6 addresses\n");
  printf ("  -p P    Lookup address for application protocol P\n");
}

int
main (int argc, char **argv)
{
  char *protocol = "http";
  bool ipv6 = false;
  char *webfile = NULL;
  short int port = 0;
  if (cmdline (argc, argv, &protocol, &ipv6, &webfile, &port) < 0)
    {
      usage ();
      return EXIT_FAILURE;
    }
  char *domain = argv[optind];

  if (webfile == NULL)
    {
      // MINIMUM requirements:
      //   Get the server list for the specified domain and protocol
      //   Given this command-line:
      //     ./net -p http www.jmu.edu
      //   Print the following output:
      //     www.jmu.edu http: TCP IPv4 134.126.10.50
      //   The requested transport-layer protocol should be based on the
      //   requested application-layer protocol. TCP is default, but use
      //   UDP for protocols "53", "67", "dns", or "dhcp".
      struct addrinfo *server_list = NULL;
      
      if(strcmp(protocol, "53") == 0 || strcmp(protocol, "67") == 0 || strcmp(protocol, "dns") == 0 || strcmp(protocol, "dhcp") == 0)
      	server_list = get_server_list(domain, protocol, false, ipv6);
      else
      	server_list = get_server_list(domain, protocol, true, ipv6);
      char* string = serv_string(server_list);
      printf("%s %s: %s\n", domain, protocol, string);
      freeaddrinfo(server_list);
      free(string);
    }
  else
    {
    	
      // FULL requirements:
      //   Use web() to create a socket and retrieve the file headers.
      //   Print the result of web() as follows:
      //     "Result: HTTP/1.1 200 OK"
      //   If the type is also set, print it as follows:
      //     "Type: text/html; charset=UTF-8"
      char * type = NULL;
      char * result = web (domain, protocol, webfile, &type, ipv6);
      printf("Result: %s\n", result);
      printf("Type: %s\n", type);
      
      free(result);
      free(type);
    }

  return EXIT_SUCCESS;
}

/* DO NOT MODIFY THIS FUNCTION */

int
cmdline (int argc, char **argv, char **protocol, bool *ipv6, char **web,
         short int *port)
{
  int option;

  while ((option = getopt (argc, argv, "p:6w:sh")) != -1)
    {
      switch (option)
        {
          // Change this to merge -p and -o as same flag
        case 'p':
          *protocol = optarg;
          break;
        case '6':
          *ipv6 = true;
          break;
        case 'w':
          *web = optarg;
          break;
        case 'h':
          return -1;
          break;
        default:
          return -1;
        }
    }

  if (optind != argc - 1 && *web != NULL)
    return -1;

  return 0;
}
