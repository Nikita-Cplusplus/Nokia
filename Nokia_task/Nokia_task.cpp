#include "command.h"
#include <iostream>

int main(int argc, char **argv)
{
	try
	{
		command inst(argv[1]);
		inst.normalize();
	}
	catch (const char *ex)
	{
		std::cerr << ex;
		return EXIT_FAILURE;
	}
	return 0;
}
