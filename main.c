#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <winuser.h>
#include <wininet.h>
#include <windowsx.h>
#define sleep(x) Sleep(1000 * (x))
#define close closesocket
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <unistd.h>
#endif


// Server config
unsigned short serverPort = 1999;
const char *ServerIP = "192.168.1.78";
int new_return_code = 0;
int initialize_server_socket(unsigned short port, const char *ip_address);

void flush_stdout() {
    fflush(stdout);
    fflush(stderr);
    fflush(stdin);
}

//  Windows GUI application entry point, hides the console window and redirects the standard streams to a file named COCONUT$
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {


    console: // Label for goto statement
    HWND hWnd = GetConsoleWindow();
    printf("[+] Starting server...\n ... Initializing console...\n");
    if (hWnd == NULL) {
        printf("[!] Console window not found\n [!]... Retrying in 3 seconds...\n");
        sleep(3);
        goto console;
    }
    printf("[+] Console window found: %p\n", hWnd);

    // Hide the console window and redirect the standard streams to a file named COCONUT$
    ShowWindow(hWnd, SW_HIDE);
    AllocConsole();
    hWnd = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(hWnd, 0);
    freopen("COCONUT$", "w", stdout);
    freopen("COCONUT$", "w", stderr);
    freopen("CONN$", "r", stdin);

    // Initialize the server socket and retry if it fails to initialize.
    ///... gotta love C's do-while loop and goto statement
int server_socket;
do {
    server_socket = initialize_server_socket(serverPort, ServerIP);
    new_return_code = WSAGetLastError();

    if (server_socket > 0) {
            printf("\n[+] Server initialized on %s:%d (socket: %d)\n", ServerIP, serverPort, server_socket);
    } else {
                printf("\n[-] Server initialization failed on %s:%d (error: %d)\n", ServerIP, serverPort, new_return_code);
                printf("[-] Retrying in 3 seconds...\n");
                sleep(3);
    }
    flush_stdout();
} while (server_socket <= 0);
return 0;
}


/* Initialize the server socket, WSAStartup, socket, bind, and listen.
    The WSAStartup function initiates the use of the Winsock DLL by a process.
    The WSADATA structure contains information about the Windows Sockets implementation.
 */
int initialize_server_socket(unsigned short port, const char *ip_address) {
    printf("[+] Initializing Winsock...\n [!] ", &ip_address, port);
    sleep(3);
    fflush(stdout);

    WSADATA wsaData; // Structure that contains information about the Windows Sockets implementation.
    int wVersionRequested = MAKEWORD(2, 2);
    int wsaError = WSAGetLastError();
    printf("[+] WSAGetLastError: %d\n", wsaError);
    printf("[+] WSAStartup: %d\n", wsaError);
    printf("[+] MAKEWORD: %d\n", wVersionRequested);
    wsaError = WSAStartup(wVersionRequested, &wsaData);

    if (wsaError != 0) {
        printf("\n [-] Please make sure you have WinSock DLL. \n [-] Error code: %d\n", WSAGetLastError());
        printf("[+] Error code: %d\n", wsaError);
        exit(EXIT_FAILURE);
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("[+] WSAStartup initialized\n");

    printf("[+] Error code: %d\n", wsaData.lpVendorInfo);
    printf("[+] Major version: %d\n", wsaData.wVersion);
    printf("[+] Minor version: %d\n", wsaData.wHighVersion);
    printf("[+] Description: %s\n", wsaData.szDescription);
    printf("[+] System status: %s\n", wsaData.szSystemStatus);
    printf("[+] Max sockets: %d\n", wsaData.iMaxSockets);
    printf("[+] Max datagram size: %d\n", wsaData.iMaxUdpDg);
    printf("[+] Vendor info: %s\n", wsaData.lpVendorInfo);
    printf("--------------------------------------------------------------------------------\n");
    fflush(stdout);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("[-] Socket creation failed");
        return EXIT_FAILURE;
    }

    printf("[+] Socket created: %d\n", server_socket);
    fflush(stdout);

    printf("\n [!] Structuring Server Service \n");
    sleep(2);

    /// Structuring the server service, clear the struct to 0, then set the family, port, and IP address
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip_address);

    printf("\n [+] sin_family: %d, sin_port: %d, sin_addr.s_addr: %lu\n",
           server_addr.sin_family,
           ntohs(server_addr.sin_port),
           (unsigned long)server_addr.sin_addr.s_addr);

    printf("\n [+] Binding Socket \n");
    sleep(2);
    fflush(stdout);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Bind failed");
        close(server_socket);
        return -1;
    }
    printf("[+] Bind done\n");
    fflush(stdout);

    if (listen(server_socket, 5) < 0) {
        perror("[-] Listen failed");
        close(server_socket);
        return -1;
    }
    printf("[+] Server listening on %s:%d\n", ip_address, port);
    fflush(stdout);

    return server_socket;
}
