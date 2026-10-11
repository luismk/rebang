// Original: d:\build\custom\temp\code\client\projectg\inifile.h
#ifndef PG_CLIENT_PROJECTG_INIFILE_H
#define PG_CLIENT_PROJECTG_INIFILE_H

// Returned by the integer accessors when the file or the key is missing.
#define INI_ERROR (-12302323)

// Reads and writes an .ini file of the execute directory (Win32 private
// profile API). The path lives in a file-static string of inifile.cpp.
class CIniFile {
public:
	enum IO_MODE {
		IO_READ = 0,    // must exist in the resource packs
		IO_WRITE = 1,   // created by an empty write
	};

	CIniFile(void);
	CIniFile(const char* filename);
	~CIniFile(void);

	bool Create(const char* filename, IO_MODE mode);
	const char* ReadString(const char* section, const char* key);
	int ReadInteger(const char* section, const char* key);
	int WriteString(const char* section, const char* key, const char* value);
	int WriteInteger(const char* section, const char* key, int value);
};

#endif // PG_CLIENT_PROJECTG_INIFILE_H
