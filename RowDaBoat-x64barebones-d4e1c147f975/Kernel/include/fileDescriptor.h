#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H

#define MAX_FILE_DESCRIPTORS 100

//Que tipo de fd va a ser.
#define STDIN_FD 0
#define STDOUT_FD 1
#define PIPE_FD 2
#define ALT_FD 3

typedef struct fileDescriptorCDT * fds;
int createFds(int type, int pipe, void (*altRead)(char *, int),void (*altWrite)(char *, int));
int createDefaultFds();
int read(int fd, char * dest, int count);
int write(int fd, char * src, int count);



#endif