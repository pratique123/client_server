
//  udpclientserver.c
//  Created by Prattique Timalsena on 04/11/15.
//  Copyright © 2015 Prattique Timalsena. All rights reserved.
//
//
//  main.m
//  udpclientserver.c
//
//  Created by Prattique Timalsena on 03/08/15.
//  Copyright (c) 2015 Prattique Timalsena. All rights reserved.
//




#include "client.h"

/*error displaying function*/
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

/*write packet id and timestamp(rtt) to the file */
void write_packet_info(struct packet_info *sender)
{
    // open file in appendmode checking either file exists or open new one to write
    FILE *file1 = fopen("rtt.txt", "a+");
    if(file1 == NULL)
    {
        printf("\nCan't open file or file doesn't exist.");
        exit(0);
    }
    
    fprintf(file1,"%zu\t%lld\t%lld\t", sender->id,sender->send_time,sender->rtt);
    fprintf(file1,"\n");
    fflush(stdin);
    fflush(stdout);
    fclose(file1);
    // write the structure  to the file;

}
/* prepare packet info to the structure */
void create_packet_info(size_t id,long long microseconds1,long long rtt)
{   struct packet_info *sender = malloc(sizeof( struct packet_info));
    sender->id = id;
    sender->send_time = microseconds1;
    sender->rtt = rtt;
    write_packet_info(sender);
    free (sender);

}

/*sort by milisecond if needed currently not used */
int milisecond_sort(struct send_time *a, struct send_time *b) {
    return (a->milisecond - b->milisecond);
}

/* sort by id */
int id_sort(struct send_time *a, struct send_time *b) {
    return (a->id - b->id);
}

void sort_by_milisecond() {
    HASH_SORT(senders, milisecond_sort);
}

void sort_by_id() {
    HASH_SORT(senders, id_sort);

}
void add_sender(size_t id, long long milisecond) {
    struct send_time *s;


    HASH_FIND_INT(senders, &id, s);  /* id already in the hash? */
    if (s==NULL) {
        s = (struct send_time*)malloc(sizeof(struct send_time));
        s->id = id;
        HASH_ADD_INT( senders, id, s );  /* id: milisecond of key field */
    }
    s->milisecond = milisecond;
    //printf("data added successfuly:\n");
}

void add_rountrip_time(size_t id, long tr)
{
    struct roundtrip_time *Rtt;
    HASH_FIND_INT(rond, &id, Rtt);  /* id already in the hash? */
    if (Rtt==NULL) {
        Rtt = (struct roundtrip_time*)malloc(sizeof(struct roundtrip_time));
        Rtt->id = id;
        HASH_ADD_INT( rond, id, Rtt );  /* id: milisecond of key field */
    }
    Rtt->rtt = tr;
    //printf(" round trip time data added successfuly:\n");

}

struct send_time *find_sender(size_t id) {
    struct send_time *s = malloc(sizeof(struct send_time));
    printf(" The value of id == %zu",id);
    HASH_FIND_INT( senders, &id, s );  /* s: output pointer */
    printf(" hello you are here to return the value------->");
    return s;
}
void add_receiver(size_t id, long long rt)
{
    struct receive_time *r;
    HASH_FIND_INT(receivers, &id, r);  /* id already in the hash? */
    if (r==NULL) {
        r= (struct receive_time*)malloc(sizeof(struct receive_time));
        r->id = id;
        HASH_ADD_INT( receivers, id, r );  /* id: milisecond of key field */
    }
    r->rt = rt;
    // printf("rtt data added successfuly:\n");

}

void delete_sender(struct send_time *sender) {
    HASH_DEL( senders, sender);  /* sender: pointer to deletee */
    free(sender);
}

void delete_all() {
    struct send_time *current_sender, *tmp;

    HASH_ITER(hh, senders, current_sender, tmp) {
        HASH_DEL(senders,current_sender);  /* delete it (senders advances to next) */
        free(current_sender);            /* free it */
    }
}

long long  print_senders(size_t sequence) {
    struct send_time *s;
    long long milisecond;
    for(s=senders; s != NULL; s=(struct send_time*)(s->hh.next)) {
         if (s->id == sequence)
           milisecond = s->milisecond;
    }
return(milisecond);
}
void print_roundtrip_time(size_t sequence) {
    struct roundtrip_time *Rtt;

    for(Rtt=rond; Rtt != NULL; Rtt=(struct roundtrip_time*)(Rtt->hh.next)) {
        if (Rtt->id == sequence) {
            //printf("\nRtt id =%zu\t",Rtt->id);
            send_time = print_senders(sequence);
            create_packet_info(sequence,send_time,Rtt->rtt);
    }
  }
}

void print_receive_time()
{

    struct receive_time *r;
    for(r=receivers; r != NULL; r=(struct receive_time*)(r->hh.next))
    {
        printf("received id %zu: timemicrosec %lld\n", r->id, r->rt);

    }
}

