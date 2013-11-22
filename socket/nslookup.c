
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
	struct hostent *host;
	struct in_addr h_addr;

	if(argc != 2)
	{
		fprintf(stderr, "USAGE: nslookup <inet_address>\n");
		exit(1);
	}

	if((host = gethostbyname(argv[1])) == NULL)
	{
		fprintf(stderr, "nslookup failed on %s \n", argv[1]);
		exit(1);
	}

	h_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
	fprintf(stdout, "%s\n", inet_ntoa(h_addr));
	exit(0);
}
