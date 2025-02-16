#include <stdio.h>
#include <stdlib.h> // for exit()
#include <string.h> // for memset()
#include <winuser.h> // for MessageBox()
#include <winsock2.h> // for Windows Sockets API
#include <wininet.h> // for InternetOpen()
#include <ws2tcpip.h> // for Windows Sockets API
#include <windowsx.h> // for Windows API
#include <string.h> // for memset()
#include <stdio.h> // for printf()
#include <stdlib.h> // for exit()
#include <sys/types.h> // for socket()
#include <sys/socket.h> // for socket()
#include <netinet/in.h> // for struct sockaddr_in
#include <unistd.h> // for close()
#include <arpa/inet.h> // for inet_ntoa()
#include <sys/wait.h> // for waitpid()
#include <sys/select.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

// Define some common types for easier porting between platforms (Windows/Linux) and architectures (32/64 bit)
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif


// Define some common types for easier porting between platforms (Windows/Linux) and architectures (32/64 bit)
typedef int8_t CHAR;      // CHAR = signed 8 bit value
typedef int16_t SHORT;    // SHORT = signed 16 bit value
typedef int64_t LONGLONG; // LONGLONG = signed 64 bit value
typedef uint64_t QWORD;   // QWORD = unsigned 64 bit value
typedef uint32_t DWORD;   // DWORD = unsigned 32 bit value
typedef uint16_t WORD;    // WORD = unsigned 16 bit value
typedef uint8_t BYTE;     // BYTE = unsigned 8 bit value

// Define the server IP address and port number
/**
 * @brief The IP address of the server.
 * The selected code struct sockaddr_in server; is a declaration of a structure variable named server of type sockaddr_in.
 * This structure is used to specify an endpoint address to which a socket can be bound. The sockaddr_in structure is
 * defined in the <netinet/in.h> header file and is commonly used in network programming to handle IPv4 addresses.
 * The sockaddr_in structure includes several fields, such as sin_family, sin_port, and sin_addr,
 * which are used to specify the address family (e.g., AF_INET for IPv4), the port number, and the IP address, respectively.
 * In the context of the provided code, the server variable is used to set up the server address for binding the socket.
 */

// Create a socket structure for the server
struct sockaddr_in server;
int server_socket;
int client_socket;
int return_code;
int new_return_code;
int initial_return_code;

// Create a socket structure for the server
struct sockaddr_in server_addr;
unsigned int server_len = sizeof(server_addr);
unsigned short serverPort = 1999;
char *ServerIP = "192.168.1.78";
int server_socket;
int client_socket;
int return_code;
int new_return_code;
int initial_return_code;

// Define a structure to store the number of bytes sent and received
struct BytesData {
    int bytes_received;
    int bytes_sent;
    int bytes_total;
    int bytes_remaining;
    int bytes_to_send;
    int bytes_sent_total;
    int bytes_sent_remaining;
};
struct ServerData {
    int server_socket;
    int client_socket;
    int return_code;
    int new_return_code;
    int initial_return_code;
    struct sockaddr_in server_addr;
    unsigned int server_len;
    unsigned short serverPort;
    char *ServerIP;
};


// Declare WSADATA structure to store information about the Windows Sockets implementation.


