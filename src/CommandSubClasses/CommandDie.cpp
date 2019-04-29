#include "CommandSubClasses/CommandDie.h"

#include "Entity381.h"

CommandDie::CommandDie(Entity381 *parent) :
	Command(parent), m_pitch_target(0) {
}

CommandDie::~CommandDie(void) {
}

void CommandDie::Tick(float dt) {
	if (m_running) {
	}
}

void CommandDie::Init(void) {

	Command::Init();
}