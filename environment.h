#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

void init(void);
void reset(void);

void step(unsigned short action);

void* save(void);
void load(const void* savestate);
size_t get_state_size(void);

void close(void);

#endif