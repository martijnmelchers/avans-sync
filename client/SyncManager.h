#pragma once
#include "Settings.cpp"
#include "CommandManager.h"

class SyncManager
{
public:
	void execute(Settings& settings, CommandManager* manager);
};
