#include "killcounter.h"
#include <string.h>

KillCounter_t KillCounter;

KillCounter_t::KillCounter_t() {
	this->kills = 0;
	this->InitKillTable();
}

int KillCounter_t::GetKills() {
	return this->kills;
}

void KillCounter_t::IncrementKills(const char* map_name) {
	this->kills += 1;

	// Hideous. I wish C++98 maps weren't so ass
	if (strcmp("c0a0", map_name) == 0
		|| strcmp("c0a0a", map_name) == 0
		|| strcmp("c0a0b", map_name) == 0
		|| strcmp("c0a0c", map_name) == 0
		|| strcmp("c0a0d", map_name) == 0
		|| strcmp("c0a0e", map_name) == 0
		|| strcmp("c1a0", map_name) == 0
		|| strcmp("c1a0d", map_name) == 0
		|| strcmp("c1a0a", map_name) == 0
		|| strcmp("c1a0b", map_name) == 0
		|| strcmp("c1a0c", map_name) == 0
		|| strcmp("c1a0e", map_name) == 0
		|| strcmp("c1a1", map_name) == 0
		|| strcmp("c1a1a", map_name) == 0
		|| strcmp("c1a1f", map_name) == 0
		|| strcmp("c1a1b", map_name) == 0
		|| strcmp("c1a1c", map_name) == 0
		|| strcmp("c1a1d", map_name) == 0)
	{
		KillTable["UC"] += 1;
	}

	else if (strcmp("c1a2", map_name) == 0
		|| strcmp("c1a2a", map_name) == 0
		|| strcmp("c1a2b", map_name) == 0
		|| strcmp("c1a2c", map_name) == 0
		|| strcmp("c1a2d", map_name) == 0)
	{
		KillTable["OC"] += 1;
	}

	else if (strcmp("c1a3", map_name) == 0
		|| strcmp("c1a3a", map_name) == 0
		|| strcmp("c1a3b", map_name) == 0
		|| strcmp("c1a3c", map_name) == 0
		|| strcmp("c1a3d", map_name) == 0)
	{
		KillTable["WGH"] += 1;
	}

	else if (strcmp("c1a4", map_name) == 0
		|| strcmp("c1a4k", map_name) == 0
		|| strcmp("c1a4b", map_name) == 0
		|| strcmp("c1a4f", map_name) == 0
		|| strcmp("c1a4d", map_name) == 0
		|| strcmp("c1a4e", map_name) == 0
		|| strcmp("c1a4i", map_name) == 0
		|| strcmp("c1a4g", map_name) == 0
		|| strcmp("c1a4j", map_name) == 0)
	{
		KillTable["BP"] += 1;
	}

	else if (strcmp("c2a1", map_name) == 0
		|| strcmp("c2a1a", map_name) == 0
		|| strcmp("c2a1b", map_name) == 0)
	{
		KillTable["PU"] += 1;
	}

	else if (strcmp("c2a2", map_name) == 0
		|| strcmp("c2a2a", map_name) == 0
		|| strcmp("c2a2b1", map_name) == 0
		|| strcmp("c2a2b2", map_name) == 0
		|| strcmp("c2a2c", map_name) == 0
		|| strcmp("c2a2d", map_name) == 0
		|| strcmp("c2a2e", map_name) == 0
		|| strcmp("c2a2f", map_name) == 0
		|| strcmp("c2a2g", map_name) == 0
		|| strcmp("c2a2h", map_name) == 0)
	{
		KillTable["OAR"] += 1;
	}

	else if (strcmp("c2a3", map_name) == 0
		|| strcmp("c2a3a", map_name) == 0
		|| strcmp("c2a3b", map_name) == 0
		|| strcmp("c2a3c", map_name) == 0
		|| strcmp("c2a3d", map_name) == 0
		|| strcmp("c2a3e", map_name) == 0)
	{
		KillTable["APP"] += 1;
	}

	else if (strcmp("c2a4", map_name) == 0
		|| strcmp("c2a4a", map_name) == 0
		|| strcmp("c2a4b", map_name) == 0
		|| strcmp("c2a4c", map_name) == 0)
	{
		KillTable["RP"] += 1;
	}

	else if (strcmp("c2a4d", map_name) == 0
		|| strcmp("c2a4e", map_name) == 0
		|| strcmp("c2a4f", map_name) == 0
		|| strcmp("c2a4g", map_name) == 0)
	{
		KillTable["QE"] += 1;
	}

	else if (strcmp("c2a5", map_name) == 0
		|| strcmp("c2a5w", map_name) == 0
		|| strcmp("c2a5x", map_name) == 0
		|| strcmp("c2a5a", map_name) == 0
		|| strcmp("c2a5b", map_name) == 0
		|| strcmp("c2a5c", map_name) == 0
		|| strcmp("c2a5d", map_name) == 0
		|| strcmp("c2a5e", map_name) == 0
		|| strcmp("c2a5f", map_name) == 0
		|| strcmp("c2a5g", map_name) == 0)
	{
		KillTable["ST"] += 1;
	}

	else if (strcmp("c3a1", map_name) == 0
		|| strcmp("c3a1a", map_name) == 0
		|| strcmp("c3a1b", map_name) == 0)
	{
		KillTable["FAF"] += 1;
	}

	else if (strcmp("c3a2e", map_name) == 0
		|| strcmp("c3a2", map_name) == 0
		|| strcmp("c3a2a", map_name) == 0
		|| strcmp("c3a2b", map_name) == 0
		|| strcmp("c3a2c", map_name) == 0
		|| strcmp("c3a2d", map_name) == 0
		|| strcmp("c3a2f", map_name) == 0)
	{
		KillTable["LC"] += 1;
	}

	else if (strcmp("c4a1", map_name) == 0)
	{
		KillTable["XEN"] += 1;
	}

	else if (strcmp("c4a1", map_name) == 0
		|| strcmp("c4a2", map_name) == 0
		|| strcmp("c4a2a", map_name) == 0
		|| strcmp("c4a2b", map_name) == 0)
	{
		KillTable["GL"] += 1;
	}

	else if (strcmp("c4a1a", map_name) == 0
		|| strcmp("c4a1b", map_name) == 0
		|| strcmp("c4a1c", map_name) == 0
		|| strcmp("c4a1d", map_name) == 0
		|| strcmp("c4a1e", map_name) == 0
		|| strcmp("c4a1f", map_name) == 0)
	{
		KillTable["INT"] += 1;
	}

	else
	{
		KillTable["END"] += 1;
	}
}

void KillCounter_t::ResetKills() {
	this->kills = 0;
	this->InitKillTable();
}

void KillCounter_t::SetKills(int kills) {
	this->kills = kills;
}

void KillCounter_t::InitKillTable() {
	KillTable["UC"] = 0;
	KillTable["OC"] = 0;
	KillTable["WGH"] = 0;
	KillTable["BP"] = 0;
	KillTable["PU"] = 0;
	KillTable["OAR"] = 0;
	KillTable["APP"] = 0;
	KillTable["RP"] = 0;
	KillTable["QE"] = 0;
	KillTable["ST"] = 0;
	KillTable["FAF"] = 0;
	KillTable["LC"] = 0;
	KillTable["XEN"] = 0;
	KillTable["GL"] = 0;
	KillTable["INT"] = 0;
	KillTable["END"] = 0;
}