#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
void parse_uri(char *uri, char *hostname, char *port, char *path);
void doit(int fd);
void build_http_header(char* method, char *uri, char* version, char* buf);

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Accepted connection from (%s, %s)\n", hostname, port);
        doit(connfd);
    }
    printf("%s", user_agent_hdr);
    return 0;
}

void doit(int fd) 
{
    int clientfd;
    struct hostent *host_server;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], path[MAXLINE], port[MAXLINE];
    rio_t rio_server, rio_client;
    struct in_addr ina;

    Rio_readinitb(&rio_server, fd);
    if (!Rio_readlineb(&rio_server, buf, MAXLINE))
    {
        return;
    } 
        
    sscanf(buf, "%s %s %s", method, uri, version);
    parse_uri(uri, hostname, port, path);
    printf("%s\n%s\n", hostname, port);
    clientfd = Open_clientfd(hostname, port);

    return;
}
void parse_uri(char *uri, char *hostname, char *port, char *path)
{
    int p = 80;
    char *pos1 = strstr(uri, "//"), *pos2=NULL;
    pos1 = pos1?pos1 + 2: uri;
    pos2 = strstr(pos1, ":");
    if (pos2)
    {
        *pos2 = '\0';
        sscanf(pos1, "%s", hostname);
        sscanf(pos2 + 1, "%d%s", &p, path);
    }
    else
    {
        pos2 = strstr(pos1, "/");
        *pos2 = '\0';
        sscanf(pos1, "%s", hostname);
        *pos2 = '/';
        sscanf(pos2, "%s", path);
    }
    sprintf(port, "%d", p);
    return;
}
void build_http_header(char *method, char *uri, char *version, char *header)
{

}