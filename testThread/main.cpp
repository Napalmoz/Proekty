#include <netdb.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <ntripclient.h>
void TestFunc();
pthread_mutex_t mutex;


static int encode(char *buf, int size, const char *user, const char *pwd)
{
  unsigned char inbuf[3];
  char *out = buf;
  int i, sep = 0, fill = 0, bytes = 0;

  while(*user || *pwd)
  {
    i = 0;
    while(i < 3 && *user) inbuf[i++] = *(user++);
    if(i < 3 && !sep)    {inbuf[i++] = ':'; ++sep; }
    while(i < 3 && *pwd)  inbuf[i++] = *(pwd++);
    while(i < 3)         {inbuf[i++] = 0; ++fill; }
    if(out-buf < size-1)
      *(out++) = encodingTable[(inbuf [0] & 0xFC) >> 2];
    if(out-buf < size-1)
      *(out++) = encodingTable[((inbuf [0] & 0x03) << 4)
               | ((inbuf [1] & 0xF0) >> 4)];
    if(out-buf < size-1)
    {
      if(fill == 2)
        *(out++) = '=';
      else
        *(out++) = encodingTable[((inbuf [1] & 0x0F) << 2)
                 | ((inbuf [2] & 0xC0) >> 6)];
    }
    if(out-buf < size-1)
    {
      if(fill >= 1)
        *(out++) = '=';
      else
        *(out++) = encodingTable[inbuf [2] & 0x3F];
    }
    bytes += 4;
  }
  if(out-buf < size)
    *out = 0;
  return bytes;
}

int RCV(int __fd, void *__buf, size_t __n, int __flags){
    pthread_mutex_lock(&mutex);
    int numbytes=recv(__fd, __buf, __n, __flags);
    pthread_mutex_unlock(&mutex);
    return numbytes;
}

