#include <printf.h>

// Forward declare because including <stdio.h> will cause printf conflicts
int putchar(int);

// TODO: move this to a better home
void _putchar(char ch)
{
	putchar(ch);
}
