### Introduction

A short, nifty program is written to allow any given command to be injected from outside and executed in a container. Useful for data collection, diagnostics or other tools, it is intended to work as a lite version of kubetl debug without the overhead of running a full blown container in the same pod as the target. 

![Parachute_command_into_container](Istiovirtualservicecomboaccess_20250529.png)

### TL;DR

*  The program accepts the name of a target container and a command to be executed therein as input arguments.
*  The given command is provided by the host, thus belonging to the mount namespace of the host.
*  Upon execution, the command joins the pid and network namespace of the given container.
*  The command goes on to mount proc for process utilities like ps, top to work with the pid namespace of the container.

### Program hightlights

Running on a backend pod, the server is designed to perform the following key tasks.

1. Call system(3) to use the crictl CLI to retrieve the container ID and process ID $pid of the frontend pod.
2. Open /proc/$pid/ns/net that represents the Linux network namespace of the frontend container process.
3. Call setns(3) to set the Linux network namespace of the server temporarily to that of the frontend.
4. Call socket() to create a socket listening on the frontend. From now on, any worker sockets arising to accept client requests from the listening socket will likewise reside in the frontend.
5. Call setns(3) to return to the original Linux network namespace of the server.
6. Go through the standard TCP concurrent server workflow with the sockets to handle each client request with popen(3) in a child process.

Build the server program from the source code in source/popen_server_ns.c.
```
gcc -o pop_server_ns popen_server_ns.c
```
