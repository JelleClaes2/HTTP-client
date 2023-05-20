#ifdef _WIN32
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#include <winsock2.h> //for all socket programming
#include <ws2tcpip.h> //for getaddrinfo, inet_pton, inet_ntop
#include <stdio.h> //for fprintf, perror
#include <unistd.h> //for close
#include <stdlib.h> //for exit
#include <string.h> //for memset
void OSInit( void )
{
    WSADATA wsaData;
    int WSAError = WSAStartup( MAKEWORD( 2, 0 ), &wsaData );
    if( WSAError != 0 )
    {
        fprintf( stderr, "WSAStartup errno = %d\n", WSAError );
        exit( -1 );
    }
}
void OSCleanup( void )
{
    WSACleanup();
}
#define perror(string) fprintf( stderr, string ": WSA errno = %d\n", WSAGetLastError() )
#else
#include <sys/socket.h> //for sockaddr, socket, socket
#include <sys/types.h> //for size_t
#include <netdb.h> //for getaddrinfo
#include <netinet/in.h> //for sockaddr_in
#include <arpa/inet.h> //for htons, htonl, inet_pton, inet_ntop
#include <errno.h> //for errno
#include <stdio.h> //for fprintf, perror
#include <unistd.h> //for close
#include <stdlib.h> //for exit
#include <string.h> //for memset
void OSInit( void ) {}
void OSCleanup( void ) {}
#endif

int initialization(int flag);
int connection( int internet_socket,const char * client_address_string, int size );
void IPgetRequest(const char * client_address_string,FILE *filePointer);
void execution( int internet_socket );
void cleanup(int client_internet_socket);

int main( int argc, char * argv[] )
{

    FILE *filePointer =fopen( "log.log", "w" );

    OSInit();

    int internet_socket = initialization(0);//BOTNETS

    char client_address_string[INET_ADDRSTRLEN];

    int client_internet_socket = connection( internet_socket, client_address_string,sizeof(client_address_string) );

    setsockopt(client_internet_socket,SOL_SOCKET,SO_KEEPALIVE,1, sizeof (int) );

    int number_of_bytes_received = 0;
    char buffer[1000];
    number_of_bytes_received = recv( internet_socket, buffer, ( sizeof buffer ) - 1, 0 );
    if( number_of_bytes_received == -1 )
    {
        perror( "recv" );
    }
    else
    {
        buffer[number_of_bytes_received] = '\0';
        printf( "Received : %s\n", buffer );
    }

    IPgetRequest(client_address_string,filePointer);

    execution( client_internet_socket );

    cleanup(client_internet_socket);

    fclose(filePointer);

    cleanup(internet_socket);

    OSCleanup();

    return 0;
}

int initialization(int flag)
{
    //Step 1.1
    struct addrinfo internet_address_setup;
    struct addrinfo * internet_address_result;
    memset( &internet_address_setup, 0, sizeof internet_address_setup );
    internet_address_setup.ai_family = AF_INET;
    internet_address_setup.ai_socktype = SOCK_STREAM;
    internet_address_setup.ai_flags = AI_PASSIVE;
    if(flag == 0){
        int getaddrinfo_return = getaddrinfo( NULL, "22", &internet_address_setup, &internet_address_result );
        if( getaddrinfo_return != 0 )
        {
            fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( getaddrinfo_return ) );
            exit( 1 );
        }

        int internet_socket = -1;
        struct addrinfo * internet_address_result_iterator = internet_address_result;
        while( internet_address_result_iterator != NULL )
        {
            //Step 1.2
            internet_socket = socket( internet_address_result_iterator->ai_family, internet_address_result_iterator->ai_socktype, internet_address_result_iterator->ai_protocol );
            if( internet_socket == -1 )
            {
                perror( "socket" );
            }
            else
            {
                //Step 1.3
                int bind_return = bind( internet_socket, internet_address_result_iterator->ai_addr, internet_address_result_iterator->ai_addrlen );
                if( bind_return == -1 )
                {
                    perror( "bind" );
                    close( internet_socket );
                }
                else
                {
                    //Step 1.4
                    int listen_return = listen( internet_socket, 1 );
                    if( listen_return == -1 )
                    {
                        close( internet_socket );
                        perror( "listen" );
                    }
                    else
                    {
                        break;
                    }
                }
            }
            internet_address_result_iterator = internet_address_result_iterator->ai_next;
        }

        freeaddrinfo( internet_address_result );

        if( internet_socket == -1 )
        {
            fprintf( stderr, "socket: no valid socket address found\n" );
            exit( 2 );
        }

        return internet_socket;
    }else if (flag == 1){

        int getaddrinfo_return = getaddrinfo( "ip-api.com", "80", &internet_address_setup, &internet_address_result );
        if( getaddrinfo_return != 0 )
        {
            fprintf( stderr, "getaddrinfo: %s\n", gai_strerror( getaddrinfo_return ) );
            exit( 1 );
        }

        int internet_socket = -1;
        struct addrinfo * internet_address_result_iterator = internet_address_result;
        while( internet_address_result_iterator != NULL )
        {
            //Step 1.2
            internet_socket = socket( internet_address_result_iterator->ai_family, internet_address_result_iterator->ai_socktype, internet_address_result_iterator->ai_protocol );
            if( internet_socket == -1 )
            {
                perror( "socket" );
            }
            else
            {
                //Step 1.3
                int connect_return = connect( internet_socket, internet_address_result_iterator->ai_addr, internet_address_result_iterator->ai_addrlen );
                if( connect_return == -1 )
                {
                    perror( "connect" );
                    close( internet_socket );
                }
                else
                {
                    break;
                }
            }
            internet_address_result_iterator = internet_address_result_iterator->ai_next;
        }

        freeaddrinfo( internet_address_result );

        if( internet_socket == -1 )
        {
            fprintf( stderr, "socket: no valid socket address found\n" );
            exit( 2 );
        }

        return internet_socket;
    }

}

