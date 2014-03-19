#include "new.h"

#include <FreeRTOS.h>

void * operator new(size_t size)
{
  return malloc(size);
  //return pvPortMalloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
  //vPortFree(ptr);
} 

void * operator new[](size_t size)
{
  return malloc(size);
  //return pvPortMalloc(size);
}

void operator delete[](void * ptr)
{
  free(ptr);
  //vPortFree(ptr);
}

int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
void __cxa_guard_abort (__guard *) {}; 

void __cxa_pure_virtual(void) {};

