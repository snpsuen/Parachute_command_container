### Introduction

A short, nifty program is written to allow any given command to be injected from outside and executed in a container. Useful for data collection, diagnostics or other tools, it is intended to work as a lite version of kubetl debug without the overhead of running a full blown container in the same pod as the target. 

![Parachute_command_into_container](Istiovirtualservicecomboaccess_20250529.png)

### TL;DR

*  The program accepts the name of a target container and a command to be executed therein as input arguments.
*  The given command is provided by the host, thus belonging to the mount namespace of the host.
*  Upon execution, the command joins the pid and network namespace of the given container.
*  The command goes on to mount /proc for process utilities like ps, top to work with the pid namespace of the container.

### Highlights of program workflow

1. Call popen(2) with the crictl CLI to find out the global process ID of the initial process of the target container.
2. Call pidfd_open(2) and setns(2) to join the pid and network namepaces of the container.
3. Call unshare(2) to .
4. Call socket() to create a socket listening on the frontend. From now on, any worker sockets arising to accept client requests from the listening socket will likewise reside in the frontend.
5. Call setns(3) to return to the original Linux network namespace of the server.
6. Go through the standard TCP concurrent server workflow with the sockets to handle each client request with popen(3) in a child process.

Build the server program from the source code in source/popen_server_ns.c.
```
gcc -o pop_server_ns popen_server_ns.c
```
