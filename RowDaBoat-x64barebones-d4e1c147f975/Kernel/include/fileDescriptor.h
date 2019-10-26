#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H

#define MAX_FILE_DESCRIPTORS 100

//Que tipo de fd va a ser.

enum FdType {STDIN_FD, STDOUT_FD, PIPE_FD, ALT_FD};

typedef struct fileDescriptorCDT * fds;
int createFds(enum FdType type, int pipe, void (*altRead)(char *, int),void (*altWrite)(char *, int));
int createDefaultFds();
int read(int fd, char * dest, int count);
int write(int fd, char * src, int count);



#endif