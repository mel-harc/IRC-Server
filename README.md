# IRC Server 🖥️

Welcome to the IRC Server project! This project implements a basic Internet Relay Chat (IRC) server, allowing clients to communicate with each other in a chatroom-like environment. The server supports multiple channels, private messages, and basic user management commands.

---

## **Project Overview** 🔍

This project is a simple implementation of an IRC (Internet Relay Chat) server in C. It provides functionality for handling multiple users and channels, sending messages, and managing basic server commands like joining and leaving channels.

The main features include:

- Multi-user support
- Multiple channels with user management
- Private messaging
- Basic server commands (JOIN, PART, PRIVMSG, etc.)

---

## **Features 🚀**

### **1. Multi-User Support** 👥
The server can handle multiple connected clients at once. Each user can join one or more channels, send and receive messages in real-time.

### **2. Channels** 📡
Users can join or leave different chat channels. Each channel has a unique name and supports multiple users communicating in it simultaneously.

### **3. Private Messages** ✉️
Users can send private messages directly to each other, even if they're not in the same channel.

### **4. Server Commands** 📜
The IRC server supports standard IRC commands, such as:

- `/JOIN <channel>`: Join a specified channel.
- `/PART <channel>`: Leave a specified channel.
- `/PRIVMSG <user> <message>`: Send a private message to a user.
- `/NICK <nickname>`: Set your nickname.
- `/USER <username>`: Set your username.

---

## **How to Use 🖥️**

### **1. Clone the repository**
First, clone the repository to your local machine.

```bash
git clone git@github.com:mel-harc/irs.git
cd irc-server
```

### **2. Compile the server**
Use the Makefile to compile the server.

```bash
make
```
This will generate an executable called irc_server.

### 3. Run the server
Run the IRC server by executing the following command:

```bash
./irc_server
```

The server will start on port 6667 by default, and you will see a message indicating that it's running and waiting for connections.

### **4. Connect with an IRC Client***
To connect to the server, use an IRC client (such as HexChat, irssi, or even the command-line client). Connect to the server using the following parameters:

Server: localhost (or the server IP if running remotely)
Port: 6667 (default port)
Once connected, you can register your nickname and start chatting by joining channels and sending messages.

## **Commands List 📝**

### **1. JOIN**
Join a channel.

```bash
/JOIN <channel_name>
```
### **2. PART**
Leave a channel.
```bash
/PART <channel_name>
```

### **3. PRIVMSG**
Send a private message to a user.
```bash
/PRIVMSG <username> <message>
```

### **4. NICK**
Set your nickname.
```bash
/USER <username>
```

## **Technical Details 🛠️**

- Language: C
- Protocols: Custom protocol based on IRC RFC
- Port: Default 6667 for communication
- Libraries:
     - unistd.h for socket handling
     - stdio.h and string.h for message parsing
     - stdlib.h for memory allocation
 
## ** Acknowledgements 🎓**
Special thanks to the authors and contributors of the original IRC protocols and to the open-source community for making resources available that helped in building this project.


## by mel-harc 



