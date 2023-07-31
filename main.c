#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_QUEUE_LENGTH 5
#define PORT 1024
#define BUFF_SIZE 1000
#define MAX_RESP_SIZE 1000

int main() {
    char buff[BUFF_SIZE];
    int socket_d;
    struct sockaddr client_sock_addr;
    socklen_t slen = sizeof(client_sock_addr);
    char size_buff[10];

    //Constructing response
    char body[] = 
"<!DOCTYPE html>\
<html>\
<head>\
<title>Page Title</title>\
</head>\
<body>\
\
<h1>This is a Heading</h1>\
<p>This is a paragraph.</p>\
\
</body>\
</html>";
    int size = strlen(body);
    sprintf(size_buff, "%d", size + 1);
    char response[MAX_RESP_SIZE] = 
"HTTP/1.1 200 OK\r\n\
Date: Mon, 27 Jul 2049 12:28:53 GMT\r\n\
Content-Length: ";
    strcat(response, size_buff);
    strcat(response, "\r\nContent-Type: text/html\r\n\r\n");
    strcat(response, body);
    //-----------------------

    if ((socket_d = socket(AF_INET6, SOCK_STREAM, 0)) == -1) {
        perror("socket creation error");
        goto NO_CLEANUP;
    }
    printf("socket created\n");

    struct sockaddr_in6 addr = {.sin6_family = AF_INET6, .sin6_port = htons(PORT)};
    addr.sin6_addr = in6addr_any;
    if (bind(socket_d, (const struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind error");
        goto SOCKET_CLEANUP;
    }
    printf("bind succesfull\n");

    if (listen(socket_d, MAX_QUEUE_LENGTH) == -1) {
        perror("socket listen error");
        goto SOCKET_CLEANUP;
    }

    int client_socket_d;
    while (client_socket_d = accept(socket_d, &client_sock_addr, &slen)) {
        if (client_socket_d != -1) {
            printf("accepted, client d: %d\n", client_socket_d);

            if (read(client_socket_d, buff, sizeof(buff)) != -1) {
                printf("%s\n", buff);

                if (write(client_socket_d, response, MAX_RESP_SIZE) == -1) {
                    perror("writing response to buff error");
                }
            }
            close(client_socket_d);
        }
    }
SOCKET_CLEANUP:
    if (close(socket_d) == -1) {
        perror("close error");
    }

NO_CLEANUP:
    return 0;
}





