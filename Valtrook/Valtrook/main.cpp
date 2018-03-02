#include "Random.h"
#include <time.h>
#include "Engine.h"

int main() {
	Val::Random::Instance->setSeed(static_cast<long>(time(NULL)));

	Val::Engine engine = Val::Engine();
	engine.start();

	return 0;
}