int connection( int internet_socket, const char * client_address_string, int size )
{
    //Step 2.1
    struct sockaddr_in client_internet_address;
    socklen_t client_internet_address_length = sizeof(client_internet_address);
    int client_socket = accept( internet_socket, (struct sockaddr *) &client_internet_address, &client_internet_address_length );
    if( client_socket == -1 )
    {
        perror( "accept" );
        close( internet_socket );
        exit( 3 );
    }

    inet_ntop(AF_INET, &(client_internet_address.sin_addr), client_address_string, size);

    printf("Client IP address: %s\n", client_address_string);

    return client_socket;
}

void IPgetRequest(const char * client_address_string,FILE *filePointer){

    int internet_socket_HTTP = initialization(1);

    fputs("The IP address =",filePointer);
    fputs(client_address_string,filePointer);
    fputs("\n",filePointer);

    char buffer[1000];
    char HTTPrequest[100] ={0};

    sprintf(HTTPrequest,"GET /json/%s HTTP/1.0\r\nHost: ip-api.com\r\nConnection: close\r\n\r\n", client_address_string);
    printf("HTTP request = %s",HTTPrequest);

    //SEND HTTP REQUEST TO GET LOCATION OF BOTNET
    int number_of_bytes_send = 0;
    number_of_bytes_send = send( internet_socket_HTTP, HTTPrequest , strlen(HTTPrequest), 0 );
    if( number_of_bytes_send == -1 )
    {
        perror( "send" );
    }

    int number_of_bytes_received = 0;
    number_of_bytes_received = recv( internet_socket_HTTP, buffer, ( sizeof buffer ) - 1, 0 );
    if( number_of_bytes_received == -1 )
    {
        perror( "recv" );
    }
    else
    {
        buffer[number_of_bytes_received] = '\0';
        printf( "Received : %s\n", buffer );
    }

    char* jsonFile = strchr(buffer,'{');

    if( jsonFile == NULL){
        number_of_bytes_received = recv( internet_socket_HTTP, buffer, ( sizeof buffer ) - 1, 0 );
        if( number_of_bytes_received == -1 )
        {
            perror( "recv" );
        }
        else
        {
            buffer[number_of_bytes_received] = '\0';
            printf( "Received : %s\n", buffer );
        }


        fputs( buffer , filePointer );
    } else{


        fputs(jsonFile, filePointer);
    }

    cleanup(internet_socket_HTTP);
}

void execution( int internet_socket )
{
    //Step 3.1


    //Step 3.2
    int number_of_bytes_send = 0;
    number_of_bytes_send = send( internet_socket, "Hello TCP world!", 16, 0 );
    if( number_of_bytes_send == -1 )
    {
        perror( "send" );
    }
}

void cleanup(int client_internet_socket)
{
    //Step 4.2
    int shutdown_return = shutdown( client_internet_socket, 2 );
    if( shutdown_return == -1 )
    {
        perror( "shutdown" );
    }

    //Step 4.1
    close( client_internet_socket );
} /* Can u rebuild this program to an IPV4 */


