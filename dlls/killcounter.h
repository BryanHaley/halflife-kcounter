#pragma once

#pragma warning(disable: 4786)
#include <stdlib.h>
#include <map>
#include <string.h>

class KillCounter_t
{
private:
	void InitKillTable();
	int kills;
public:
	KillCounter_t();
	int GetKills();
	void SetKills(int kills);
	void ResetKills();
	void IncrementKills(const char *map_name);

	std::map<const char*, int> KillTable;
};

extern KillCounter_t KillCounter;

#define HANDLE_KILL_COUNTER_KILL() \
if (m_iKillCounterEligble && !( pev->spawnflags & SF_IGNORE_KILL )) { \
	CBaseEntity *pPlayer = UTIL_FindEntityByClassname( NULL, "player" ); \
	if ((CVAR_GET_FLOAT( "kc_hit_marker" ) != 0)) { \
		EMIT_SOUND( pPlayer->edict(), CHAN_AUTO, "kc/hitmarker.wav", 1.0f, ATTN_NONE ); \
	} \
	KillCounter.IncrementKills(STRING(gpGlobals->mapname)); \
	char num_kills[16]; \
	itoa(KillCounter.GetKills(), num_kills, 10); \
	char report[128]; \
	report[0] = '\0'; \
	strcat(report, "report_to_demo "); \
	strcat(report, STRING(pev->classname)); \
	strcat(report, " '"); \
	strcat(report, STRING(pev->targetname)); \
	strcat(report, "' killedon "); \
	strcat(report, STRING(gpGlobals->mapname)); \
	strcat(report, " total "); \
	strcat(report, num_kills); \
	strcat(report, "\n"); \
	ALERT( at_console, report ); \
	CLIENT_COMMAND ( pPlayer->edict(), report); \
	m_iKillCounterEligble = 0; \
}

#define MAKE_KILL_COUNTER_ELIGBLE() \
if (!(pev->spawnflags & SF_MAX_ONLY) || (CVAR_GET_FLOAT( "kc_max_percent" ) != 0)) { \
	m_iKillCounterEligble = 1; \
} else if ((CVAR_GET_FLOAT( "kc_ghosts" ) != 0)) { \
	pev->rendermode = kRenderTransAdd; \
	pev->renderamt = 128; \
}