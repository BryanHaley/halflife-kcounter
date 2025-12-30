/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// Health.cpp
//
// implementation of CHudHealth class
//

#include "STDIO.H"
#include "STDLIB.H"
#include "MATH.H"

#include "hud.h"
#include "cl_util.h"
#include "parsemsg.h"
#include <string.h>
#include <stdio.h>

#pragma warning(disable: 4786)
#include <map>
#include <string.h>
#include <fstream>


DECLARE_MESSAGE(m_Health, Health )
DECLARE_MESSAGE(m_Health, Damage )
DECLARE_MESSAGE(m_Health, Kills )

#define PAIN_NAME "sprites/%d_pain.spr"
#define DAMAGE_NAME "sprites/%d_dmg.spr"

std::map<const char*, int> map_kills_table;
std::map<const char*, int> map_counts_table;
// std::map<const char*, const char*> level_chapters; // Why are C++98 maps so ass???

int giDmgHeight, giDmgWidth;

int giDmgFlags[NUM_DMG_TYPES] = 
{
	DMG_POISON,
	DMG_ACID,
	DMG_FREEZE|DMG_SLOWFREEZE,
	DMG_DROWN,
	DMG_BURN|DMG_SLOWBURN,
	DMG_NERVEGAS, 
	DMG_RADIATION,
	DMG_SHOCK,
	DMG_CALTROP,
	DMG_TRANQ,
	DMG_CONCUSS,
	DMG_HALLUC
};

void init_map_kills(void)
{
	// Init kill table
	map_kills_table["END"] = 0;
	map_kills_table["INT"] = 0;
	map_kills_table["GL"] = 0;
	map_kills_table["XEN"] = 0;
	map_kills_table["LC"] = 0;
	map_kills_table["FAF"] = 0;
	map_kills_table["ST"] = 0;
	map_kills_table["QE"] = 0;
	map_kills_table["RP"] = 0;
	map_kills_table["APP"] = 0;
	map_kills_table["OAR"] = 0;
	map_kills_table["PU"] = 0;
	map_kills_table["BP"] = 0;
	map_kills_table["WGH"] = 0;
	map_kills_table["OC"] = 0;
	map_kills_table["UC"] = 0;
}

void load_count_file(void)
{
	// Init count table
	map_counts_table["END"] = 0;
	map_counts_table["INT"] = 0;
	map_counts_table["GL"] = 0;
	map_counts_table["XEN"] = 0;
	map_counts_table["LC"] = 0;
	map_counts_table["FAF"] = 0;
	map_counts_table["ST"] = 0;
	map_counts_table["QE"] = 0;
	map_counts_table["RP"] = 0;
	map_counts_table["APP"] = 0;
	map_counts_table["OAR"] = 0;
	map_counts_table["PU"] = 0;
	map_counts_table["BP"] = 0;
	map_counts_table["WGH"] = 0;
	map_counts_table["OC"] = 0;
	map_counts_table["UC"] = 0;

	std::ifstream count_file;

	char kc_path[256]; kc_path[0] = '\0';
	char valve_path[256]; valve_path[0] = '\0';
	char valve_won_path[256]; valve_won_path[0] = '\0';
	char *counts_filepath = (char *)CVAR_GET_STRING( "kc_counts_file" );

	strncat(kc_path, "killcount/", 256); strncat(kc_path, counts_filepath, 256);
	strncat(valve_path, "valve/", 256); strncat(valve_path, counts_filepath, 256);
	strncat(valve_won_path, "valve_WON/", 256); strncat(valve_won_path, counts_filepath, 256);

	count_file.open(counts_filepath);
	if (count_file.fail()) {
		count_file.close();
		count_file.clear();
		count_file.open(kc_path);
	} 
	if (count_file.fail()) {
		count_file.close();
		count_file.clear();
		count_file.open(valve_won_path);
	} 
	if (count_file.fail()) {
		count_file.close();
		count_file.clear();
		count_file.open(valve_path);
	} 
	if (count_file.fail()) {
		count_file.close();
		return;
	}

	count_file >> map_counts_table["UC"];
	count_file >> map_counts_table["OC"];
	count_file >> map_counts_table["WGH"];
	count_file >> map_counts_table["BP"];
	count_file >> map_counts_table["PU"];
	count_file >> map_counts_table["OAR"];
	count_file >> map_counts_table["APP"];
	count_file >> map_counts_table["RP"];
	count_file >> map_counts_table["QE"];
	count_file >> map_counts_table["ST"];
	count_file >> map_counts_table["FAF"];
	count_file >> map_counts_table["LC"];
	count_file >> map_counts_table["XEN"];
	count_file >> map_counts_table["GL"];
	count_file >> map_counts_table["INT"];
	count_file >> map_counts_table["END"];

	count_file.close();
}

