#ifndef NTRIPCLIENT
#define NTRIPCLIENT

#endif // NTRIPCLIENT

#define BUFFERSIZE 80100
#define DEBUG 1

typedef int sockettype;
#define MAXDATASIZE 1000 /* max number of bytes we can get at once */
#define myperror perror
#define AGENTSTRING "NTRIP NtripClientPOSIX"
static char revisionstr[] = "$Revision: 1.50 $";

//Установка параметров для подключения
enum MODE { HTTP = 1, NTRIP1 = 3, AUTO = 4, UDP = 5, END };

static const char encodingTable [64] = {
  'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
  'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
  'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
  'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
};

class NtripClient
{
public:
    NtripClient(char *server_in, char *port_in, char *user_in, char *password_in, char *mnt_in, int mode_in);
    ~NtripClient();
    void Run();
    void Stop();
    int connect_interval;
    char *server;
    char *port;
    char *user;
    char *password;
    char *mnt;
    int         mode;
    void RunTest();


private:
    static void *StartSendThread(void*);
    void SendThread();
    bool ConnectNtrip();
    void lock();
    void unlock();
    void TestReadBuffer();
    int encode(char *buf, int size, const char *user, const char *pwd);

    unsigned char Buffer[BUFFERSIZE];
    int BytesCount;
    int IndexSend;
    bool IsRunning;
    bool IsConnected;

    //ProgramConfig* Config;

    int sock;

    int port_d;

    pthread_attr_t attr;

    int stop = 0;
};



