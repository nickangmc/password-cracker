/* A simple client program for server.c

   To compile: gcc client.c -o client

   To run: start the server, then the client */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <math.h>

int hex_to_dec(char *hex);
int execute_cmd(char *cmd, char *output);
void get_first_two_byte(char *first_two_byte, char *output);

int main(int argc, char ** argv) {
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent * server;

    char buffer[256];
    char command[1024];

    portno = 7800;

    /* Translate host name into peer's IP address ;
     * This is name translation service by the operating system
     */
    server = gethostbyname("172.26.37.44");

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    /* Building data structures for socket */

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy(server->h_addr_list[0], (char *)&serv_addr.sin_addr.s_addr, server->h_length);

    serv_addr.sin_port = htons(portno);

    /* Create TCP socket -- active open
    * Preliminary steps: Setup: creation of active open socket
    */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(0);
    }

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(0);
    }

    // send username first
    bzero(buffer, 256);
    sprintf(buffer, "minka\n");
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(0);
    }

    // extract the first two bytes(a hex number) of the sha256 hash of this file
    // char *output = malloc(2048 * sizeof(char))
    char output[2048];

    bzero(output, 2048);
    bzero(buffer, 256);
    strcpy(command, "openssl sha256 dh.c");
    n = execute_cmd(command, output);

    if (n < 0) {
        perror("ERROR hashing dh.c w/ sha256");
        exit(0);
    }

    printf("%s", output);

    // char *first_two_hex = malloc(3 * sizeof(char));
    char first_two_hex[3];
    bzero(first_two_hex, 3);
    get_first_two_byte(first_two_hex, output);

    // convert the hex to decimal
    int b;
    b = hex_to_dec(first_two_hex);

    // compute secret key
    int g = 15, p = 97;

    bzero(command, 1024);
    // sprintf(command, "echo '(%d ^ %d) %% %d' | bc", g, b, p);
    sprintf(command, "dc -e '15 %d 97 | p'\n", b);

    bzero(output, 2048);
    n = execute_cmd(command, output);

    if (n < 0) {
        perror("ERROR calculating secret key");
        exit(0);
    }

    printf("g: %d, b: %d, p: %d\n", g, b, p);


    int secret_key = atoi(output);
    printf("Secret key = %d\n", secret_key);

    // write the secret key to the socket
    bzero(buffer, 256);
    sprintf(buffer, "%d\n", secret_key);

    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(0);
    }

    // read server side's secret key
    int a;
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);

    if (n < 0) {
        perror("ERROR reading from server");
        exit(0);
    }

    printf("Secret key received: %s",buffer);

    // calculate final secret key
    a = atoi(buffer);
    bzero(command, 1024);
    // sprintf(command, "echo '(%d ^ (%d * %d)) %% %d' | bc", g, b, a, p);
    sprintf(command, "dc -e '%d %d 97 | p'\n", a, b);

    bzero(output, 2048);
    n = execute_cmd(command, output);

    if (n < 0) {
        perror("ERROR calculating secret key");
        exit(0);
    }

    printf("g: %d, b: %d, p: %d, a: %d\n", g, b, p, a);


    int shared_secret_key = atoi(output);
    printf("Shared secret key = %d\n", shared_secret_key);

    // write the final secret key to the socket
    bzero(buffer, 256);
    sprintf(buffer, "%d\n", shared_secret_key);

    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(0);
    }

    // read the status report
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    
    if (n < 0) {
        perror("ERROR reading from server");
        exit(0);
    }

    printf("Status report: %s\n", buffer);

    return 0;

}

// https://stackoverflow.com/questions/646241/c-run-a-system-command-and-get-output
int execute_cmd(char *cmd, char *output) {

    FILE *fp;
    char buffer[1035];

    /* Open the command for reading. */
    fp = popen(cmd, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return -1;
    }

    /* Read the output a line at a time - output it. */
    while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
        strcat(output, buffer);
    }

    /* close */
    pclose(fp);

    return 0;
}

// https://codeforwin.org/2015/09/c-program-to-convert-hexadecimal-to-decimal-number-system.html
int hex_to_dec(char *hex) {

    long long decimal = 0;
    int i = 0, val = 0, len = 0;

    // Find the length of total number of hex digit
    len = strlen(hex);
    len--;

    // Iterate over each hex digit
    for(i=0; hex[i]!='\0'; i++) {
        /* Find the decimal representation of hex[i] */
        if( hex[i]>='0' && hex[i]<='9') {
            val = hex[i] - 48;
        }
        else if (hex[i]>='a' && hex[i]<='f') {
            val = hex[i] - 97 + 10;
        }
        else if (hex[i]>='A' && hex[i]<='F') {
            val = hex[i] - 65 + 10;
        }

        decimal += val * pow(16, len);
        len--;
    }

    printf("hex: %s\n", hex);
    printf("dec: %lld\n", decimal);

    return decimal;
}

void get_first_two_byte(char *first_two_byte, char *output) {
    // define the prefixes
    char prefixes[] = "SHA256(dh.c)= ";

    strncpy(first_two_byte, strstr(output, prefixes) + strlen(prefixes), 2);
    // end the string
    first_two_byte[3] = 0;
}
