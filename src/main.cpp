
#if 1
#include "Engine.h"

int main(int argc, char *argv[]) {
	Engine *engine = new Engine();

	engine->Init();
	engine->Run();
	engine->Cleanup();

	delete engine;

	return 0;
}
#else
#include <mutex>
#include <iostream>
using namespace std;

static mutex m;

int main(int argc, char *argv[]) {
	cout << "Hello" << endl;
}
#endif
