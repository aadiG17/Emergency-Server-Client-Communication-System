
# Emergency Server–Client Communication System

A **TCP-based client–server application** implemented in **C using socket programming**, designed to simulate an **emergency communication system**. This project demonstrates core **Computer Networks concepts** such as TCP/IP communication, socket lifecycle management, and reliable data transfer between multiple endpoints.

---

## Project Overview

The Emergency Server–Client Communication System allows a client to send emergency messages to a centralized server. The server listens for incoming connections, receives emergency requests, processes them, and sends acknowledgments back to the client.

This project is developed as part of an **academic Computer Networks mini-project** to gain hands-on experience with **network programming in C**.

---

##  Objectives

- Implement a reliable **TCP client–server model**
- Understand **socket creation, binding, listening, and connection handling**
- Simulate real-time **emergency message communication**
- Apply theoretical networking concepts practically

---

## Tech Stack

- **Programming Language:** C  
- **Networking Protocol:** TCP/IP  
- **Concepts Used:** Socket Programming, Client–Server Architecture  
- **Platform:** Linux / Unix-based Systems  
- **Compiler:** GCC  

---

## Project Structure

Emergency-Server-Client-System/
│
├── client.c # Client-side socket program
├── server.c # Server-side socket program
├── Design-Document.ppt # System design and architecture
├── Test-Document.pdf # Test cases and validation
└── README.md # Project documentation


---

## System Architecture

### Client
- Creates a TCP socket  
- Connects to the server using IP and port  
- Sends emergency messages  
- Receives acknowledgment from the server  

### Server
- Creates and binds a TCP socket  
- Listens for incoming client connections  
- Accepts client requests  
- Receives emergency messages  
- Sends acknowledgment responses  

---

## Working Flow

1. Server starts and listens on a predefined port  
2. Client initiates connection to the server  
3. Client sends an emergency message  
4. Server receives and processes the request  
5. Server sends acknowledgment to the client  
6. Connection is closed gracefully  

---

## How to Run the Project

### 1. Compile the Server
```bash
gcc server.c -o server
```
### 2. Compile the Client
```bash
gcc client.c -o client
```
### 3. Run the Server
```bash
./server
```
### 4. Run the Client (in a new terminal)
```bash
./client
```
## Sample Output
### Server Output

```bash
Server started...
Client connected
Emergency message received
Acknowledgment sent
```
### Client Output
```bash
Connected to server
Emergency request sent
Response received from server
```
### Testing

Verified successful client–server connection

Tested message transmission reliability

Ensured correct server acknowledgment

Validated proper socket termination

Refer to Test-Document.pdf for detailed test cases and results.

### Future Enhancements

Support for multiple clients using threads or fork()

Secure communication using encryption

GUI-based client application

Logging and monitoring of emergency requests

### Learning Outcomes

Hands-on experience with TCP socket programming

Understanding of client–server communication

Practical application of networking fundamentals

Improved debugging and error-handling skills in C

