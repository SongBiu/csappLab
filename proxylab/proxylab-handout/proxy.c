#include <stdio.h>
#include <string.h>
#include "csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
void doit(int fd);
void build_http_header(char* method, char *uri, char* version, char* buf);
void parse_uri(char* uri, char* hostname, char* port, char* path);
int main(int argc, char **argv)
{
    // int listenfd, connfd;
    // char hostname[MAXLINE], port[MAXLINE];
    // socklen_t clientlen;
    // struct sockaddr_storage clientaddr;
    // if (argc != 2) {
    //     fprintf(stderr, "usage: %s <port>\n", argv[0]);
    //     exit(1);
    // }
    // listenfd = Open_listenfd(argv[1]);
    // while (1)
    // {
    //     clientlen = sizeof(clientaddr);
	//     connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    //     Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
    //                 port, MAXLINE, 0);
    //     printf("Accepted connection from (%s, %s)\n", hostname, port);
    //     doit(connfd);
    // }
    // printf("%s", user_agent_hdr);
    // return 0;
}
void doit(int fd) 
{
    int clientfd;
    int port;
    struct hostent *host_server;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], path[MAXLIne];
    rio_t rio_server, rio_client;

    Rio_readinitb(&rio_server, fd);
    if (!Rio_readlineb(&rio_server, buf, MAXLINE))
    {
        return;
    } 
        
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    parse_uri(uri, hostname, &port, path);
    // printf("method:%s\nuri:%s\nversion:%s\n", method, uri, version);
    // host_server = Gethostbyname(uri);
    // Connect(clientfd, (struct sockaddr*)host_server->h_addr, sizeof((struct sockaddr*)host_server->h_addr));
    // Rio_writen(clientfd, )
    return;
}
void parse_uri(char *uri, char *hostname, int *port, char *path)
{
    *port = 80;
    int i;
    char *pos1 = strstr(uri, "//"), *pos2=NULL;
    pos1 = pos1?pos1 + 2: uri;
    pos2 = strstr(pos1, ":");
    if (pos2)
    {
        *pos2 = '\n';
        sscanf(pos1, "%s", hostname);
        sscanf(pos2 + 1, "%d%s", port, path);
    }
    else
    {
        pos2 = strstr(pos1, "/");
        *pos2 = '\0';
        sscanf(pos1, "%s", hostname);
        *pos2 = '/';
        sscanf(pos2, "%s", path);
    }
    return ;

}