#include "Portability.hpp"
#include <stdlib.h>

void operator delete(void * ptr, size_t size){
    free(ptr);
}

void operator delete[](void * ptr, size_t size){
    free(ptr);
}