// Original: d:\build\custom\temp\code\client\projectg\guilddefine.h
#ifndef PG_CLIENT_PROJECTG_GUILDDEFINE_H
#define PG_CLIENT_PROJECTG_GUILDDEFINE_H

// Guild definitions shared by the guild dialogs; only the date line format
// is known (its dynamic initializer, sub_009e1620 in IngameGuildMyRoomDlg,
// docs/tough_problems/010). " - <year>Y <month>M <day>D <text>"
static const char* const GUILD_DATE_FORMAT_TEXT =
	" - %d\xb3\xe2 %d\xbf\xf9 %d\xc0\xcf %s\n";

// guilddefine.h:12
static const char* GUILD_DATE_FORMAT = GUILD_DATE_FORMAT_TEXT;

#endif // PG_CLIENT_PROJECTG_GUILDDEFINE_H
