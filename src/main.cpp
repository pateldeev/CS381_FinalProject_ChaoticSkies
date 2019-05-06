#include "Engine.h"

int main(int argc, char *argv[]) {
	Engine *engine = new Engine();

	engine->Init();
	engine->Run();
	engine->Cleanup();

	delete engine;

	return 0;
}
