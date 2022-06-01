#pragma once

#include <stddef.h>

void operator delete(void * ptr, size_t size);
void operator delete[](void * ptr, size_t size);