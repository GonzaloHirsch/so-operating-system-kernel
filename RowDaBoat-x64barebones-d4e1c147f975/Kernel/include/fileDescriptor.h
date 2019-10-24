#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H

#define MAX_FILE_DESCRIPTORS 100

//Que tipo de fd va a ser.
#define STD_FD 0
#define PIPE_FD 1
#define ALT_FD 2

typedef struct fileDescriptorCDT * fds;
int createFds(int type, int pipe, void (*altRead)(char *, int),void (*altWrite)(char *, int));
int read(int fd, char * dest, int length);
int write(int fd, char * src, int length);



#endif