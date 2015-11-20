//
//  main.m
//  udpserver
//
//  Created by Prattique Timalsena on 09/09/15.
//  Copyright (c) 2015 Prattique Timalsena. All rights reserved.
//
#include "server.h"
int n1;

/* Creates a datagram server.  The port
 number is passed as an argument.  This
 server runs forever */
 static int count1 = 0;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void handle_timeval()
{
    tv.tv_sec = 12;
    tv.tv_usec = 500000;

}

void create_received_info(size_t id,long long microseconds1)
{   struct received_info *receiver = malloc(sizeof(struct received_info));
    receiver->id = id;
    receiver->receive_time = microseconds1;
    write_received_info(receiver);
    free(receiver);
}


void write_received_info(struct received_info  *receiver)
{
    // open file in appendmode checking either file exists or open new one to write
    FILE *file1 = fopen("receiving_info.txt", "a+");
    if(file1 == NULL)
    {
        printf("\nputi kha Can't open file or file doesn't exist.");
        exit(0);
    }
    //fwrite(receiver, sizeof(struct log_sender_receiving), 1, file);

    fprintf(file1,"%zu\t%lld", receiver->id,receiver->receive_time);
    fprintf(file1,"\n");
    fflush(stdin);
    fflush(stdout);
    fclose(file1);
    // write the structure  to the file

 }

void copy_structure(char *buf, struct packet *copy_data){
    memcpy(copy_data, buf,sizeof(struct packet));
}

int main(int argc, char *argv[])
{
        //sleep(3);
    int sock, length, n;
    socklen_t fromlen;
    struct sockaddr_in server;
    struct sockaddr_in from;
    char buf[1024];
    struct packet *data1 = malloc(sizeof(struct packet));
    if (argc < 2) {
        fprintf(stderr, "ERROR, port  or number of iteration not provided \n");
        exit(0);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");
    length = sizeof(server);
    bzero(&server,length);
    server.sin_family=AF_INET;
    server.sin_addr.s_addr=INADDR_ANY;
    server.sin_port=htons(atoi(argv[1]));
    //Npack = atoi(argv[2]);
    if (bind(sock,(struct sockaddr *)&server,length)<0)
        error("binding");
    fromlen = sizeof(struct sockaddr_in);
    static size_t count = 0;
    while (1) {
        count ++;
        bzero(buf,1024);
        FD_ZERO(&rfds);
        FD_SET(sock, &rfds);
        handle_timeval();
        select_receive = select(sock+1, &rfds,NULL,NULL,&tv);
        //printf("the value of send_receive =%d \n",select_receive);
        if (select_receive <0)
        {
            error("select\n");
        }
        else if(select_receive == 0)
        {   count1++;
            //printf("Time out receive\n");
            if((count1 >50)&&(count==count1)) printf("Still waiting for packets from client\n");
        }
        else
        {

        if (FD_ISSET(sock, &rfds))
        {
            n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
            gettimeofday(&Te, NULL); // get current time
            microseconds1 =  Te.tv_sec*1000000LL+Te.tv_usec;
            copy_structure(&buf, data1);
             n1 = sendto(sock,buf,1024,0,(struct sockaddr *)&from,fromlen);
             gettimeofday(&Te, NULL); // get current time
           // microseconds2=  Te.tv_sec*1000000LL+Te.tv_usec;
            if (n < 0)
            {
                error("recvfrom");

            }
            create_received_info(data1->seq,microseconds1);

 if (n1  < 0)
            {
                error("sendto");
            }

            sleep(2);
        }
     }

    }
    free(data1);

    return 0;
}