void calculate_rtt()
{
    struct receive_time *r;
    struct send_time *s;
    for(s=senders; s != NULL; s=(struct send_time*)(s->hh.next))        {
        for (r=receivers; r !=NULL; r = (struct receive_time*)(r->hh.next))

        {

            if (s->id == r->id) {
                long tr = r->rt - s->milisecond;
                add_rountrip_time(s->id,tr);
            }
        }
    }
}


/** populates the structure with desired data to make a packet*/
void fillup_buffer(struct packet *data, char *buffer){
    bzero(buffer,1024);
    memcpy(buffer,data,sizeof(struct packet));
}

/* copies buffer to a structure*/
void copy_structure(char *buf, struct packet *copy_data){
    memcpy(copy_data, buf,sizeof(struct packet));
}
/* generates packets with seq , size and put in the structure*/
void  fillup_structure(struct packet *data, size_t size, size_t count)
{
    //count = count +1;
    bzero(data -> payload, size);
    data ->seq = count;
    data ->size = size;
    memset(data->payload ,'0',size);
    // printf(" the value of payload =%s",data->payload);
    //printf("\n");
}
/*sets waiting time for select fuction*/
void handle_timeval()
{
    tv.tv_sec = 5;
    tv.tv_usec = 500000;

}

int main(int argc, char *argv[])
{

    unsigned int length;
    struct sockaddr_in server, from;
    struct hostent *hp;
    char buffer[1024];
    char buf[1024];

   // struct packet *data = malloc(sizeof(struct packet));

    if (argc != 4)
    {
        printf("Usage: server port\n");
        exit(1);
    }
    sock= socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) error("socket");
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp==0) error("Unknown host");

    bcopy((char *)hp->h_addr,
          (char *)&(server.sin_addr),
          hp->h_length);
    server.sin_port = htons(atoi(argv[2]));
    // int Npack = atoi(argv[4]);
    size_t size = atoi(argv[3]);
    length=sizeof(struct sockaddr_in);
    static size_t count = 0;
    /* This loop is for sending desired number of the packet increasing the count as a sequence */
    while(1)
    {
        count++;
        struct packet *data = malloc(sizeof(struct packet));
        memset(data,0,sizeof(struct packet));
        bzero(buffer,1024);
        bzero(buf, 1024);
        /*call the function to populate the structure element*/
        fillup_structure(data,size,count);

        //buffer_serialization(&buffer,data);

        /** copy the structure to the buffer which will be sent to server **/
        fillup_buffer(data,&buffer);
       free(data);
        struct packet *data1 = malloc(sizeof(struct packet));
	memset(data1,0,sizeof(struct packet));
        gettimeofday(&Tb, NULL); // get current time
        microseconds1 =  (Tb.tv_sec*1000000LL)+(Tb.tv_usec);
        copy_structure(&buffer, data1);
        /* send the packet to the server */
        n=sendto(sock,buffer, 1024,0,(const struct sockaddr *)&server,length);
        /*check if the send operation ok or not */
        if (n < 0)
        {
          error("Sendto");

        }

         add_sender(data1->seq,microseconds1);//add the sending information to hash
        bzero(message,1024);
        FD_ZERO(&rfds);
         FD_SET(sock,&rfds);
        handle_timeval();//set timeout for select function
        /*set select function for receiving socket*/
        select_receive = select(sock+1,&rfds,NULL,NULL,&tv);//select for checking if the socket is readable

        if (select_receive<0)
        {

            error("select");

        }

        else if(select_receive == 0)

        {
            count1++;
            if(count1>15 && count1==count)

               // if ((count%50)==0)

                    printf("\nTimeout receive: Packet is not received  back yet trying to get packet");

        }

        else
        {
            /*check if the data available to read*/
           // n=FD_ISSET(sock, &rfds);

        if (FD_ISSET(sock, &rfds))

        {
                n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from, &length);// receiving packet from server
                gettimeofday(&Te, NULL); // get current time
                microseconds2 = (Te.tv_sec*1000000LL)+ (Te.tv_usec);
                Ts = microseconds2 - microseconds1;
             if (n < 0)
                {
                   // strcpy(message,  "Receiving back to the message failed:");
                    //create_log_structure_receive(logreceiver,NULL,microseconds2, &message);
                    //log_sender_receiving(logreceiver);
                    error("recvfrom");

                }

                struct packet *data2 = malloc(sizeof(struct packet));

                /* copy the buffer sent by server to the structure*/
                copy_structure(&buf,data2);
                /* add receiving time stamp and id to hash */
                add_receiver(data2->seq, microseconds2);

                /* calculate the rtt of the packet after matching id on the both hashes*/
                calculate_rtt();
            // calculate the rtt on the basis of the sent and received packet id
                print_roundtrip_time(data1->seq);// print the rtt of the package after matching the packet id
                bzero(buffer,1024);
                bzero(buf, 1024);
               fflush(stdin);
               fflush(stdout);
                // printf(" seq =%zu \t size =%zu ", data2 -> seq,data2 -> size);
                //printf("\tThe round trip time = %ld\n",Ts);
                free (data2);// free structure used to receive
                sleep(2);//sleep for 2s for easy sending and receiving

            }

           free(data1);//free the structure used to send

        }
    }
    close(sock);
   // free(data);
    return 0;
}


