#include "main.h"

/**
 * my_strlen - Calculate the length of a string.
 * @str: Pointer to the input string.
 *
 * Return: Length of the string.
 */
size_t my_strlen(const char *str) {
    size_t len = 0;

    // Increment len until the null-terminator is encountered.
    while (str[len] != '\0') {
        len++;
    }

    return len;
}

/**
 * my_strcpy - Copy a string from source to destination.
 * @dest: Pointer to the destination buffer.
 * @src: Pointer to the source string.
 */
void my_strcpy(char *dest, const char *src) {
    // Copy characters from source to destination until null-terminator is reached.
    while ((*dest = *src) != '\0') {
        dest++;
        src++;
    }
}

/**
 * _strcmp - compares two strings
 * @s1: first input
 * @s2: second input
 * Return: integer
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	for (i = 0; (s1[i] != '\0') && (s2[i] != '\0'); i++)
	{
		if (s1[i] != s2[i])
		{
			return (s1[i] - s2[i]);
		}
	}
	return (0);
}