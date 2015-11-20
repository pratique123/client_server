//
//  client.h
//  udpclientserver.c
//
//  Created by Prattique Timalsena on 11/09/15.
//  Copyright (c) 2015 Prattique Timalsena. All rights reserved.
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "uthash.h"
int sock, n;
static int count1 =0;
size_t packet_id;
struct timeval Te , Tb;
fd_set rfds;
fd_set wfds;
struct timeval tv;
long long microseconds1,microseconds2;
long Ts;
char message[1000];
int select_send;
int select_receive;
void add_sender(size_t id, long long milisecond);
void add_receiver(size_t,long long);
struct send_time *find_sender(size_t id);
void delete_sender(struct send_time *sender);
void delete_all();
long long  print_senders();
int milisecond_sort(struct send_time *a, struct send_time *b);
int id_sort(struct send_time *a, struct send_time *b);
void sort_by_milisecond();
void sort_by_id();
void print_roundtrip_time(size_t);
void calculate_rtt();
void print_receive_time();
void error(const char *);
long long send_time = 0;
struct send_time {
    size_t id;                    /* key */
    long long milisecond;
    UT_hash_handle hh;         /* makes this structure hashable */
};
struct receive_time{
    size_t id;
    long long rt;
    UT_hash_handle hh;
};
struct roundtrip_time
{
    size_t id;
    long rtt;
    UT_hash_handle hh;
};
struct send_time *senders = NULL;
struct receive_time *receivers = NULL;
struct roundtrip_time *rond = NULL;

struct packet_info
{
    size_t id;
    long long send_time;
    long long rtt;

};
struct packet_info sender;

struct packet
{
    size_t seq;
    size_t size;
    char  payload[1024];
};
void write_packet_info(struct packet_info *sender);
void create_packet_info(size_t id,long long microseconds1,long long rtt);

