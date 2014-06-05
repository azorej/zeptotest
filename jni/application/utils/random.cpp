#include "random.hpp"

std::default_random_engine& my_random_generator()
{
	//не лучшая идея использовать time как seed, но random_device не всегда реализован
	static std::default_random_engine random_generator((unsigned int)time(0));
	return random_generator;
}
