#include "main.h"

/**
 * my_strlen - Calculate the length of a string.
 * @str: Pointer to the input string.
 *
 * Return: Length of the string.
 */
size_t my_strlen(const char *str)
{
	size_t len = 0;

	while (str[len] != '\0')
	{
		len++;
	}

	return (len);
}

/**
 * my_strcpy - Copy a string from source to destination.
 * @dest: Pointer to the destination buffer.
 * @src: Pointer to the source string.
 */
void my_strcpy(char *dest, const char *src)
{
	while ((*dest = *src) != '\0')
	{
		dest++;
		src++;
	}
}

/**
 * _strcmp - Compares two strings.
 * @s1: First input.
 * @s2: Second input.
 *
 * Return: Integer indicating comparison result.
 */
int _strcmp(char *s1, char *s2)
{
	int i;

	for (i = 0; (s1[i] != '\0') && (s2[i] != '\0'); i++)
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
	}
	return (0);
}
/**
 * _strdup - Duplicates a string in memory.
 * @str: The input string to duplicate.
 *
 * Return: Pointer to the duplicated string or NULL on failure.
 */
char *_strdup(char *str)
{
	char *duplicate;
	size_t len, i;

	if (str == NULL)
		return (NULL);

	len = my_strlen(str);

	duplicate = malloc((len + 1) * sizeof(char));
	if (duplicate == NULL)
		return (NULL);

	for (i = 0; i <= len; i++)
	{
		duplicate[i] = str[i];
	}

	return (duplicate);
}
