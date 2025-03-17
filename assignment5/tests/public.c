#include <arpa/inet.h>
#include <assert.h>
#include <check.h>
#include <fcntl.h>
#include <inttypes.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../client.h"

START_TEST (MIN_addr_string_localhost_v4)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  MIN addr string localhost v4\n");
  printf ("  Convert 32-bit integer address for localhost\n");
  printf ("  into dotted-decimal 127.0.0.1 string format.\n\n");

  struct sockaddr_in address;
  memset (&address, 0, sizeof (address));
  address.sin_addr.s_addr = htonl (0x7f000001);

  struct addrinfo server;
  server.ai_family = AF_INET;
  server.ai_addr = (struct sockaddr *) &address;

  char *ipv4 = addr_string (&server);
  printf ("  IPv4 address for localhost is '%s'\n", ipv4);
  printf ("\n");

  ck_assert_str_eq (ipv4, "127.0.0.1");
  free (ipv4);

}
END_TEST

START_TEST (MIN_get_server_list_localhost)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  MIN get server list localhost\n");
  printf ("  Use getaddrinfo() to gather information about localhost\n");
  printf ("  for the HTTP protocol with IPv4. Requires network access\n");
  printf ("  to get information from DNS.\n\n");

  struct addrinfo *server =
    get_server_list ("localhost", "http", true, false);
  struct sockaddr_in *addr4 = (struct sockaddr_in *) server->ai_addr;
  uint32_t address = ntohl (addr4->sin_addr.s_addr);

  printf ("  IPv4 address for localhost is 0x%08" PRIx32 "\n", address);
  printf ("\n");
  ck_assert_int_eq (address, 0x7f000001);

  freeaddrinfo (server);
}
END_TEST

START_TEST (MIN_serv_string_localhost)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  MIN serv string localhost\n");
  printf ("  Converts a struct addrinfo into a string that contains\n");
  printf ("  the transport-layer protocol, IP version, and IP address\n");
  printf ("  in the appropriate format.\n\n");

  struct sockaddr_in address;
  memset (&address, 0, sizeof (address));
  address.sin_addr.s_addr = htonl (0x7f000001);

  struct addrinfo server;
  server.ai_family = AF_INET;
  server.ai_addr = (struct sockaddr *) &address;
  server.ai_socktype = SOCK_STREAM;

  char *serv_info = serv_string (&server);
  printf ("  Server information for localhost: '%s'\n", serv_info);
  printf ("\n");

  ck_assert_str_eq (serv_info, "TCP IPv4 127.0.0.1");
  free (serv_info); 
}
END_TEST

START_TEST (FULL_web_client_youtube_com)
{
  printf ("======================================\n");
  printf ("PUBLIC\n");
  printf ("  FULL web client youtube.com\n");
  printf ("  Creates a socket and sends a GET / HTTP/1.0 request to\n");
  printf ("  youtube.com. Requires network access.\n\n");

  printf ("  Request to youtube.com/\n");
  char *type = NULL;
  char *result = web ("youtube.com", "http", "/", &type, false);
  printf ("  Received result: %s\n", result);
  printf ("  Content type: %s\n", type);
  printf ("\n");

  ck_assert_str_eq (result, "HTTP/1.0 200 OK");
  ck_assert_str_eq (type, "text/html; charset=ISO-8859-1");

  free (type);
  free (result);
}
END_TEST

void public_tests (Suite *s)
{
  TCase *tc_public = tcase_create ("Public");
  tcase_add_test (tc_public, MIN_addr_string_localhost_v4);
  tcase_add_test (tc_public, MIN_get_server_list_localhost);
  tcase_add_test (tc_public, MIN_serv_string_localhost);
  tcase_add_test (tc_public, FULL_web_client_youtube_com);
  suite_add_tcase (s, tc_public);
}

