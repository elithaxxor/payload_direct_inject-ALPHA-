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

unsigned short serverPort = 1999;
const char *ServerIP = "192.168.1.78";
int new_return_code = 0;

int initialize_server_socket(unsigned short port, const char *ip_address);

void flush_stdout() {
    fflush(stdout);
    fflush(stderr);
    fflush(stdin);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HWND hWnd = GetConsoleWindow();

    if (hWnd == NULL) {
        printf("[!n] Console window not found\n");
    } else {
        ShowWindow(hWnd, SW_HIDE);
    }
    AllocConsole();
    hWnd = FindWindowA("ConsoleWindowClass", NULL);
    ShowWindow(hWnd, 0);

    freopen("COCONUT$", "w", stdout);
    freopen("COCONUT$", "w", stderr);
    freopen("CONN$", "r", stdin);

    int server_socket = initialize_server_socket(serverPort, ServerIP);
    printf("[!] Initial socket details,\n [+] IP: %s\n [+] Port: %hu\n [+]Server Socket: %d\n", ServerIP, serverPort, server_socket, WSAGetLastError());
    sleep(3);
    flush_stdout();

    if (server_socket == 0 || new_return_code == 0) {
        printf("\n[+] Server initialized\n [+] IP: %s\n [+] Port: %d\n", ServerIP, serverPort);
        printf("\n [+] Server Socket: %d (last error code: %d)\n", server_socket, WSAGetLastError());
        printf("\n [+] Server is now ready to accept connections\n");
        flush_stdout();
        server_socket = initialize_server_socket(serverPort, ServerIP);
        sleep(3);
        flush_stdout();
    } else {
        printf("\n[-] Server initialization failed\n [-] IP: %s\n [-] Port: %d\n", ServerIP, serverPort);
        printf("[-] Error code: %d\n", WSAGetLastError());
        printf("[-] retrying...");
        sleep(3);
        flush_stdout();
    }

    while (server_socket != 0 || new_return_code != 0) {
        int return_code = WSAGetLastError();
        printf("\n [-]Server initialization failed\n [-] IP: %s\n [-] Port: %d\n", ServerIP, serverPort);
        printf("\n [-] Error code: %d\n retrying...", return_code);
        server_socket = initialize_server_socket(serverPort, ServerIP);
        new_return_code = WSAGetLastError();
        struct sockaddr_in server_addr = {0};
        sleep(3);
        flush_stdout();
    }
}

int initialize_server_socket(unsigned short port, const char *ip_address) {
    printf("Initializing Winsock...\n");
    fflush(stdout);

    WSADATA wsaData;
    int wVersionRequested = MAKEWORD(2, 2);
    int wsaError = WSAGetLastError();
    wsaError = WSAStartup(wVersionRequested, &wsaData);

    if (wsaError != 0) {
        printf("\n [-] Please make sure you have WinSock DLL. \n [-] Error code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
    printf("[+] WSAStartup initialized\n");
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