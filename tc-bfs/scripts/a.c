#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	printf("hello world!\n");
	char *p = (char *)malloc(100);
	strcpy(p, "marco polo!");
	printf("%s\n", p);
	free(p);
	return 0;
}
