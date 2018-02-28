#include "Random.h"
#include <time.h>

int main() {
	Val::Random::Instance->setSeed(time(NULL));


	return 0;
}