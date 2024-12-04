#include <stddef.h>

void init(void);

void step(unsigned short action);

void* save(void);
void load(const void* savestate);
size_t get_state_size(void);
void free_state(void* savestate);
