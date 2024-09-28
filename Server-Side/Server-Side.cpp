// requirements : recv data from clients then log the data to csv or .txt file, using udp socket

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {
    WSADATA wsaData;
    SOCKET sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    int addr_len = sizeof(client_addr);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // Create a UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on any available interface
    server_addr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    std::cout << "UDP server listening on port " << PORT << std::endl;

    // Receive data in a loop
    while (true) {
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
            (struct sockaddr*)&client_addr, &addr_len);
        if (recv_len == SOCKET_ERROR) {
            std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
            break;
        }
        buffer[recv_len] = '\0'; // Null-terminate the received data

        // Convert client address to string
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));

        std::cout << "Received message: " << buffer
            << " from " << client_ip
            << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Optional: Send a response back to the client
        const char* response = "Message received";
        sendto(sockfd, response, strlen(response), 0,
            (struct sockaddr*)&client_addr, addr_len);
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}

