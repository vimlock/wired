#pragma once

#include <stddef.h>

/**
 * Allocate memory similar to malloc().
 * 
 * This will never return NULL. If the allocation fails calls
 * wPanic()
 * 
 * @param size Number of bytes to allocate. Can be 0 and a valid unique pointer
 * will be returned.
 * 
 * @return Pointer to allocated memory.
 */
void *wMemAlloc(size_t size);

/**
 * Reallocate memory similar to realloc().
 * 
 * This will never return NULL. If the allocation fails calls
 * wPanic()
 * 
 * @param ptr Pointer to previously allocated memory. Can be NULL.
 * @param size New size in bytes. Can be 0 and a valid unique pointer will
 * be returned.
 * 
 * @return Pointer to reallocated memory.
 */
void *wMemRealloc(void *ptr, size_t size);

/**
 * Free memory previously allocated with wMemAlloc() or wMemRealloc().
 * 
 * If ptr is NULL this function does nothing.
 * 
 * @param ptr Pointer to previously allocated memory. Can be NULL.
 */
void wMemFree(void *ptr);
