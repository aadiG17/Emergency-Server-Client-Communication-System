#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>     // For fcntl()
#include <errno.h>
#include <sys/select.h> // For select()

#define PORT 5555
#define BROADCAST_IP "255.255.255.255"
#define BUFFER_SIZE 1024
#define RETRY_INTERVAL_SEC 0        // Time in seconds
#define RETRY_INTERVAL_USEC 100000  // Time in microseconds (0.1 seconds)

// Array to map user inputs to service names
const char *service_list[] = {
    "Police Station Number",
    "Ambulance Number",
    "Fire Station Number",
    "Vehicle Repair Number",
    "Food Delivery Number",
    "Blood Bank Number"
};

// Function to query the emergency server
void query_emergency_server(const char *service) {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    socklen_t server_addr_len = sizeof(server_addr);

    // Create a UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Enable broadcast option for the socket
    int broadcast_enable = 1;
    if (setsockopt(client_socket, SOL_SOCKET, SO_BROADCAST, &broadcast_enable, sizeof(broadcast_enable)) < 0) {
        perror("Error setting socket options for broadcast");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, BROADCAST_IP, &server_addr.sin_addr);

    // Send the service request as a broadcast message in a loop until we receive a response
    int response_received = 0;
    while (!response_received) {
        // Send the service request to the server
        sendto(client_socket, service, strlen(service), 0,
               (struct sockaddr *)&server_addr, sizeof(server_addr));

        // Set up the file descriptor set
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(client_socket, &read_fds);

        // Set up the timeout for select
        struct timeval timeout;
        timeout.tv_sec = RETRY_INTERVAL_SEC;
        timeout.tv_usec = RETRY_INTERVAL_USEC;

        // Use select to wait for a response with a timeout
        int activity = select(client_socket + 1, &read_fds, NULL, NULL, &timeout);
        if (activity > 0 && FD_ISSET(client_socket, &read_fds)) {
            // Data available to read from the socket
            int bytes_received = recvfrom(client_socket, buffer, BUFFER_SIZE - 1, 0,
                                          (struct sockaddr *)&server_addr, &server_addr_len);
            if (bytes_received >= 0) {
                buffer[bytes_received] = '\0'; // Null-terminate the received data
                printf("Response from server: %s\n", buffer);
                response_received = 1; // Exit the loop on successful response
            } else {
                perror("Error receiving data from server");
            }
        }
    }

    close(client_socket);
}

int main() {
    int user_choice;

    // Loop to continuously prompt the user for service requests
    while (1) {
        // Display the service options
        printf("\nEnter the service you need:\n");
        printf("1. Police Station Number\n");
        printf("2. Ambulance Number\n");
        printf("3. Fire Station Number\n");
        printf("4. Vehicle Repair Number\n");
        printf("5. Food Delivery Number\n");
        printf("6. Blood Bank Number\n");
        printf("Type '0' to exit\n");
        printf("Enter your choice: ");

        // Get the user's choice
        if (scanf("%d", &user_choice) != 1) {
            // Handle invalid input
            printf("Invalid input. Please enter a number between 0 and 6.\n");
            // Clear the input buffer
            while (getchar() != '\n'); // discard invalid input
            continue;
        }

        // Check if the user wants to exit the program
        if (user_choice == 0) {
            printf("Exiting the client program.\n");
            break;
        }

        // Validate user input and send the request if valid
        if (user_choice >= 1 && user_choice <= 6) {
            query_emergency_server(service_list[user_choice - 1]);
        } else {
            printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    }

    return 0;
}