void flush_stdout() {
    fflush(stdout);
    fflush(stderr);
    fflush(stdin);
};
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    /**
     * @brief The entry point for a Windows application.
     * This function is the entry point for a Windows application. It is
     * responsible for initializing the application and starting the main
     * event loop.
     *
     * @param hInstance The handle of the module that contains the application.
     * @param hPrevInstance This value is always NULL.
     * @param lpCmdLine The command line arguments passed to the application.
     * @param nCmdShow The show state of the window.
     * @return The exit code of the application.
     */


    // #1 Hide the console window from the user (optional)
    HWND hWnd = GetConsoleWindow();
    AllocConsole(); // Allocate a new console
    hWnd = FindWindowA("ConsoleWindowClass", NULL); // Find the console window
    ShowWindow(hWnd,0); // ShowWindow(hWnd, SW_HIDE); Hide the console window (0 = SW_HIDE)

    // #2 Redirect standard input, output, and error streams
    freopen("CONOUT$", "w", stdout); // Redirect standard output
    freopen("CONOUT$", "w", stderr); // Redirect standard error
    freopen("CONIN$", "r", stdin);   // Redirect standard input

    // #3 Initialize the server socket and call the server initialization function to start listening for incoming connections
    int server_socket = initialize_server_socket(serverPort, ServerIP);
    printf("[!] Initial socket detials,\n [+] IP: %s\n [+] Port: %hu\n [+]Server Socket: %d\n",ServerIP, serverPort,
    server_socket, WSAGetLastError());
    sleep(3);
    flush_stdout();

    if (server_socket == 0 || new_return_code == 0) {
        printf("\n[+] Server initialized\n [+] IP: %s\n [+] Port: %d\n", ServerIP, serverPort);
        printf("\n [+] Server Socket: %d (last error code: %d)\n", server_socket, WSAGetLastError());
        printf("\n [+] Server is now ready to accept connections\n");
        flush_stdout();
        server_socket = initialize_server_socket(serverPort, *ServerIP);
        sleep(3);
        flush_stdout();

        /// Add your code to accept connections and handle clients here ////
        /// add code to accept connections and handle clients here ////
    } else {
        printf("\n[-] Server initialization failed\n [-] IP: %s\n [-] Port: %d\n", ServerIP, serverPort);
        printf("[-] Error code: %d\n", WSAGetLastError());
        printf("[-] retrying...");
        sleep(3);
        flush_stdout();

    }

    // #4 Handle potential errors, if the server initialization fails, then the error code is retrieved using WSAGetLastError(). This error code is stored in the variable return_code and is used to provide diagnostic information about why the initialization failed:
    // *The selected code int new_return_code = WSAGetLastError(); is part of a loop that attempts to initialize a server socket and handle potential errors during the initialization process. This code is used to retrieve the last error code generated by the Windows Sockets API (Winsock).  In the context of the loop, the server socket initialization is attempted, and if it fails, the error code is retrieved using WSAGetLastError(). This error code is stored in the variable return_code and is used to provide diagnostic information about why the initialization failed:*/int new_return_code = WSAGetLastError();
    while (server_socket != 0 || new_return_code != 0) {
        int return_code = WSAGetLastError();
        printf("\n[-]Server initialization failed\n [-] IP: %s\n [-] Port: %d\n", ServerIP, serverPort);
        printf("[-] Error code: %d\n retrying...", return_code);
        server_socket = initialize_server_socket(serverPort, *ServerIP);
        new_return_code = WSAGetLastError();
        sleep(3);
    }
}

// Initialize the server socket and start listening for incoming connections on the specified port and IP address.

int initialize_server_socket(unsigned short port, const char *ip_address) {

printf("Initializing Winsock...\n");

    fflush(stdout); // flush the output buffer
    printf("Initializing Winsock...\n");
    fflush(stdout); // flush the output buffer

    // #1 Set up the server address structure and initialize Winsock [Windows only- wsadata structure]

    WSADATA wsaData; // Declare WSADATA structure to store information about the Windows Sockets implementation.
    int wVersionRequested = MAKEWORD(2, 2); // Request Winsock version 2.2 -- Initialization of Winsock, to generate the versioning word expected by WSAStartup.
    int wsaError = WSAGetLastError() ; // Retrieve the last error code generated by the Windows Sockets API (Winsock).
    wsaError = WSAStartup(wVersionRequested, &wsaData); // Initialize Winsock

    if (wsaError != 0) {
        printf("\n [-] Please make sure you have WinSock DLL. \n [-] Error code: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE); //  return 1;
    }
    printf("[+] WSAStartup initialized\n");
    fflush(stdout); // flush the output buffer

    int server_socket = socket(AF_INET, SOCK_STREAM, 0); // Create a socket structure for the server
    if (server_socket == -1) {
        perror("[-] Socket creation failed");
        return EXIT_FAILURE;
    }

    printf("[+] Socket created: %d\n", server_socket);
    fflush(stdout); // flush the output buffer

    // 2.1 Set up the server address structure
    printf("\n [!] Structuring Server Service \n");
    sleep(2);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET; // Set the address family to AF_INET (IPv4)
    server_addr.sin_port = htons(port); // Convert port number to network byte order
    server_addr.sin_addr.s_addr = inet_addr(ip_address); // Convert server IP from text to binary form

    printf("\n [+] sin_family: %d, sin_port: %d, sin_addr.s_addr: %lu\n",
           server_addr.sin_family,
           ntohs(server_addr.sin_port),
           (unsigned long)server_addr.sin_addr.s_addr);

    printf("\n [+] Binding Socket \n");
    sleep(2);
    fflush(stdout); // flush the output buffer

    // #3 Bind the socket to the address and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("[-] Bind failed");
        close(server_socket);
        return -1;
    }
    printf("[+] Bind done\n");
    fflush(stdout); // flush the output buffer

    // #4 Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        perror("[-] Listen failed");
        close(server_socket);
        return -1;
    }
    printf("[+] Server listening on %s:%d\n", ip_address, port);
    fflush(stdout); // flush the output buffer


    return server_socket;
}

// Initialize the server socket and start listening for incoming connections on the specified port and IP address.