void TestFunc(void)
{
    pthread_mutex_init(&mutex, NULL);

    const char *server = "127.0.0.1";
    const char *port = "5000";
    const char *user = "100";
    const char *password = "100";
    const char *mnt = "TEST0";
    int         mode = AUTO;

    int stop = 0;
    setbuf(stdout, 0);
    setbuf(stdin, 0);
    setbuf(stderr, 0);
    int sleeptime = 0;
    do
    {
        int error = 0;
        sockettype sockfd = 0;
        int numbytes;

        char buf[MAXDATASIZE];
        struct sockaddr_in their_addr; /* connector's address information */
        struct hostent *he;
        struct servent *se;
        //const char *server, *port, *proxyserver = 0;
        const char *proxyserver = 0;
        char proxyport[6];
        char *b;
        long i;
        if(sleeptime)
        {
          sleeptime += 2;
        }
        else
        {
          sleeptime = 1;
        }

        //server = Args.server;
        //port = Args.port;

        if(!stop && !error)
        {
          memset(&their_addr, 0, sizeof(struct sockaddr_in));
          if((i = strtol(port, &b, 10)) && (!b || !*b))
            their_addr.sin_port = htons(i);
          else if(!(se = getservbyname(port, 0)))
          {
            fprintf(stderr, "Can't resolve port %s.", port);
            stop = 1;
          }
          else
          {
            their_addr.sin_port = se->s_port;
          }
          if(!stop && !error)
          {
            if(!(he=gethostbyname(server)))
            {
              fprintf(stderr, "Server name lookup failed for '%s'.\n", server);
              error = 1;
            }
            else if((sockfd = socket(AF_INET, (mode == UDP ? SOCK_DGRAM :
            SOCK_STREAM), 0)) == -1)
            {
              myperror("socket");
              error = 1;
            }
            else
            {
              their_addr.sin_family = AF_INET;
              their_addr.sin_addr = *((struct in_addr *)he->h_addr);
            }
          }
        }
        if(!stop && !error)
        {
            if(connect(sockfd, (struct sockaddr *)&their_addr,
            sizeof(struct sockaddr)) == -1)
            {
              myperror("connect");
              error = 1;
            }
            if(!stop && !error)
            {
              if(!mnt)
              {
                i = snprintf(buf, MAXDATASIZE,
                "GET %s%s%s%s/ HTTP/1.1\r\n"
                "Host: %s\r\n%s"
                "User-Agent: %s/%s\r\n"
                "Connection: close\r\n"
                "\r\n"
                , proxyserver ? "http://" : "", proxyserver ? proxyserver : "",
                proxyserver ? ":" : "", proxyserver ? proxyport : "",
                server, mode == NTRIP1 ? "" : "Ntrip-Version: Ntrip/2.0\r\n",
                AGENTSTRING, revisionstr);
              }
              else
              {
                i=snprintf(buf, MAXDATASIZE-40, /* leave some space for login */
                "GET %s%s%s%s/%s HTTP/1.1\r\n"
                "Host: %s\r\n%s"
                "User-Agent: %s/%s\r\n"
                "Connection: close%s"
                , proxyserver ? "http://" : "", proxyserver ? proxyserver : "",
                proxyserver ? ":" : "", proxyserver ? proxyport : "",
                mnt,server,
                mode == NTRIP1 ? "" : "Ntrip-Version: Ntrip/2.0\r\n",
                AGENTSTRING, revisionstr,
                (*user || *password) ? "\r\nAuthorization: Basic " : "");
                if(i > MAXDATASIZE-40 || i < 0) /* second check for old glibc */
                {
                  fprintf(stderr, "Requested data too long\n");
                  stop = 1;
                }
                else
                {
                  i += encode(buf+i, MAXDATASIZE-i-4, user, password);
                  if(i > MAXDATASIZE-4)
                  {
                    fprintf(stderr, "Username and/or password too long\n");
                    stop = 1;
                  }
                  else
                  {
                    buf[i++] = '\r';
                    buf[i++] = '\n';
                    buf[i++] = '\r';
                    buf[i++] = '\n';
                  }
                }
              }
            }
        }
        if(!stop && !error)
        {
          if(send(sockfd, buf, (size_t)i, 0) != i)
          {
            myperror("send");
            error = 1;
          }
          else if(mnt && *mnt != '%')
          {
            int k = 0;
            int chunkymode = 0;
            int starttime = time(0);
            int lastout = starttime;
            int totalbytes = 0;
            int chunksize = 0;

            while(!stop && !error &&
            (numbytes = RCV(sockfd, buf, MAXDATASIZE-1, 0)) > 0)
            {

              if(!k)
              {
                if( numbytes > 17 &&
                  !strstr(buf, "ICY 200 OK")  &&  /* case 'proxy & ntrip 1.0 caster' */
                  (!strncmp(buf, "HTTP/1.1 200 OK\r\n", 17) ||
                  !strncmp(buf, "HTTP/1.0 200 OK\r\n", 17)) )
                {
                  const char *datacheck = "Content-Type: gnss/data\r\n";
                  const char *chunkycheck = "Transfer-Encoding: chunked\r\n";
                  int l = strlen(datacheck)-1;
                  int j=0;
                  for(i = 0; j != l && i < numbytes-l; ++i)
                  {
                    for(j = 0; j < l && buf[i+j] == datacheck[j]; ++j)
                      ;
                  }
                  if(i == numbytes-l)
                  {
                    fprintf(stderr, "No 'Content-Type: gnss/data' found\n");
                    error = 1;
                  }
                  l = strlen(chunkycheck)-1;
                  j=0;
                  for(i = 0; j != l && i < numbytes-l; ++i)
                  {
                    for(j = 0; j < l && buf[i+j] == chunkycheck[j]; ++j)
                      ;
                  }
                  if(i < numbytes-l)
                    chunkymode = 1;
                }
                else if(!strstr(buf, "ICY 200 OK"))
                {
                  fprintf(stderr, "Could not get the requested data: ");
                  for(k = 0; k < numbytes && buf[k] != '\n' && buf[k] != '\r'; ++k)
                  {
                    fprintf(stderr, "%c", isprint(buf[k]) ? buf[k] : '.');
                  }
                  fprintf(stderr, "\n");
                  error = 1;
                }
                else if(mode != NTRIP1)
                {
                  fprintf(stderr, "NTRIP version 2 HTTP connection failed%s.\n",
                  mode == AUTO ? ", falling back to NTRIP1" : "");
                  if(mode == HTTP)
                    stop = 1;
                }
                ++k;
              }
              else
              {
                sleeptime = 0;
                if(chunkymode)
                {
                  int cstop = 0;
                  int pos = 0;
                  while(!stop && !cstop && !error && pos < numbytes)
                  {
                    switch(chunkymode)
                    {
                    case 1: /* reading number starts */
                      chunksize = 0;
                      ++chunkymode; /* no break */
                    case 2: /* during reading number */
                      i = buf[pos++];
                      if(i >= '0' && i <= '9') chunksize = chunksize*16+i-'0';
                      else if(i >= 'a' && i <= 'f') chunksize = chunksize*16+i-'a'+10;
                      else if(i >= 'A' && i <= 'F') chunksize = chunksize*16+i-'A'+10;
                      else if(i == '\r') ++chunkymode;
                      else if(i == ';') chunkymode = 5;
                      else cstop = 1;
                      break;
                    case 3: /* scanning for return */
                      if(buf[pos++] == '\n') chunkymode = chunksize ? 4 : 1;
                      else cstop = 1;
                      break;
                    case 4: /* output data */
                      i = numbytes-pos;
                      if(i > chunksize) i = chunksize;

                      else
                        fwrite(buf+pos, (size_t)i, 1, stdout);
                      totalbytes += i;
                      chunksize -= i;
                      pos += i;
                      if(!chunksize)
                        chunkymode = 1;
                      break;
                    case 5:
                      if(i == '\r') chunkymode = 3;
                      break;
                    }
                  }
                }
                else
                {
                  totalbytes += numbytes;
                  fwrite(buf, (size_t)numbytes, 1, stdout);
                }
                fflush(stdout);
                if(totalbytes < 0) /* overflow */
                {
                  totalbytes = 0;
                  starttime = time(0);
                  lastout = starttime;
                }
              }
            }
          }
          else
          {
            sleeptime = 0;
            while(!stop && (numbytes=recv(sockfd, buf, MAXDATASIZE-1, 0)) > 0)
            {
              fwrite(buf, (size_t)numbytes, 1, stdout);
            }
          }
        }
      }
    while(mnt && *mnt != '%' && !stop);

    pthread_exit(NULL);

}


int main(void) {
    pthread_t p1;
    int stat;

    stat = pthread_create(&p1, NULL, (void*(*)(void*))&TestFunc, NULL);
    if (stat == 0)
        printf( "Первый поток успешно создан!\n");
    else
        printf( "Неудалось создать второй поток!\n");

    pthread_join(p1, NULL);
    return 0;
}

