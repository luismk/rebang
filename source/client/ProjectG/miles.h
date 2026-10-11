// Original: d:\build\custom\temp\code\client\projectg\miles.h
#ifndef PG_CLIENT_PROJECTG_MILES_H
#define PG_CLIENT_PROJECTG_MILES_H
#include <windows.h>
#include "..\..\defines.h"
#include "..\..\padding.h"
#include "wlist.h"

// Miles Sound System handles (mss.h: HDIGDRIVER, HSAMPLE, HSTREAM).
struct _DIG_DRIVER;
struct _SAMPLE;
struct _STREAM;
class cFile;

// The game's Miles Sound System wrapper (mss32.dll).
class WMilesSoundSystem {
	LAYOUT_CHECK_FRIEND;
public:
	// Speaker configurations, in the order of sOption::aGetSpeakerText
	// (WMilesSoundSystem::SpeakerType switches on 0..5).
	enum w_speaker_type {
		w_speaker_2ch = 0,       // 2 channel speakers
		w_speaker_headphone = 1, // headphones
		w_speaker_surround = 2,  // surround
		w_speaker_4ch = 3,       // 4 channel speakers
		w_speaker_51ch = 4,      // 5.1 channel speakers
		w_speaker_71ch = 5       // 7.1 channel speakers
	};

	// A loaded 2D sample file, shared by name (LoadSample2D).
	struct msample2d {
		void *m_data;     // 0x0, the file image (AIL_file_read)
		int m_size;       // 0x4, AIL_file_size
		int m_refCount;   // 0x8
		int m_idleFrames; // 0xC, ClearIdleSound calls while unused; cleared by Release2D
		int m_lastSound;  // 0x10, the sound handle that played it last (Play)
		char m_name[4];   // 0x14, allocated to the name's length
	};

	// A loaded 3D sample file, shared by name (LoadSample3D).
	struct msample3d {
		void *m_data;     // 0x0, the file image (AIL_file_read)
		int m_refCount;   // 0x4
		int m_idleFrames; // 0x8, ClearIdleSound calls while unused; cleared by Release3D
		int m_lastSound;  // 0xC, the sound handle that played it last (Play)
		char m_name[4];   // 0x10, the file name (AIL_file_size)
	};

	// One sound handle (Load returns its index).
	struct msound {
		_SAMPLE *m_sample2d;   // 0x0
		msample2d *m_data2d;   // 0x4, released by DestroySoundBuffer
		_SAMPLE *m_sample3d;   // 0x8
		msample3d *m_data3d;   // 0xC, released by DestroySoundBuffer
		_STREAM *m_stream;     // 0x10, a streamed sound (m_state 2)
		int m_streamPosition;  // 0x14, where Resume restarts the stream
		float m_volume;        // 0x18
		float m_x;             // 0x1C, SetPosition
		float m_y;             // 0x20
		float m_z;             // 0x24
		float m_near;          // 0x28, SetNearFar
		float m_far;           // 0x2C, SetNearFar
		int m_state;           // 0x30, 0 = 2D sample, 1 = 3D sample, 2 = stream, 3 = free (GetEmptyHandle)
	};

	WMilesSoundSystem(HWND hwnd, const char *redistDirectory, bool flag, int a, int b, int c);
	virtual ~WMilesSoundSystem();

	void SetReverb(void);
	void ClearAllStreams(void);
	void ClearAllSamples(void);
	void ClearAllSounds(void);
	int GetCPUpercent(void);
	void SpeakerType(w_speaker_type type);
	void SetBalance(int balance);
	void Activate(bool active);
	void Volume(float volume);
	void Resume(int handle, int flag);
	void Stop(int handle);
	void SetPitch(int handle, int rate);
	int GetPitch(int handle);
	void SetVolume(int handle, float volume);
	void SetNearFar(int handle, float nearDist, float farDist);
	void SetPosition(int handle, float x, float y, float z);
	bool IsPlaying(int handle);
	int GetPosition(int handle);
	int GetLength(int handle);
	static void Process(void);
	void ClearIdleSample(void);
	void ClearIdleSound(int frames);
	bool Init(HWND hwnd, int a, int b);
	void DestroySoundBuffer(int handle);
	void Play(int handle, int loop, bool stopPrevious);
	int Load(char *name, int type);

