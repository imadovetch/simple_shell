#include "main.h"

/**
 * _free - Frees a pointer and NULLs the address.
 * @p: Address of the pointer.
 *
 * Return: 1 if freed, 0 if not.
 */
int _free(char **p)
{
	if (p && *p)
	{
		free(*p);
		*p = NULL;
		return (1);
	}

	return (0);
}

/**
 * string_free - Free a string of strings.
 * @ptr: Pointer to a string.
 * Return: void
 */
void string_free(char **ptr)
{
	char *a = *ptr;

	if (!ptr)
		return;

	while (*ptr)
	{
		free(*ptr++);
	}
	free(a);
}

/**
 * my_realloc - Reallocate memory block with size adjustment.
 * @ptr: Pointer to the original memory block.
 * @new_size: New size for the memory block.
 * Return: Pointer to the reallocated memory block.
 */
void *my_realloc(void *ptr, size_t new_size)
{
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}

	if (ptr == NULL)
	{
		return (malloc(new_size));
	}

	/*Extract the old size from the metadata stored before the pointer.*/
	size_t old_size = *((size_t *)ptr - 1);

	if (old_size >= new_size)
	{
		return (ptr);/*No need to reallocate if old size is sufficient.*/
	}

	/*Allocate a new memory block and copy old data to it.*/
	void *new_ptr = malloc(new_size);

	if (new_ptr)
{
	size_t copy_size = MIN(old_size, new_size);

	memcpy(new_ptr, ptr, copy_size);
	free(ptr);
}

	return (new_ptr);
}