const char *get_chapter_from_map_name(const char* map_name)
{
	// This WAS a map and then I thought something wasn't working but I was wrong but I already changed it :-)))) That's what I get for not using version control
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
		return "UC";
	}

	else if (strcmp("c1a2", map_name) == 0
		|| strcmp("c1a2a", map_name) == 0
		|| strcmp("c1a2b", map_name) == 0
		|| strcmp("c1a2c", map_name) == 0
		|| strcmp("c1a2d", map_name) == 0)
	{
		return "OC";
	}

	else if (strcmp("c1a3", map_name) == 0
		|| strcmp("c1a3a", map_name) == 0
		|| strcmp("c1a3b", map_name) == 0
		|| strcmp("c1a3c", map_name) == 0
		|| strcmp("c1a3d", map_name) == 0)
	{
		return "WGH";
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
		return "BP";
	}

	else if (strcmp("c2a1", map_name) == 0
		|| strcmp("c2a1a", map_name) == 0
		|| strcmp("c2a1b", map_name) == 0)
	{
		return "PU";
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
		return "OAR";
	}

	else if (strcmp("c2a3", map_name) == 0
		|| strcmp("c2a3a", map_name) == 0
		|| strcmp("c2a3b", map_name) == 0
		|| strcmp("c2a3c", map_name) == 0
		|| strcmp("c2a3d", map_name) == 0
		|| strcmp("c2a3e", map_name) == 0)
	{
		return "APP";
	}

	else if (strcmp("c2a4", map_name) == 0
		|| strcmp("c2a4a", map_name) == 0
		|| strcmp("c2a4b", map_name) == 0
		|| strcmp("c2a4c", map_name) == 0)
	{
		return "RP";
	}

	else if (strcmp("c2a4d", map_name) == 0
		|| strcmp("c2a4e", map_name) == 0
		|| strcmp("c2a4f", map_name) == 0
		|| strcmp("c2a4g", map_name) == 0)
	{
		return "QE";
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
		return "ST";
	}

	else if (strcmp("c3a1", map_name) == 0
		|| strcmp("c3a1a", map_name) == 0
		|| strcmp("c3a1b", map_name) == 0)
	{
		return "FAF";
	}

	else if (strcmp("c3a2e", map_name) == 0
		|| strcmp("c3a2", map_name) == 0
		|| strcmp("c3a2a", map_name) == 0
		|| strcmp("c3a2b", map_name) == 0
		|| strcmp("c3a2c", map_name) == 0
		|| strcmp("c3a2d", map_name) == 0
		|| strcmp("c3a2f", map_name) == 0)
	{
		return "LC";
	}

	else if (strcmp("c4a1", map_name) == 0)
	{
		return "XEN";
	}

	else if (strcmp("c4a1", map_name) == 0
		|| strcmp("c4a2", map_name) == 0
		|| strcmp("c4a2a", map_name) == 0
		|| strcmp("c4a2b", map_name) == 0)
	{
		return "GL";
	}

	else if (strcmp("c4a1a", map_name) == 0
		|| strcmp("c4a1b", map_name) == 0
		|| strcmp("c4a1c", map_name) == 0
		|| strcmp("c4a1d", map_name) == 0
		|| strcmp("c4a1e", map_name) == 0
		|| strcmp("c4a1f", map_name) == 0)
	{
		return "INT";
	}

	else
	{
		return "END";
	}
}

