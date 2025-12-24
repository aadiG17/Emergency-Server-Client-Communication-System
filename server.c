#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>     // For fcntl()
#include <errno.h>       // For errno and EINTR
#include <sys/select.h> // For select()

#define PORT 5555
#define BUFFER_SIZE 1024

// Emergency service data
typedef struct {
    char *service;
    char *number;
} EmergencyService;

EmergencyService emergency_services[] = {
    {"Police Station Number", "911"},
    {"Ambulance Number", "912"},
    {"Fire Station Number", "913"},
    {"Vehicle Repair Number", "914"},
    {"Food Delivery Number", "915"},
    {"Blood Bank Number", "916"},
};

// Function to find the emergency number for a given service
const char* get_emergency_number(const char *service) {
    static char response[BUFFER_SIZE]; // Static to retain memory after function call

    for (int i = 0; i < sizeof(emergency_services) / sizeof(EmergencyService); i++) {
        if (strcmp(service, emergency_services[i].service) == 0) {
            snprintf(response, sizeof(response), "The %s is %s", service, emergency_services[i].number);
            return response;
        }
    }
    return "Invalid request. Service not available.";
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_addr_len = sizeof(client_addr);

    // Create a UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set the socket to non-blocking mode
    int flags = fcntl(server_socket, F_GETFL, 0);
    fcntl(server_socket, F_SETFL, flags | O_NONBLOCK);

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Emergency server started on port %d, waiting for client requests...\n", PORT);

    // Initialize the file descriptor set
    fd_set read_fds;
    int max_sd = server_socket;

    while (1) {
        // Clear the socket set and add the server socket to the set
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);

        // Use select to check for data on the socket with a timeout of 1 second
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(max_sd + 1, &read_fds, NULL, NULL, &timeout);

        if (activity < 0 && errno != EINTR) {
            perror("select error");
        }

        // Check if there is data to read from the socket
        if (FD_ISSET(server_socket, &read_fds)) {
            // Receive request from client
            int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE - 1, 0,
                                          (struct sockaddr *)&client_addr, &client_addr_len);
            if (bytes_received < 0) {
                perror("Error receiving data");
                continue;
            }
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            printf("Received request: %s from client %s:%d\n",
                   buffer, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            // Get the response based on the client request
            const char *response = get_emergency_number(buffer);

            // Send response back to the client
            sendto(server_socket, response, strlen(response), 0,
                   (struct sockaddr *)&client_addr, client_addr_len);
        } else {
            // No data available to read; server remains responsive
            printf("No data available to read; server is still responsive.\n");
        }
    }

    close(server_socket);
    return 0;
}





