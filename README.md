# 1. ```udp_msg```: Key-value arrays over UDP

This lightweight header-only library sends/receives key and value arrays over UDP packets. 
- Can be used to open and manage sockets, translate multiple key-value arrays from and into messages. 
- Maximum message size is limited to 512 MB to prevent fragmentation of the message into multiple UDP packets.
- The default and the only translation done by this library interprets the value array as the specified value type. However, the keys can be used to communicate the types of the values if it is different from ```VALUE_T```.
- Compatible with Linux and Windows (*tested on Linux Mint and Windows 10).

---  

- [1. ```udp_msg```: Key-value arrays over UDP](#1-udpmsg-key-value-arrays-over-udp)
- [2. Installation](#2-installation)
- [3. Usage](#3-usage)
- [4. Examples](#4-examples)
	- [4.1. Example: Receive](#41-example-receive)
	- [4.2. Example: Send](#42-example-send)
- [5. Key-Value arrays](#5-key-value-arrays)

# 2. Installation
  
Include all or some of the headers in ```include/``` into your project. You could use [FetchContent()](https://cmake.org/cmake/help/latest/module/FetchContent.html) in your ```CMakeLists.txt```:
```CMake
FetchContent_Declare(udp_msg URL https://github.com/cinaral/udp_msg/releases/download/<RELEASE_TAG>/src.zip)
FetchContent_MakeAvailable(udp_msg)
```

Use CTest to test the library before using. There is one test, which is a single send-then-receive test over the loopback address.  

# 3. Usage

Create a socket object to send and receive:
```Cpp
	udp_msg::sock<KEY_T, VAL_T, KEY_DIM, VAL_DIM> udp_msg_obj(
		const char hostname[], 
		const unsigned port, 
		const bool is_binding = true,
	    const bool is_nonblocking = true, 
		const int af = AF_INET, 
		const int type = SOCK_DGRAM,
	    const int protocol = 0);
	//* send
	udp.send(key_sent, val_sent);
```
Set ```is_binding``` to false if you do not wish to bind the socket. Set ```is_nonblocking``` to false if you wish blocking behavior. ```af```, ```type```, and ```protocol``` are passed to ```::socket(af, type, protocol)``` (defined in ```sys/socket.h```) and can be used to change the socket type.

To receive use ```udp_msg::receive()```:
```Cpp
int receive(KEY_T (&key_arr)[KEY_DIM], VAL_T (&val_arr)[VAL_DIM]);
```

To send use ```udp_msg::send()```:
```Cpp
int	send(const KEY_T (&key_arr)[KEY_DIM], const VAL_T (&val_arr)[VAL_DIM]);
```

# 4. Examples

## 4.1. Example: Receive

```Cpp
#include "udp_msg.hpp"
//...
int
main()
{
	//* create a socket to receive one char and one float from 192.168.1.1:8887
	udp_msg::sock<char, float, 1, 1> soc("192.168.1.1", 8887);
	//* buffer to hold the received message
	char key_arr[1]; 
	float val_arr[1];
	
	if (soc.receive(key_arr, val_arr) > 0) {
		//* received something...
	}
	return 0;
}
```
See [receive.cpp](./examples/receive.cpp) for details.

## 4.2. Example: Send

```Cpp
#include "udp_msg.hpp"
//...
int
main()
{
	//* create a socket to send ASCII '0' and 3.14f to 192.168.1.2:8887
	udp_msg::sock<char, float, 1, 1> soc("192.168.1.2", 8887);
	
	if (udp.send(0x30, 3.14) > 0) {
		//* send successful...
	}
	return 0;
}
```
See [send.cpp](./examples/send.cpp) for details.

# 5. Key-Value arrays

This library sends and receives two separate arrays, the first array contains the ```KEY_DIM``` keys of type ```KEY_T```, and the second array contains ```VALUE_DIM``` values of type ```VAL_T```. 
- Size of 1 message = ```KEY_DIM * sizeof(KEY_T) + VALUE_DIM * sizeof(VAL_T)``` bytes
- Number of keys need not be equal to number of values. 
- Example 52-byte message, which consists of four (```KEY_DIM=4```) three-dimensional coordinate frame IDs represented by characters (```KEY_T=char```, one byte), and twelve (```VALUE_DIM=12```) coordinate values represented by single-precision floating-point numbers (```VAL_T=float```, four bytes):
  | Key (```char[4]```) |    Value (```float[12]```)     |
  | :-----------------: | :----------------------------: |
  |          A          |      $x_A$, $y_A$, $z_A$       |
  |          B          |       $x_B$, $y_B$,$z_B$       |
  |          C          | $\theta_C$, $\phi_C$, $\psi_C$ |
  |          D          | $\theta_D$, $\phi_D$,$\psi_D$  |
  ```Cpp
	char key[4] = {'A', 'B', 'C', 'D'};
	float val[12] = {x_A, y_A, z_A, x_B, y_B, z_B, theta_C, phi_C, psi_C, theta_D, phi_D, psi_D};
  ```
