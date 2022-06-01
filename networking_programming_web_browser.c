#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/stat.h>

#define MAXBUF          1024    // 1kb단위로 file read

int main(int argc, char* argv[]) {
        int sockd;
        int counter;

        struct sockaddr_in webServer;
        char buf[MAXBUF];
        char msg[256] = "GET / HTTP/1.1\r\nHost: ";	// index.html을 받아오기 위한 명령어
        char msg_tail[] = "\r\n\r\n";			// input으로 받은 IP 주소 뒤 enter 명령어
        int returnStatus;

        if(argc < 3) {
                fprintf(stderr, "Usage: %s <ip address> <port> \n", argv[0]);	// 실행 파일, IP 주소, port 번호 
                exit(1);
        }

        sockd = socket(AF_INET, SOCK_STREAM, 0);	// TCP 방식이므로 SOCK_STREAM 사용, 0 사용 시 자동으로 TCP
        if(sockd == -1) {
                fprintf(stderr, "Could not create socket!\n");
                exit(1);
        } else {
                fprintf(stderr, "socket created!\n");
        }

        webServer.sin_family = AF_INET;
        webServer.sin_addr.s_addr = inet_addr(argv[1]);	
        webServer.sin_port = htons(atoi(argv[2]));		// ascii로 받은 값 integer로 변환(atoi), byte order 맞춤(htons) 

        returnStatus = connect(sockd, (struct sockaddr*)&webServer, sizeof(webServer));
        if(returnStatus == -1) {
                fprintf(stderr, "Could not connect to server!\n");
                exit(1);
        } else {
                fprintf(stderr, "Connect successful!\n");
        }

        strcat(msg, argv[1]);		// 명령어에 input IP 주소 추가
        strcat(msg, msg_tail);		// enter 추가
        send(sockd, msg, strlen(msg), 0);
        while((counter = read(sockd, buf, MAXBUF) > 0)) {
                printf("%s\n", buf);	// index.html 출력
                }

        close(sockd);
        return 0;
}

