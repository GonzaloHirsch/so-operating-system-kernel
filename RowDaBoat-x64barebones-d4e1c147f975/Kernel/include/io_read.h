#ifndef IO_READ_H
#define IO_READ_H

int read_key();
int check_key();

uint8_t read_port(uint16_t port);
void write_port(uint16_t port, uint8_t data);

void _over_clock(uint16_t value);

#endif