	// miles.h:58
	void SetDistance(float nearDist, float farDist) {
		m_nearDistance = nearDist;
		m_farDistance = farDist;
	}

protected:
	bool InitMixer(void);
	void SetMixer(float volume);
	void CloseMixer(void);
	void Release2D(msample2d *sample);
	static void LoadSample3D(msample3d *sample);
	void Release3D(msample3d *sample);
	int GetEmptyHandle(void);
	_SAMPLE *GetAllocateSample(int handle, bool flag);
	_SAMPLE *GetAllocateSample3D(int handle, bool flag);
	msample2d *LoadSample2D(char *name);
	msample3d *LoadSample3D(char *name);

	static unsigned int __stdcall Open_callback(const char *name, unsigned int *handle);
	static void __stdcall Close_callback(unsigned int handle);
	static unsigned int __stdcall Read_callback(unsigned int handle, void *buffer, unsigned int bytes);
	static int __stdcall Seek_callback(unsigned int handle, int offset, unsigned int type);

	static int g_offset[16];      // pending seek of each open file, -1 for none
	static cFile *g_files[16];    // the files opened by Open_callback

	float m_masterVolume;           // 0x4, 1.0f after the constructor
	float m_nearDistance;           // 0x8, SetDistance
	float m_farDistance;            // 0xC, SetDistance
	float m_leftVolume;             // 0x10, SetBalance
	float m_rightVolume;            // 0x14, SetBalance
	bool m_invertX;                 // 0x18, SetPosition negates x
	bool m_option19;                // 0x19, false after the constructor
	bool m_multiPlay;               // 0x1A, Play lets a sample overlap its previous play
	UNKNOWN(0x1B, 0x1C);
	_DIG_DRIVER *m_dig;             // 0x1C
	int m_reserved20;               // 0x20, 0 after the constructor
	WList<msample2d *> m_samples2d; // 0x24
	_SAMPLE *m_handles2d[16];       // 0x44, the 2D sample handles (GetAllocateSample)
	int m_handleSounds2d[16];       // 0x84, the sound handle using each 2D sample handle
	bool m_handleFlags2d[16];       // 0xC4, GetAllocateSample's flag of each 2D sample handle
	WList<msample3d *> m_samples3d; // 0xD4
	_SAMPLE *m_handles3d[16];       // 0xF4, the 3D sample handles (GetAllocateSample3D)
	int m_handleSounds3d[16];       // 0x134, the sound handle using each 3D sample handle
	bool m_handleFlags3d[16];       // 0x174, GetAllocateSample3D's flag of each 3D sample handle
	_STREAM *m_streams[8];          // 0x184
	int m_streamHandles[8];         // 0x1A4, the sound handle of each stream
	msound m_sounds[1024];          // 0x1C4
	int m_lastHandle;               // 0xD1C4, GetEmptyHandle's cursor
	HMIXER m_mixer;                 // 0xD1C8
	MIXERCONTROL m_mixerControl;    // 0xD1CC, the volume control (InitMixer)
	MIXERLINE m_mixerLine;          // 0xD260, the speakers line (InitMixer)
};
CHECK_SIZE(WMilesSoundSystem::msample2d, 0x18);
CHECK_SIZE(WMilesSoundSystem::msound, 0x34);
CHECK_SIZE(WMilesSoundSystem, 0xD308);
BEGIN_LAYOUT_CHECK(WMilesSoundSystem)
	CHECK_FIELD(WMilesSoundSystem::msample2d, m_refCount, 0x8);
	CHECK_FIELD(WMilesSoundSystem::msample2d, m_idleFrames, 0xC);
	CHECK_FIELD(WMilesSoundSystem::msample2d, m_lastSound, 0x10);
	CHECK_FIELD(WMilesSoundSystem::msample2d, m_name, 0x14);
	CHECK_FIELD(WMilesSoundSystem::msample3d, m_refCount, 0x4);
	CHECK_FIELD(WMilesSoundSystem::msample3d, m_idleFrames, 0x8);
	CHECK_FIELD(WMilesSoundSystem::msample3d, m_lastSound, 0xC);
	CHECK_FIELD(WMilesSoundSystem::msample3d, m_name, 0x10);
	CHECK_FIELD(WMilesSoundSystem::msound, m_data2d, 0x4);
	CHECK_FIELD(WMilesSoundSystem::msound, m_sample3d, 0x8);
	CHECK_FIELD(WMilesSoundSystem::msound, m_data3d, 0xC);
	CHECK_FIELD(WMilesSoundSystem::msound, m_stream, 0x10);
	CHECK_FIELD(WMilesSoundSystem::msound, m_streamPosition, 0x14);
	CHECK_FIELD(WMilesSoundSystem::msound, m_x, 0x1C);
	CHECK_FIELD(WMilesSoundSystem::msound, m_near, 0x28);
	CHECK_FIELD(WMilesSoundSystem::msound, m_state, 0x30);
	CHECK_FIELD(WMilesSoundSystem::msound, m_far, 0x2C);
	CHECK_FIELD(WMilesSoundSystem, m_masterVolume, 0x4);
	CHECK_FIELD(WMilesSoundSystem, m_nearDistance, 0x8);
	CHECK_FIELD(WMilesSoundSystem, m_farDistance, 0xC);
	CHECK_FIELD(WMilesSoundSystem, m_leftVolume, 0x10);
	CHECK_FIELD(WMilesSoundSystem, m_rightVolume, 0x14);
	CHECK_FIELD(WMilesSoundSystem, m_invertX, 0x18);
	CHECK_FIELD(WMilesSoundSystem, m_option19, 0x19);
	CHECK_FIELD(WMilesSoundSystem, m_multiPlay, 0x1A);
	CHECK_FIELD(WMilesSoundSystem, m_reserved20, 0x20);
	CHECK_FIELD(WMilesSoundSystem, m_dig, 0x1C);
	CHECK_FIELD(WMilesSoundSystem, m_samples2d, 0x24);
	CHECK_FIELD(WMilesSoundSystem, m_handles2d, 0x44);
	CHECK_FIELD(WMilesSoundSystem, m_handleSounds2d, 0x84);
	CHECK_FIELD(WMilesSoundSystem, m_handleFlags2d, 0xC4);
	CHECK_FIELD(WMilesSoundSystem, m_samples3d, 0xD4);
	CHECK_FIELD(WMilesSoundSystem, m_handles3d, 0xF4);
	CHECK_FIELD(WMilesSoundSystem, m_handleSounds3d, 0x134);
	CHECK_FIELD(WMilesSoundSystem, m_handleFlags3d, 0x174);
	CHECK_FIELD(WMilesSoundSystem, m_streams, 0x184);
	CHECK_FIELD(WMilesSoundSystem, m_streamHandles, 0x1A4);
	CHECK_FIELD(WMilesSoundSystem, m_sounds, 0x1C4);
	CHECK_FIELD(WMilesSoundSystem, m_lastHandle, 0xD1C4);
	CHECK_FIELD(WMilesSoundSystem, m_mixer, 0xD1C8);
	CHECK_FIELD(WMilesSoundSystem, m_mixerControl, 0xD1CC);
	CHECK_FIELD(WMilesSoundSystem, m_mixerLine, 0xD260);
END_LAYOUT_CHECK

#endif // PG_CLIENT_PROJECTG_MILES_H