// Dummy command just used as a marker in the demo file
DECLARE_COMMAND(m_Health, ReportToDemo);
void CHudHealth::UserCmd_ReportToDemo(void) { }

int CHudHealth::Init(void)
{
	HOOK_MESSAGE(Health);
	HOOK_MESSAGE(Damage);
	HOOK_MESSAGE(Kills);
	m_iHealth = 100;
	m_fFade = 0;
	m_iFlags = 0;
	m_bitsDamage = 0;
	m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 0;
	giDmgHeight = 0;
	giDmgWidth = 0;

	memset(m_dmg, 0, sizeof(DAMAGE_IMAGE) * NUM_DMG_TYPES);

	CVAR_CREATE( "kc_counts_file", "counts.txt", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_ghosts", "1", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_hit_marker", "1", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_count_friendlies", "0", FCVAR_ARCHIVE ); 
	CVAR_CREATE( "kc_auto_reset_on_c1a0", "1", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_show_kill_table", "1", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_show_all_chapters", "1", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_show_current_map", "0", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_max_percent", "0", FCVAR_ARCHIVE ); // Eliminates a few particularly annoying/unintended kills from the run
	CVAR_CREATE( "kc_kill_table_x", "20", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_kill_table_y", "220", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_show_crosshair_counter", "0", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_crosshair_counter_offset_x", "0", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_crosshair_counter_offset_y", "0", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_map_x", "20", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_map_y", "200", FCVAR_ARCHIVE );
	CVAR_CREATE( "kc_debug_monster_path_search", "0", FCVAR_ARCHIVE);
	CVAR_CREATE( "kc_debug_nihilanth_attacks", "0", FCVAR_ARCHIVE);

	HOOK_COMMAND("report_to_demo", ReportToDemo);

	//init_map_kills();
	//load_count_file();

	gHUD.AddHudElem(this);
	return 1;
}

void CHudHealth::Reset( void )
{
	// make sure the pain compass is cleared when the player respawns
	m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 0;


	// force all the flashing damage icons to expire
	m_bitsDamage = 0;
	for ( int i = 0; i < NUM_DMG_TYPES; i++ )
	{
		m_dmg[i].fExpire = 0;
	}
}

int CHudHealth::VidInit(void)
{
	m_hSprite = 0;

	m_HUD_dmg_bio = gHUD.GetSpriteIndex( "dmg_bio" ) + 1;
	m_HUD_cross = gHUD.GetSpriteIndex( "cross" );

	giDmgHeight = gHUD.GetSpriteRect(m_HUD_dmg_bio).right - gHUD.GetSpriteRect(m_HUD_dmg_bio).left;
	giDmgWidth = gHUD.GetSpriteRect(m_HUD_dmg_bio).bottom - gHUD.GetSpriteRect(m_HUD_dmg_bio).top;

	// Need to do this here so kc_counts_file has a chance to apply
	init_map_kills();
	load_count_file();

	return 1;
}

int CHudHealth:: MsgFunc_Health(const char *pszName,  int iSize, void *pbuf )
{
	// TODO: update local health data
	BEGIN_READ( pbuf, iSize );
	int x = READ_BYTE();

	m_iFlags |= HUD_ACTIVE;

	// Only update the fade if we've changed health
	if (x != m_iHealth)
	{
		m_fFade = FADE_TIME;
		m_iHealth = x;
	}

	return 1;
}

int CHudHealth:: MsgFunc_Kills(const char *pszName,  int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	m_iKills = (int) READ_SHORT(); // 2
	map_kills_table["UC"] = (int) READ_BYTE();
	map_kills_table["OC"] = (int) READ_BYTE();
	map_kills_table["WGH"] = (int) READ_BYTE();
	map_kills_table["BP"] = (int) READ_BYTE();
	map_kills_table["PU"] = (int) READ_BYTE();
	map_kills_table["OAR"] = (int) READ_BYTE();
	map_kills_table["APP"] = (int) READ_BYTE();
	map_kills_table["RP"] = (int) READ_BYTE();
	map_kills_table["QE"] = (int) READ_BYTE();
	map_kills_table["ST"] = (int) READ_BYTE();
	map_kills_table["FAF"] = (int) READ_BYTE();
	map_kills_table["LC"] = (int) READ_BYTE();
	map_kills_table["XEN"] = (int) READ_BYTE();
	map_kills_table["GL"] = (int) READ_BYTE();
	map_kills_table["INT"] = (int) READ_BYTE();
	map_kills_table["END"] = (int) READ_BYTE(); // 18

	return 1;
}


int CHudHealth:: MsgFunc_Damage(const char *pszName,  int iSize, void *pbuf )
{
	BEGIN_READ( pbuf, iSize );

	int armor = READ_BYTE();	// armor
	int damageTaken = READ_BYTE();	// health
	long bitsDamage = READ_LONG(); // damage bits

	vec3_t vecFrom;

	for ( int i = 0 ; i < 3 ; i++)
		vecFrom[i] = READ_COORD();

	UpdateTiles(gHUD.m_flTime, bitsDamage);

	// Actually took damage?
	if ( damageTaken > 0 || armor > 0 )
		CalcDamageDirection(vecFrom);

	return 1;
}


// Returns back a color from the
// Green <-> Yellow <-> Red ramp
void CHudHealth::GetPainColor( int &r, int &g, int &b )
{
	int iHealth = m_iHealth;

	if (iHealth > 25)
		iHealth -= 25;
	else if ( iHealth < 0 )
		iHealth = 0;
#if 0
	g = iHealth * 255 / 100;
	r = 255 - g;
	b = 0;
#else
	if (m_iHealth > 25)
	{
		UnpackRGB(r,g,b, RGB_YELLOWISH);
	}
	else
	{
		r = 250;
		g = 0;
		b = 0;
	}
#endif 
}

int CHudHealth::Draw(float flTime)
{
	int r, g, b;
	int a = 0, x, y;
	int HealthWidth;

//	if (m_iHealth <= 0)
//		return 1;

	if ( gHUD.m_iHideHUDDisplay & HIDEHUD_HEALTH )
		return 1;

	if ( !m_hSprite )
		m_hSprite = LoadSprite(PAIN_NAME);
	
	// Has health changed? Flash the health #
	if (m_fFade)
	{
		m_fFade -= (gHUD.m_flTimeDelta * 20);
		if (m_fFade <= 0)
		{
			a = MIN_ALPHA;
			m_fFade = 0;
		}

		// Fade the health number back to dim

		a = MIN_ALPHA +  (m_fFade/FADE_TIME) * 128;

	}
	else
		a = MIN_ALPHA;

	// If health is getting low, make it bright red
	if (m_iHealth <= 15)
		a = 255;
		
	GetPainColor( r, g, b );
	ScaleColors(r, g, b, a );

	// Only draw health if we have the suit.
	if (gHUD.m_iWeaponBits & (1<<(WEAPON_SUIT)))
	{
		HealthWidth = gHUD.GetSpriteRect(gHUD.m_HUD_number_0).right - gHUD.GetSpriteRect(gHUD.m_HUD_number_0).left;
		int CrossWidth = gHUD.GetSpriteRect(m_HUD_cross).right - gHUD.GetSpriteRect(m_HUD_cross).left;

		y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight / 2;
		x = CrossWidth /2;

		SPR_Set(gHUD.GetSprite(m_HUD_cross), r, g, b);
		SPR_DrawAdditive(0, x, y, &gHUD.GetSpriteRect(m_HUD_cross));

		x = CrossWidth + HealthWidth / 2;

		x = gHUD.DrawHudNumber(x, y, DHN_3DIGITS | DHN_DRAWZERO, m_iHealth, r, g, b);

		x += HealthWidth/2;

		int iHeight = gHUD.m_iFontHeight;
		int iWidth = HealthWidth/10;
		FillRGBA(x, y, iWidth, iHeight, 255, 160, 0, a);

		r = 255;
		g = 160;
		b = 0;
		a = MIN_ALPHA;
		ScaleColors(r, g, b, a );

		// Draw kill counter
		x = CrossWidth /2;
		y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight * 2;

		// Draw kill counter
		x = CrossWidth + HealthWidth / 2;
		y = ScreenHeight - gHUD.m_iFontHeight - gHUD.m_iFontHeight * 2;
		x = gHUD.DrawHudNumber(x, y, DHN_3DIGITS | DHN_DRAWZERO, m_iKills, r, g, b); // 255, 160, 0
		gHUD.DrawHudString( x+9, y+8, 320, "Kills", r, g, b );

		// Kill counter stuff
		bool draw_kill_table = CVAR_GET_FLOAT( "kc_show_kill_table" ) != 0;
		bool show_all_chapters = CVAR_GET_FLOAT( "kc_show_all_chapters" ) != 0;
		bool show_current_map = CVAR_GET_FLOAT( "kc_show_current_map" ) != 0;
		bool draw_crosshair_counter = CVAR_GET_FLOAT( "kc_show_crosshair_counter" ) != 0;

		char map_name [64];
		memset(map_name, '\0', 64);
		strncpy(map_name, gEngfuncs.pfnGetLevelName()+5, strlen(gEngfuncs.pfnGetLevelName()+5)-4);
		const char *chapter = get_chapter_from_map_name(map_name);

		if (draw_crosshair_counter)
		{
			int x_offset = (int) CVAR_GET_FLOAT( "kc_crosshair_counter_offset_x" );
			int y_offset = (int) CVAR_GET_FLOAT( "kc_crosshair_counter_offset_y" );

			if (map_kills_table[chapter] >= map_counts_table[chapter]) {
				r = 0;
				g = 255;
				b = 0;
				a = MIN_ALPHA;
				ScaleColors(r, g, b, a );
			}

			char str_buff [20];
			char num_buff [10];
			memset(str_buff, '\0', 20);
			memset(num_buff, '\0', 10);
			itoa(map_counts_table[chapter], num_buff, 10);
			strncat(str_buff, "/ ", 5);
			strncat(str_buff, num_buff, 5);

			// Draw crosshair kill counter
			x = (ScreenWidth / 2) + x_offset;
			y = (ScreenHeight / 2) + y_offset;
			x = gHUD.DrawHudNumber(x, y, DHN_3DIGITS | DHN_DRAWZERO, map_kills_table[chapter], r, g, b);
			gHUD.DrawHudString( x+5, y+5, ScreenWidth, str_buff, r, g, b );
		}

		if (show_current_map && strncmp(map_name, "c5a1", 64) != 0) {
			x = (int) CVAR_GET_FLOAT( "kc_map_x" );
			y = (int) CVAR_GET_FLOAT( "kc_map_y" );

			r = 255;
			g = 160;
			b = 0;
			a = MIN_ALPHA;
			ScaleColors(r, g, b, a );

			gHUD.DrawHudString( x, y, ScreenWidth, map_name, r, g, b );
		}

		if (draw_kill_table || strncmp(map_name, "c5a1", 64) == 0) {
			x = (int) CVAR_GET_FLOAT( "kc_kill_table_x" );
			y = (int) CVAR_GET_FLOAT( "kc_kill_table_y" );
			char num_buff [20];
			char count_buff [20];
			char str_buff [20];

			for (std::map<const char*, int>::iterator it = map_kills_table.begin(); it != map_kills_table.end(); ++it)
			{
				r = 255;
				g = 160;
				b = 0;
				a = MIN_ALPHA;
				ScaleColors(r, g, b, a );

				itoa(it->second, num_buff, 10);
				itoa(map_counts_table[it->first], count_buff, 10);
				strcpy(str_buff, it->first);

				if (it->second >= map_counts_table[it->first]) {
					r = 0;
					g = 255;
					b = 0;
					a = MIN_ALPHA;
					ScaleColors(r, g, b, a );
				}

				if ((!show_all_chapters && strncmp(map_name, "c5a1", 64) != 0) && strcmp(chapter, it->first) != 0) continue; // I undid all the maps and did a bunch of ugly if/else chains because find() wasn't working here but it turns out the outer if statement block was just using the wrong cvar :-))))) FML. That's what I get for 4am coding.

				gHUD.DrawHudString( x, y, ScreenWidth, str_buff, r, g, b );
				gHUD.DrawHudString( x+50, y, ScreenWidth, num_buff, r, g, b );
				gHUD.DrawHudString( x+80, y, ScreenWidth, " / ", r, g, b );
				gHUD.DrawHudString( x+100, y, ScreenWidth, count_buff, r, g, b );
				y += 15;
			}
		}
	}

	DrawDamage(flTime);
	return DrawPain(flTime);
}

void CHudHealth::CalcDamageDirection(vec3_t vecFrom)
{
	vec3_t	forward, right, up;
	float	side, front;
	vec3_t vecOrigin, vecAngles;

	if (!vecFrom[0] && !vecFrom[1] && !vecFrom[2])
	{
		m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 0;
		return;
	}


	memcpy(vecOrigin, gHUD.m_vecOrigin, sizeof(vec3_t));
	memcpy(vecAngles, gHUD.m_vecAngles, sizeof(vec3_t));


	VectorSubtract (vecFrom, vecOrigin, vecFrom);

	float flDistToTarget = vecFrom.Length();

	vecFrom = vecFrom.Normalize();
	AngleVectors (vecAngles, forward, right, up);

	front = DotProduct (vecFrom, right);
	side = DotProduct (vecFrom, forward);

	if (flDistToTarget <= 50)
	{
		m_fAttackFront = m_fAttackRear = m_fAttackRight = m_fAttackLeft = 1;
	}
	else 
	{
		if (side > 0)
		{
			if (side > 0.3)
				m_fAttackFront = max(m_fAttackFront, side);
		}
		else
		{
			float f = fabs(side);
			if (f > 0.3)
				m_fAttackRear = max(m_fAttackRear, f);
		}

		if (front > 0)
		{
			if (front > 0.3)
				m_fAttackRight = max(m_fAttackRight, front);
		}
		else
		{
			float f = fabs(front);
			if (f > 0.3)
				m_fAttackLeft = max(m_fAttackLeft, f);
		}
	}
}

int CHudHealth::DrawPain(float flTime)
{
	if (!(m_fAttackFront || m_fAttackRear || m_fAttackLeft || m_fAttackRight))
		return 1;

	int r, g, b;
	int x, y, a, shade;

	// TODO:  get the shift value of the health
	a = 255;	// max brightness until then

	float fFade = gHUD.m_flTimeDelta * 2;
	
	// SPR_Draw top
	if (m_fAttackFront > 0.4)
	{
		GetPainColor(r,g,b);
		shade = a * max( m_fAttackFront, 0.5 );
		ScaleColors(r, g, b, shade);
		SPR_Set(m_hSprite, r, g, b );

		x = ScreenWidth/2 - SPR_Width(m_hSprite, 0)/2;
		y = ScreenHeight/2 - SPR_Height(m_hSprite,0) * 3;
		SPR_DrawAdditive(0, x, y, NULL);
		m_fAttackFront = max( 0, m_fAttackFront - fFade );
	} else
		m_fAttackFront = 0;

	if (m_fAttackRight > 0.4)
	{
		GetPainColor(r,g,b);
		shade = a * max( m_fAttackRight, 0.5 );
		ScaleColors(r, g, b, shade);
		SPR_Set(m_hSprite, r, g, b );

		x = ScreenWidth/2 + SPR_Width(m_hSprite, 1) * 2;
		y = ScreenHeight/2 - SPR_Height(m_hSprite,1)/2;
		SPR_DrawAdditive(1, x, y, NULL);
		m_fAttackRight = max( 0, m_fAttackRight - fFade );
	} else
		m_fAttackRight = 0;

	if (m_fAttackRear > 0.4)
	{
		GetPainColor(r,g,b);
		shade = a * max( m_fAttackRear, 0.5 );
		ScaleColors(r, g, b, shade);
		SPR_Set(m_hSprite, r, g, b );

		x = ScreenWidth/2 - SPR_Width(m_hSprite, 2)/2;
		y = ScreenHeight/2 + SPR_Height(m_hSprite,2) * 2;
		SPR_DrawAdditive(2, x, y, NULL);
		m_fAttackRear = max( 0, m_fAttackRear - fFade );
	} else
		m_fAttackRear = 0;

	if (m_fAttackLeft > 0.4)
	{
		GetPainColor(r,g,b);
		shade = a * max( m_fAttackLeft, 0.5 );
		ScaleColors(r, g, b, shade);
		SPR_Set(m_hSprite, r, g, b );

		x = ScreenWidth/2 - SPR_Width(m_hSprite, 3) * 3;
		y = ScreenHeight/2 - SPR_Height(m_hSprite,3)/2;
		SPR_DrawAdditive(3, x, y, NULL);

		m_fAttackLeft = max( 0, m_fAttackLeft - fFade );
	} else
		m_fAttackLeft = 0;

	return 1;
}

int CHudHealth::DrawDamage(float flTime)
{
	int r, g, b, a;
	DAMAGE_IMAGE *pdmg;

	if (!m_bitsDamage)
		return 1;

	UnpackRGB(r,g,b, RGB_YELLOWISH);
	
	a = (int)( fabs(sin(flTime*2)) * 256.0);

	ScaleColors(r, g, b, a);

	// Draw all the items
	for (int i = 0; i < NUM_DMG_TYPES; i++)
	{
		if (m_bitsDamage & giDmgFlags[i])
		{
			pdmg = &m_dmg[i];
			SPR_Set(gHUD.GetSprite(m_HUD_dmg_bio + i), r, g, b );
			SPR_DrawAdditive(0, pdmg->x, pdmg->y, &gHUD.GetSpriteRect(m_HUD_dmg_bio + i));
		}
	}


	// check for bits that should be expired
	for ( i = 0; i < NUM_DMG_TYPES; i++ )
	{
		DAMAGE_IMAGE *pdmg = &m_dmg[i];

		if ( m_bitsDamage & giDmgFlags[i] )
		{
			pdmg->fExpire = min( flTime + DMG_IMAGE_LIFE, pdmg->fExpire );

			if ( pdmg->fExpire <= flTime		// when the time has expired
				&& a < 40 )						// and the flash is at the low point of the cycle
			{
				pdmg->fExpire = 0;

				int y = pdmg->y;
				pdmg->x = pdmg->y = 0;

				// move everyone above down
				for (int j = 0; j < NUM_DMG_TYPES; j++)
				{
					pdmg = &m_dmg[j];
					if ((pdmg->y) && (pdmg->y < y))
						pdmg->y += giDmgHeight;

				}

				m_bitsDamage &= ~giDmgFlags[i];  // clear the bits
			}
		}
	}

	return 1;
}
 

void CHudHealth::UpdateTiles(float flTime, long bitsDamage)
{	
	DAMAGE_IMAGE *pdmg;

	// Which types are new?
	long bitsOn = ~m_bitsDamage & bitsDamage;
	
	for (int i = 0; i < NUM_DMG_TYPES; i++)
	{
		pdmg = &m_dmg[i];

		// Is this one already on?
		if (m_bitsDamage & giDmgFlags[i])
		{
			pdmg->fExpire = flTime + DMG_IMAGE_LIFE; // extend the duration
			if (!pdmg->fBaseline)
				pdmg->fBaseline = flTime;
		}

		// Are we just turning it on?
		if (bitsOn & giDmgFlags[i])
		{
			// put this one at the bottom
			pdmg->x = giDmgWidth/8;
			pdmg->y = ScreenHeight - giDmgHeight * 2;
			pdmg->fExpire=flTime + DMG_IMAGE_LIFE;
			
			// move everyone else up
			for (int j = 0; j < NUM_DMG_TYPES; j++)
			{
				if (j == i)
					continue;

				pdmg = &m_dmg[j];
				if (pdmg->y)
					pdmg->y -= giDmgHeight;

			}
			pdmg = &m_dmg[i];
		}	
	}	

	// damage bits are only turned on here;  they are turned off when the draw time has expired (in DrawDamage())
	m_bitsDamage |= bitsDamage;
}
