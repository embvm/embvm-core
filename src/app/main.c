/*
 * Copyright © 2020 Embedded Artistry LLC.
 * See LICENSE file for licensing information.
 */

#include <example/example.h>

int main(void)
{
	const int expected = 42;
	int val = ret42();

	return (expected != val);
}
