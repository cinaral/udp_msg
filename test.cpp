#include <cstdio>

int
main()
{
	struct timeval read_to;

	printf("%d", read_to.tv_usec);
	return 0;
}