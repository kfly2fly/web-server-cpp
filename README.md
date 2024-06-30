# HTTP Server with C++
This project is a basic implementation of a TCP HTTP server using C++! 

## Motivation
This HTTP server is an intentional effort to recreate the wheel for educational purposes. 
At the time of writing this, much of my professional work as well as personal interest has been invested in web development.
A key part of serving content in web development is the HTTP server (NGINX, Apache Web Server, etc).
While previously familiar with the basic function of HTTP, this project takes a deeper dive into the inner workings of HTTP.

## About the Project
A basic HTTP server is an Application layer protocol that allows networked devices to communicate.
While we could use any Transport layer protocol TCP is the most commonly used to implement HTTP since it offers a reliable connection.

The data sent in HTTP follows a specific set of known rules. 
These rules are outlined in RFC documents 7230-7235.
The rules define the layout of the data.
Maybe the most notable rule is that the body of the message is prefixed with standardized headers that give the reciever information about the message.

The HTTP server implemented in this project can be outlined as follows:
* Create a TCP socket 
* Bind the socket to a specific port and IP address
* Listen and store requests in a queue
* Accept a request and spin off a new socket that is able to respond accordingly
* Close the responding socket and continue to listen for new requests

### Create a Socket
The fist step in creating an HTTP server is defining a socket. 
A socket is a way of connecting two nodes on a network so that they can communicate with each other.
On Windows we can create a socket abstraction with the socket function from the Winsocket library.
```cpp
SOCKET socket(int domain, int type, int protocol);
```

### Bind the Socket
Our socket can then be assigned to an IP address and port. 
We can pass the required information via a sockaddr struct.
This struct consists of the socket protocol, desired port and IP address.
This struct is then passed into the bind function to define the socket address.
```cpp
int bind(SOCKET sock, const struct sockaddr *addr, socklen_t addrlen);
```

### Listen for and Accept Connections
The socket is then setup for listening and gathers incoming connections requests.
This is done through the listen() function where the parameters are the socket itself and number of connections threads to hold in queue.
```cpp
int listen(SOCKET s, int backlog);
```
We can accept a connection request by accepting the request.
This is done through the accept() function which takes the socket, a sockaddr, and address length as parameters.
The accept call will create a new socket to make the connection while the original socket is kept free to continue to listen for new connections.
```cpp
SOCKET accept(SOCKET s, sockaddr *addr, int *addrlen);
```

### Read the Request
The data in the incoming request can be read through the recv() function.
This function takes in the socket, a buffer to store the bytes, a length to read, and a flag.
The char buffer can then be processed.
```cpp
int recv(SOCKET s, char *buf, int len, int flags);
```

### Send the Response
Once the request is read, a response is formed appropriately.
A character buffer can be sent to the requester through the send() function which takes in similar arguments as the recv() function.
```cpp
int send(SOCKET s, const char *buf, int len, int flags);
```

## Resources
[Building an HTTP Server From Scratch in C++](https://osasazamegbe.medium.com/showing-building-an-http-server-from-scratch-in-c-2da7c0db6cb7)
[HTTP Server: Everything you need to know to Build a simple HTTP server from scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)
[What is the difference between a socket and a port?](https://www.techtarget.com/searchnetworking/answer/What-is-the-difference-between-a-port-and-a-socket#:~:text=Essentially%2C%20a%20socket%20acts%20as,element%20within%20that%20specific%20address)

## License

[MIT](https://choosealicense.com/licenses/mit/)

