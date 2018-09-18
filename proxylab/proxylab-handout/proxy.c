#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csapp.h"
/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
typedef struct cacheInfo
{
    char *hostname, *port;
    char *path;
} info;
void parse_uri(char *uri, char *hostname, char *port, char *path);
void doit(int fd);
void build_http_header(char *method, char *path, char *version, char *header);
void handle(void *arg);
int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t thread_id;
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    listenfd = Open_listenfd(argv[1]);
    while (1)
    {
        clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Pthread_create(&thread_id, NULL, (void *)(&handle), (void *)connfd);
    }
    return 0;
}
void handle(void *arg)
{
    int fd = (int)arg;
    doit(fd);
    Close(fd);
}
void doit(int fd) 
{
    int clientfd;
    size_t n;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], path[MAXLINE], port[MAXLINE], header[MAXLINE];
    //char response[MAX_CACHE_SIZE];
    rio_t rio_server, rio_client;

    Rio_readinitb(&rio_server, fd);
    if (!Rio_readlineb(&rio_server, buf, MAXLINE))
    {
        return;
    }
    // parser http request
    sscanf(buf, "%s %s %s", method, uri, version);
    parse_uri(uri, hostname, port, path);
    build_http_header(method, path, version, header);
    clientfd = Open_clientfd(hostname, port);
    Rio_readinitb(&rio_client, clientfd);
    printf("%s\r\n", header);
    Rio_writen(clientfd, (void *)header, sizeof(header));

    while ((n = Rio_readlineb(&rio_client, buf, MAXLINE)) != 0)
    {
        Rio_writen(fd, buf, n);
    }
    
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
void build_http_header(char *method, char *path, char *version, char *header)
{
    char *connect = "Host: www.cmu.edu\r\nAccept: */*\r\nConnection: close\r\nProxy-Connection: close";
    sprintf(header, "%s %s %s\r\n%s%s\r\n\r\n", method, path, version, user_agent_hdr, connect);
}
