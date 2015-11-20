//
//  server.h
//  udpserver
//
//  Created by Prattique Timalsena on 11/09/15.
//  Copyright (c) 2015 Prattique Timalsena. All rights reserved.
//
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
fd_set rfds;
fd_set wfds;
char message[1000];
struct timeval tv;
struct timeval Te;
long long microseconds1,microseconds2;
int sock;
int Npack;
int select_send;
int select_receive;
struct log_server_sending
{
    size_t id;
    long long send_time;
    char msg[1024];


};

struct received_info
{
    size_t id;
    long long receive_time;
};
struct packet
{
    size_t seq;
    size_t size;
    char  payload[1024];
};
void create_received_info(size_t id,long long microseconds1);
void write_received_info(struct received_info  *receiver);