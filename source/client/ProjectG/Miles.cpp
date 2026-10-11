// Original: d:\build\custom\temp\code\client\projectg\miles.cpp
#include <windows.h>
#include <mmsystem.h>
#include <new>
#include <stdio.h>
#include "Miles.h"
#include "westpak.h"
#include "wevent.h"
#include "wresrcmng.h"

// mss.h (Miles Sound System, mss32.dll): a decoded sound file's format.
struct AILSOUNDINFO {
	int format;
	const void *data_ptr;
	unsigned int data_len;
	unsigned int rate;
	int bits;
	int channels;
	unsigned int samples;
	unsigned int block_size;
	const void *initial_ptr;
	unsigned int channel_mask;
};

// mss.h (Miles Sound System, mss32.dll): the imports this unit calls.
extern "C" {
__declspec(dllimport) void __stdcall AIL_set_digital_master_reverb(_DIG_DRIVER *dig, float reverb_decay_time, float reverb_predelay, float reverb_damping);
__declspec(dllimport) int __stdcall AIL_digital_CPU_percent(_DIG_DRIVER *dig);
__declspec(dllimport) void __stdcall AIL_set_sample_playback_rate(_SAMPLE *s, int playback_rate);
__declspec(dllimport) int __stdcall AIL_sample_playback_rate(_SAMPLE *s);
__declspec(dllimport) _DIG_DRIVER *__stdcall AIL_open_digital_driver(unsigned int frequency, int bits, int channel, unsigned int flags);
__declspec(dllimport) char *__stdcall AIL_last_error(void);
__declspec(dllimport) int __stdcall AIL_set_DirectSound_HWND(_DIG_DRIVER *dig, HWND wnd);
__declspec(dllimport) void __stdcall AIL_set_stream_loop_count(_STREAM *stream, int count);
__declspec(dllimport) _SAMPLE *__stdcall AIL_stream_sample_handle(_STREAM *stream);
__declspec(dllimport) void __stdcall AIL_set_sample_volume_pan(_SAMPLE *s, float volume, float pan);
__declspec(dllimport) void __stdcall AIL_set_stream_position(_STREAM *stream, int offset);
__declspec(dllimport) void __stdcall AIL_start_stream(_STREAM *stream);
__declspec(dllimport) void __stdcall AIL_stream_info(_STREAM *stream, int *datarate, int *sndtype, int *length, int *memory);
__declspec(dllimport) void __stdcall AIL_close_stream(_STREAM *stream);
__declspec(dllimport) void __stdcall AIL_set_sample_3D_position(_SAMPLE *s, float x, float y, float z);
__declspec(dllimport) void __stdcall AIL_set_sample_3D_distances(_SAMPLE *s, float max_dist, float min_dist, int auto_3D_wet_atten);
__declspec(dllimport) int __stdcall AIL_file_size(const char *filename);
__declspec(dllimport) void *__stdcall AIL_file_read(const char *filename, void *dest);
__declspec(dllimport) int __stdcall AIL_file_type(void *data, int size);
__declspec(dllimport) int __stdcall AIL_decompress_ASI(const void *indata, unsigned int insize, const char *filename_ext, void **outdata, unsigned int *outsize, void *callback);
__declspec(dllimport) int __stdcall AIL_WAV_info(const void *data, AILSOUNDINFO *info);
__declspec(dllimport) int __stdcall AIL_decompress_ADPCM(const AILSOUNDINFO *info, void **outdata, unsigned int *outsize);
__declspec(dllimport) void __stdcall AIL_mem_free_lock(void *ptr);
__declspec(dllimport) void __stdcall AIL_stop_sample(_SAMPLE *s);
__declspec(dllimport) void __stdcall AIL_release_sample_handle(_SAMPLE *s);
__declspec(dllimport) void *__stdcall AIL_speaker_configuration(_DIG_DRIVER *dig, int *n_physical_channels, int *n_logical_channels, float *falloff_power, int *channel_spec);
__declspec(dllimport) void __stdcall AIL_set_speaker_configuration(_DIG_DRIVER *dig, void *array, int n_channels, float falloff_power);
__declspec(dllimport) void __stdcall AIL_pause_stream(_STREAM *stream, int onoff);
__declspec(dllimport) int __stdcall AIL_stream_position(_STREAM *stream);
__declspec(dllimport) int __stdcall AIL_stream_status(_STREAM *stream);
__declspec(dllimport) unsigned int __stdcall AIL_sample_status(_SAMPLE *s);
__declspec(dllimport) unsigned int __stdcall AIL_sample_position(_SAMPLE *s);
__declspec(dllimport) void __stdcall AIL_stream_ms_position(_STREAM *stream, int *total_milliseconds, int *current_milliseconds);
__declspec(dllimport) _SAMPLE *__stdcall AIL_allocate_sample_handle(_DIG_DRIVER *dig);
__declspec(dllimport) void __stdcall AIL_init_sample(_SAMPLE *s, int format);
__declspec(dllimport) void __stdcall AIL_digital_handle_release(_DIG_DRIVER *dig);
__declspec(dllimport) void __stdcall AIL_close_digital_driver(_DIG_DRIVER *dig);
__declspec(dllimport) void __stdcall AIL_shutdown(void);
__declspec(dllimport) int __stdcall AIL_set_named_sample_file(_SAMPLE *s, const char *file_type_suffix, const void *file_image, unsigned int file_size, int block);
__declspec(dllimport) int __stdcall AIL_set_sample_file(_SAMPLE *s, const void *file_image, int block);
__declspec(dllimport) void __stdcall AIL_set_sample_loop_count(_SAMPLE *s, int loop_count);
__declspec(dllimport) void __stdcall AIL_start_sample(_SAMPLE *s);
__declspec(dllimport) _STREAM *__stdcall AIL_open_stream(_DIG_DRIVER *dig, const char *filename, int stream_mem);
__declspec(dllimport) void __stdcall AIL_set_stream_user_data(_STREAM *stream, unsigned int index, int value);
__declspec(dllimport) char *__stdcall AIL_set_redist_directory(const char *dir);
__declspec(dllimport) void __stdcall AIL_set_file_callbacks(unsigned int (__stdcall *opencb)(const char *, unsigned int *), void (__stdcall *closecb)(unsigned int), int (__stdcall *seekcb)(unsigned int, int, unsigned int), unsigned int (__stdcall *readcb)(unsigned int, void *, unsigned int));
__declspec(dllimport) int __stdcall AIL_startup(void);
}

void __cdecl CloseCFile(cFile *file);

// miles.cpp:34
static WSyncEvent s_syncEvent;

int WMilesSoundSystem::g_offset[16];
cFile *WMilesSoundSystem::g_files[16];

// miles.cpp:42
WMilesSoundSystem::WMilesSoundSystem(HWND hwnd, const char *redistDirectory, bool flag, int a, int b, int c)
	: m_dig(NULL)
	, m_reserved20(0)
	, m_samples2d(16, 16)
	, m_samples3d(16, 16) {
	m_masterVolume = 1.0f;
	m_leftVolume = 1.0f;
	m_rightVolume = 1.0f;
	m_lastHandle = 0;
	m_mixer = NULL;
	m_nearDistance = 0.0f;
	m_farDistance = 100.0f;
	m_invertX = false;
	m_option19 = false;
	m_multiPlay = false;

	int i;
	for (i = 0; i < 1024; i++) {
		m_sounds[i].m_sample2d = NULL;
		m_sounds[i].m_data2d = NULL;
		m_sounds[i].m_sample3d = NULL;
		m_sounds[i].m_data3d = NULL;
		m_sounds[i].m_stream = NULL;
		m_sounds[i].m_streamPosition = 0;
		m_sounds[i].m_state = 3;
	}

	for (i = 0; i < 8; i++) m_streams[i] = NULL;
	for (i = 0; i < 8; i++) m_streamHandles[i] = 0;
	for (i = 0; i < 16; i++) m_handleSounds2d[i] = 0;
	for (i = 0; i < 16; i++) m_handles2d[i] = NULL;
	for (i = 0; i < 16; i++) m_handleSounds3d[i] = 0;
	for (i = 0; i < 16; i++) m_handles3d[i] = NULL;

	if (redistDirectory != NULL) {
		AIL_set_redist_directory(redistDirectory);
	}

	for (i = 0; i < 16; i++) g_files[i] = NULL;

	AIL_set_file_callbacks(Open_callback, Close_callback, Seek_callback, Read_callback);

	if (!AIL_startup()) {
		char text[256];
		// "Cannot initialize the sound: %s"
		sprintf(text, "\xbb\xe7\xbf\xee\xb5\xe5\xb8\xa6 \xc3\xca\xb1\xe2\xc8\xad\xc7\xd2 \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9: %s", AIL_last_error());

		MessageBoxA(NULL, text, "Miles Sound System", MB_OK);
	}
}

// miles.cpp:118
void WMilesSoundSystem::SetReverb(void) {
	AIL_set_digital_master_reverb(m_dig, 1.0f, 0.0f, 0.0f);
}

// miles.cpp:126
bool WMilesSoundSystem::InitMixer(void) {
	UINT numDevs = mixerGetNumDevs();

	m_mixer = NULL;
	if (numDevs != 0) {
		if (mixerOpen(&m_mixer, 0, 0, 0, 0) == MMSYSERR_NOERROR) {
			memset(&m_mixerLine, 0, sizeof(m_mixerLine));
			m_mixerLine.cbStruct = sizeof(MIXERLINE);
			m_mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_SRC_WAVEOUT;
			if (mixerGetLineInfo((HMIXEROBJ)m_mixer, &m_mixerLine, MIXER_GETLINEINFOF_COMPONENTTYPE) == MMSYSERR_NOERROR) {
				MIXERLINECONTROLS controls;
				controls.dwLineID = m_mixerLine.dwLineID;
				controls.pamxctrl = &m_mixerControl;

				memset(&m_mixerControl, 0, sizeof(m_mixerControl));
				m_mixerControl.cbStruct = sizeof(MIXERCONTROL);
				controls.cbStruct = sizeof(MIXERLINECONTROLS);
				controls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
				controls.cControls = 1;
				controls.cbmxctrl = sizeof(MIXERCONTROL);
				if (mixerGetLineControls((HMIXEROBJ)m_mixer, &controls, MIXER_OBJECTF_HMIXER | MIXER_GETLINECONTROLSF_ONEBYTYPE) == MMSYSERR_NOERROR) {
					return true;
				}
			}
		}
	}

	return false;
}

// miles.cpp:170
void WMilesSoundSystem::SetMixer(float volume) {
	MIXERCONTROLDETAILS_UNSIGNED value;
	value.dwValue = (int)((m_mixerControl.Bounds.dwMaximum - 1) * volume);

	MIXERCONTROLDETAILS details;
	details.cbStruct = sizeof(MIXERCONTROLDETAILS);
	details.dwControlID = m_mixerControl.dwControlID;
	details.cChannels = 1;
	details.cMultipleItems = 0;
	details.cbDetails = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
	details.paDetails = &value;
	mixerSetControlDetails((HMIXEROBJ)m_mixer, &details, MIXER_OBJECTF_HMIXER | MIXER_SETCONTROLDETAILSF_VALUE);
}

// miles.cpp:194
void WMilesSoundSystem::CloseMixer(void) {
	if (m_mixer != NULL) {
		mixerClose(m_mixer);
	}
}

// miles.cpp:200
WMilesSoundSystem::~WMilesSoundSystem() {
	CloseMixer();

	ClearAllSamples();
	ClearAllSounds();
	ClearAllStreams();

	for (unsigned int i = 0; i < 16; i++) {
		if (g_files[i] != NULL) {
			CloseCFile(g_files[i]);
			g_files[i] = NULL;
		}
	}

	if (m_dig != NULL) {
		AIL_digital_handle_release(m_dig);
	}
	if (m_dig != NULL) {
		AIL_close_digital_driver(m_dig);
	}
	AIL_shutdown();
}

// miles.cpp:231
void WMilesSoundSystem::ClearAllStreams(void) {
	for (int i = 0; i < 8; i++) {
		if (m_streams[i] != NULL) {
			AIL_close_stream(m_streams[i]);
			m_streams[i] = NULL;

			if (m_streamHandles[i] != 0) {
				m_sounds[m_streamHandles[i]].m_stream = NULL;
				m_sounds[m_streamHandles[i]].m_streamPosition = 0;
				m_streamHandles[i] = 0;
			}
		}
	}
}

// miles.cpp:253
void WMilesSoundSystem::ClearAllSamples(void) {
	int i;
	for (i = 0; i < 16; i++) {
		if (m_handles2d[i] != NULL) {
			AIL_stop_sample(m_handles2d[i]);
			AIL_release_sample_handle(m_handles2d[i]);
			m_handles2d[i] = NULL;

			if (m_handleSounds2d[i] > 0) {
				m_sounds[m_handleSounds2d[i]].m_sample2d = NULL;
				m_handleSounds2d[i] = 0;
			}
		}
	}

	memset(m_handles2d, 0, sizeof(m_handles2d));

	for (i = 0; i < 16; i++) {
		if (m_handles3d[i] != NULL) {
			AIL_stop_sample(m_handles3d[i]);
			AIL_release_sample_handle(m_handles3d[i]);
			m_handles3d[i] = NULL;

			if (m_handleSounds3d[i] > 0) {
				m_sounds[m_handleSounds3d[i]].m_sample3d = NULL;
				m_handleSounds3d[i] = 0;
			}
		}
	}

	memset(m_handles3d, 0, sizeof(m_handles3d));
}

// miles.cpp:304
void WMilesSoundSystem::ClearAllSounds(void) {
	for (msample2d *sample2d = m_samples2d.Start(); sample2d != NULL; sample2d = m_samples2d.Next()) {
		AIL_mem_free_lock(sample2d->m_data);
		delete[] (char *)sample2d;
	}
	m_samples2d.Reset();

	for (msample3d *sample3d = m_samples3d.Start(); sample3d != NULL; sample3d = m_samples3d.Next()) {
		AIL_mem_free_lock(sample3d->m_data);
		delete[] (char *)sample3d;
	}
	m_samples3d.Reset();
}

// miles.cpp:328
int WMilesSoundSystem::GetCPUpercent(void) {
	return m_dig != NULL ? AIL_digital_CPU_percent(m_dig) : 0;
}

// miles.cpp:338
bool WMilesSoundSystem::Init(HWND hwnd, int a, int b) {
	m_dig = AIL_open_digital_driver(a, b, 16, 1);
	if (m_dig == NULL) {
		char text[256];
		// "Cannot open the sound driver: %s"
		sprintf(text, "\xbb\xe7\xbf\xee\xb5\xe5 \xb5\xe5\xb6\xf3\xc0\xcc\xb9\xf6\xb8\xa6 \xbf\xad \xbc\xf6 \xbe\xf8\xbd\xc0\xb4\xcf\xb4\xd9: %s", AIL_last_error());

		MessageBoxA(NULL, text, "Miles Sound System", MB_OK);
		return false;
	}

	AIL_set_DirectSound_HWND(m_dig, hwnd);

	InitMixer();

	return true;
}

// miles.cpp:384
void WMilesSoundSystem::SpeakerType(w_speaker_type type) {
	int physicalChannels;
	int logicalChannels;
	float falloffPower = 0.0f;
	int spec;

	switch (type) {
	case w_speaker_2ch:
		spec = 2; physicalChannels = logicalChannels = 2;
		break;
	case w_speaker_headphone:
		physicalChannels = 2; logicalChannels = 2; spec = 0x20;
		break;
	case w_speaker_surround:
		physicalChannels = 2; logicalChannels = 3; spec = 0x30;
		break;
	case w_speaker_4ch:
		physicalChannels = 4; logicalChannels = 4; spec = 0x50;
		break;
	case w_speaker_51ch:
		physicalChannels = 6; logicalChannels = 6; spec = 0x60;
		break;
	case w_speaker_71ch:
		physicalChannels = 8; logicalChannels = 8; spec = 0x80;
		break;
	}

	void *speakers = AIL_speaker_configuration(m_dig, &physicalChannels, &logicalChannels, &falloffPower, &spec);
	AIL_set_speaker_configuration(m_dig, speakers, 0, 0.0f);
}

// miles.cpp:441
void WMilesSoundSystem::SetBalance(int balance) {
	if (balance < 128) {
		m_leftVolume = 1.0f; m_rightVolume = balance < 0 ? 0.0f : balance / 128.0f;
	} else {
		float left = balance > 256 ? 1.0f : (256 - balance) / 128.0f;
		m_rightVolume = 1.0f; m_leftVolume = left;
	}
}

// miles.cpp:457
void WMilesSoundSystem::Activate(bool active) {
}

// miles.cpp:518
void WMilesSoundSystem::ClearIdleSound(int frames) {
	WList<msample2d *> idle2d(16, 16);
	WList<msample3d *> idle3d(16, 16);

	msample2d *sample2d;
	for (sample2d = m_samples2d.Start(); sample2d != NULL; sample2d = m_samples2d.Next()) {
		if (sample2d->m_refCount == 0) {
			sample2d->m_idleFrames++;

			if (sample2d->m_idleFrames >= frames + 1) {
				idle2d += sample2d;
			}
		}
	}

	for (sample2d = idle2d.Start(); sample2d != NULL; sample2d = idle2d.Next()) {
		m_samples2d -= sample2d;
		AIL_mem_free_lock(sample2d->m_data);
		delete[] (char *)sample2d;
	}

	msample3d *sample3d;
	for (sample3d = m_samples3d.Start(); sample3d != NULL; sample3d = m_samples3d.Next()) {
		if (sample3d->m_refCount == 0) {
			sample3d->m_idleFrames++;

			if (sample3d->m_idleFrames >= frames + 1) {
				idle3d += sample3d;
			}
		}
	}

	for (sample3d = idle3d.Start(); sample3d != NULL; sample3d = idle3d.Next()) {
		m_samples3d -= sample3d;
		AIL_mem_free_lock(sample3d->m_data);
		delete[] (char *)sample3d;
	}
}

// miles.cpp:583
int WMilesSoundSystem::Load(char *name, int type) {
	int handle = GetEmptyHandle();

	if (handle > 0) {
		if (type == 2) {
			unsigned int i;
			for (i = 0; m_streams[i] != NULL && i < 8; i++) {
				int status = AIL_stream_status(m_streams[i]);
				if (status == 2 || status == 8) {
					break;
				}
			}
			if (i < 8) {
				if (m_streams[i] != NULL) {
					if (m_sounds[m_streamHandles[i]].m_stream == m_streams[i]) {
						m_sounds[m_streamHandles[i]].m_stream = NULL;
					}
					AIL_close_stream(m_streams[i]);
					m_streams[i] = NULL;
				}

				_STREAM *stream = AIL_open_stream(m_dig, name, 0);
				if (stream != NULL) {
					m_streams[i] = stream; m_streamHandles[i] = handle;
					AIL_set_stream_user_data(stream, i, AIL_sample_playback_rate(AIL_stream_sample_handle(stream)));

					m_sounds[handle].m_stream = stream;
					m_sounds[handle].m_state = 2;

					SetVolume(handle, 1.0f);

					return handle;
				}
			}
		} else if (type == 1) {
			msample3d *sample3d = LoadSample3D(name);
			if (sample3d != NULL) {
				m_sounds[handle].m_data3d = sample3d;
				m_sounds[handle].m_sample3d = NULL;
				m_sounds[handle].m_near = m_nearDistance;
				m_sounds[handle].m_far = m_farDistance;
				m_sounds[handle].m_state = 1;
				SetVolume(handle, 1.0f);
				return handle;
			}
		} else {
			msample2d *sample2d = LoadSample2D(name);
			if (sample2d != NULL) {
				m_sounds[handle].m_data2d = sample2d;
				m_sounds[handle].m_sample2d = NULL;
				m_sounds[handle].m_state = 0;
				SetVolume(handle, 1.0f);
				return handle;
			}
		}
	}

	return 0;
}

// miles.cpp:689
void WMilesSoundSystem::DestroySoundBuffer(int handle) {
	Stop(handle);

	switch (m_sounds[handle].m_state) {
	case 0:
		Release2D(m_sounds[handle].m_data2d);
		m_sounds[handle].m_data2d = NULL;
		break;
	case 1:
		Release3D(m_sounds[handle].m_data3d);
		m_sounds[handle].m_data3d = NULL;
		break;
	case 2:
		if (m_sounds[handle].m_stream != NULL) {
			AIL_close_stream(m_sounds[handle].m_stream);

			for (unsigned int i = 0; i < 8; i++) {
				if (m_streams[i] == m_sounds[handle].m_stream) {
					m_streams[i] = NULL;
					if (m_streamHandles[i] != 0) {
						m_sounds[m_streamHandles[i]].m_stream = NULL;
						m_streamHandles[i] = 0;
					}
					break;
				}

				if (m_streamHandles[i] == handle) {
					m_streamHandles[i] = 0; AIL_close_stream(m_streams[i]);
					m_streams[i] = NULL;
				}
			}

			m_sounds[handle].m_stream = NULL;
		}
		break;
	}

	m_sounds[handle].m_state = 3;
}

// miles.cpp:777
void WMilesSoundSystem::Volume(float volume) {
	SetMixer(volume);
}

// miles.cpp:785
void WMilesSoundSystem::Play(int handle, int loop, bool stopPrevious) {
	switch (m_sounds[handle].m_state) {
	case 0:
		if (m_sounds[handle].m_data2d->m_data != NULL) {
			int last = m_sounds[handle].m_data2d->m_lastSound;
			m_sounds[handle].m_data2d->m_lastSound = handle;

			if (!m_multiPlay) {
				if (stopPrevious && m_sounds[last].m_state == 0 && m_sounds[last].m_data2d == m_sounds[handle].m_data2d) {
					Stop(last);
				}
			}

			_SAMPLE *sample = GetAllocateSample(handle, loop != 0);
			if (sample != NULL) {
				AIL_set_named_sample_file(sample, m_sounds[handle].m_data2d->m_name, m_sounds[handle].m_data2d->m_data, m_sounds[handle].m_data2d->m_size, 0);
				AIL_set_sample_loop_count(sample, loop == 0);

				AIL_set_sample_volume_pan(sample, m_sounds[handle].m_volume, 0.5f);
				AIL_start_sample(sample);
			}
		}
		break;
	case 1:
		{
			int last = m_sounds[handle].m_data3d->m_lastSound;
			m_sounds[handle].m_data3d->m_lastSound = handle;

			if (!m_multiPlay) {
				if (stopPrevious && m_sounds[last].m_state == 1 && m_sounds[last].m_data3d == m_sounds[handle].m_data3d) {
					Stop(last);
				}
			}

			_SAMPLE *sample = GetAllocateSample3D(handle, loop != 0);
			if (sample != NULL) {
				AIL_set_sample_file(sample, m_sounds[handle].m_data3d->m_data, 0);
				AIL_set_sample_loop_count(sample, loop == 0);
				AIL_set_sample_3D_distances(sample, m_sounds[handle].m_near, m_sounds[handle].m_far, 0);
				AIL_set_sample_volume_pan(sample, m_sounds[handle].m_volume, 0.5f);
				AIL_set_sample_3D_position(m_sounds[handle].m_sample3d, m_sounds[handle].m_x, m_sounds[handle].m_y, m_sounds[handle].m_z); AIL_start_sample(sample);
			}
		}
		break;
	case 2:
		if (m_sounds[handle].m_stream != NULL) {
			AIL_set_stream_loop_count(m_sounds[handle].m_stream, loop == 0);

			_SAMPLE *sample = AIL_stream_sample_handle(m_sounds[handle].m_stream);
			AIL_set_sample_volume_pan(sample, m_sounds[handle].m_volume, 0.5f);

			AIL_start_stream(m_sounds[handle].m_stream);
		}
		break;
	}
}

// miles.cpp:894
void WMilesSoundSystem::Resume(int handle, int loop) {
	switch (m_sounds[handle].m_state) {
	case 2:
		if (m_sounds[handle].m_stream != NULL) {
			AIL_set_stream_loop_count(m_sounds[handle].m_stream, loop == 0);

			_SAMPLE *sample = AIL_stream_sample_handle(m_sounds[handle].m_stream);
			AIL_set_sample_volume_pan(sample, m_sounds[handle].m_volume, 0.5f);

			AIL_set_stream_position(m_sounds[handle].m_stream, m_sounds[handle].m_streamPosition);
			AIL_start_stream(m_sounds[handle].m_stream);
		}
		break;
	}
}

// miles.cpp:927
void WMilesSoundSystem::Stop(int handle) {
	switch (m_sounds[handle].m_state) {
	case 0:
		if (m_sounds[handle].m_sample2d != NULL) {
			for (int i = 0; i < 16; i++) {
				if (m_handleSounds2d[i] == handle) {
					m_handleSounds2d[i] = 0; AIL_stop_sample(m_handles2d[i]);
				}
			}

			m_sounds[handle].m_sample2d = NULL;
		}
		break;
	case 1:
		if (m_sounds[handle].m_sample3d != NULL) {
			for (int i = 0; i < 16; i++) {
				if (m_handleSounds3d[i] == handle) {
					m_handleSounds3d[i] = 0; AIL_stop_sample(m_handles3d[i]);
				}
			}

			m_sounds[handle].m_sample3d = NULL;
		}
		break;
	case 2:
		AIL_pause_stream(m_sounds[handle].m_stream, 1);
		m_sounds[handle].m_streamPosition = AIL_stream_position(m_sounds[handle].m_stream);
		break;
	}
}

// miles.cpp:1022
void WMilesSoundSystem::SetPitch(int handle, int rate) {
	AIL_set_sample_playback_rate(m_sounds[handle].m_sample2d, rate);
}

// miles.cpp:1028
int WMilesSoundSystem::GetPitch(int handle) {
	return AIL_sample_playback_rate(m_sounds[handle].m_sample2d);
}

// miles.cpp:1033
void WMilesSoundSystem::SetVolume(int handle, float volume) {
	switch (m_sounds[handle].m_state) {
	case 0:
		m_sounds[handle].m_volume = volume = volume * m_leftVolume;
		if (m_sounds[handle].m_sample2d != NULL) {
			AIL_set_sample_volume_pan(m_sounds[handle].m_sample2d, volume, 0.5f);
		}
		break;
	case 1:
		m_sounds[handle].m_volume = volume = volume * m_rightVolume;
		if (m_sounds[handle].m_sample3d != NULL) {
			AIL_set_sample_volume_pan(m_sounds[handle].m_sample3d, volume, 0.5f);
		}
		break;
	case 2:
		m_sounds[handle].m_volume = volume * m_leftVolume;
		if (m_sounds[handle].m_stream != NULL) {
			_SAMPLE *sample = AIL_stream_sample_handle(m_sounds[handle].m_stream);
			AIL_set_sample_volume_pan(sample, m_sounds[handle].m_volume, 0.5f);
		}
		break;
	}
}

// miles.cpp:1076
void WMilesSoundSystem::SetNearFar(int handle, float nearDist, float farDist) {
	m_sounds[handle].m_near = nearDist;
	m_sounds[handle].m_far = farDist;

	if (m_sounds[handle].m_sample3d != NULL) {
		AIL_set_sample_3D_distances(m_sounds[handle].m_sample3d, nearDist, farDist, 0);
	}
}

// miles.cpp:1091
void WMilesSoundSystem::SetPosition(int handle, float x, float y, float z) {
	m_sounds[handle].m_x = m_invertX ? -x : x;
	m_sounds[handle].m_y = y;
	m_sounds[handle].m_z = z;

	if (m_sounds[handle].m_sample3d != NULL) {
		AIL_set_sample_3D_position(m_sounds[handle].m_sample3d, x, y, z);
	}
}

// miles.cpp:1111
bool WMilesSoundSystem::IsPlaying(int handle) {
	switch (m_sounds[handle].m_state) {
	case 0:
		return m_sounds[handle].m_sample2d != NULL && AIL_sample_status(m_sounds[handle].m_sample2d) == 4 ? true : false;
	case 1:
		return m_sounds[handle].m_sample3d != NULL && AIL_sample_status(m_sounds[handle].m_sample3d) == 4 ? true : false;
	case 2:
		if (m_sounds[handle].m_stream != NULL) {
			int status = AIL_stream_status(m_sounds[handle].m_stream);
			if (status != 2 && status != 8) {
				return true;
			}
		}
		break;
	}

	return false;
}

// miles.cpp:1150
int WMilesSoundSystem::GetPosition(int handle) {
	switch (m_sounds[handle].m_state) {
	case 0:
		if (m_sounds[handle].m_sample2d != NULL && AIL_sample_status(m_sounds[handle].m_sample2d) == 4) {
			return AIL_sample_position(m_sounds[handle].m_sample2d);
		}
		break;
	case 1:
		return AIL_sample_status(m_sounds[handle].m_sample3d) == 4 ? AIL_sample_position(m_sounds[handle].m_sample3d) : -1;
	case 2:
		if (AIL_stream_status(m_sounds[handle].m_stream) == 4) {
			int position;
			AIL_stream_ms_position(m_sounds[handle].m_stream, NULL, &position);
			return position / 1000;
		}
		break;
	}

	return -1;
}

// miles.cpp:1177
int WMilesSoundSystem::GetLength(int handle) {
	switch (m_sounds[handle].m_state) {
	case 0:
	case 1:
		return 0;
	case 2:
		int datarate, sndtype, memory;
		AIL_stream_info(m_sounds[handle].m_stream, &datarate, &sndtype, &handle, &memory);
		return handle / datarate;
	}

	return -1;
}

// miles.cpp:1200
WMilesSoundSystem::msample2d *WMilesSoundSystem::LoadSample2D(char *name) {
	msample2d *sample = m_samples2d.Find(name);
	if (sample == NULL) {
		sample = (msample2d *)new char[strlen(name) + sizeof(msample2d)];
		sample->m_size = AIL_file_size(name);
		sample->m_data = sample->m_size > 0 ? AIL_file_read(name, NULL) : NULL;
		sample->m_refCount = 0;
		sample->m_lastSound = 0;
		strcpy(sample->m_name, name);
		m_samples2d.AddItem(sample, sample->m_name, false);
	}
	sample->m_refCount++;

	return sample;
}

// miles.cpp:1219
void WMilesSoundSystem::Release2D(msample2d *sample) {
	sample->m_refCount--;
	sample->m_idleFrames = 0;
}

// miles.cpp:1227
void WMilesSoundSystem::LoadSample3D(msample3d *sample) {
	void *data;
	AILSOUNDINFO info;

	int size = AIL_file_size(sample->m_name);
	sample->m_data = AIL_file_read(sample->m_name, NULL);

	switch (AIL_file_type(sample->m_data, size)) {
	case 2:
		AIL_WAV_info(sample->m_data, &info);
		AIL_decompress_ADPCM(&info, &data, NULL);
		AIL_mem_free_lock(sample->m_data);
		sample->m_data = data;
		break;
	case 0xD:
		AIL_decompress_ASI(sample->m_data, size, sample->m_name, &data, NULL, NULL);
		AIL_mem_free_lock(sample->m_data);
		sample->m_data = data;
		break;
	}
}

// miles.cpp:1253
WMilesSoundSystem::msample3d *WMilesSoundSystem::LoadSample3D(char *name) {
	msample3d *sample = m_samples3d.Find(name);
	if (sample == NULL) {
		sample = (msample3d *)new char[strlen(name) + sizeof(msample3d)];

		sample->m_refCount = 0;
		sample->m_idleFrames = 0;
		sample->m_lastSound = 0;
		strcpy(sample->m_name, name);

		LoadSample3D(sample);

		m_samples3d.AddItem(sample, sample->m_name, false);
	}
	sample->m_refCount++;

	return sample;
}

// miles.cpp:1288
void WMilesSoundSystem::Release3D(msample3d *sample) {
	sample->m_refCount--;
	sample->m_idleFrames = 0;
}

// miles.cpp:1380
void WMilesSoundSystem::Process(void) {
}

// miles.cpp:1416
int WMilesSoundSystem::GetEmptyHandle(void) {
	for (int i = 0; i < 1024; i++) {
		m_lastHandle = (m_lastHandle + 1) & 0x3FF;

		if (m_lastHandle > 0 && m_sounds[m_lastHandle].m_state == 3) {
			return m_lastHandle;
		}
	}

	return 0;
}

// miles.cpp:1440
void WMilesSoundSystem::ClearIdleSample(void) {
	int i;
	for (i = 0; i < 16; i++) {
		if (m_handles2d[i] != NULL) {
			unsigned int status = AIL_sample_status(m_handles2d[i]);
			if (status == 2 || status == 8) {
				if (m_handleSounds2d[i] > 0) {
					if (m_sounds[m_handleSounds2d[i]].m_sample2d == m_handles2d[i]) {
						m_sounds[m_handleSounds2d[i]].m_sample2d = NULL;
					}
					m_handleSounds2d[i] = 0;
				}

				AIL_stop_sample(m_handles2d[i]);
				AIL_release_sample_handle(m_handles2d[i]);
				m_handles2d[i] = NULL;
			}
		}
	}

	for (i = 0; i < 16; i++) {
		if (m_handles3d[i] != NULL) {
			unsigned int status = AIL_sample_status(m_handles3d[i]);
			if (status == 2 || status == 8) {
				if (m_handleSounds3d[i] > 0) {
					if (m_sounds[m_handleSounds3d[i]].m_sample3d == m_handles3d[i]) {
						m_sounds[m_handleSounds3d[i]].m_sample3d = NULL;
					}
					m_handleSounds3d[i] = 0;
				}

				AIL_stop_sample(m_handles3d[i]);
				AIL_release_sample_handle(m_handles3d[i]);
				m_handles3d[i] = NULL;
			}
		}
	}
}

// miles.cpp:1512
_SAMPLE *WMilesSoundSystem::GetAllocateSample(int handle, bool flag) {
	unsigned int i;
	for (i = 0; i < 16; i++) {
		if (m_handles2d[i] == NULL) {
			m_handles2d[i] = AIL_allocate_sample_handle(m_dig);
			break;
		}

		unsigned int status = AIL_sample_status(m_handles2d[i]);
		if (status == 2 || status == 8) {
			break;
		}
	}

	if (i == 16 || m_handles2d[i] == NULL) {
		for (i = 0; i < 16; i++) {
			if (m_handles2d[i] != NULL && !m_handleFlags2d[i]) {
				break;
			}
		}

		if (i == 16) {
			return NULL;
		}
	}

	if (m_handleSounds2d[i] > 0) {
		if (m_sounds[m_handleSounds2d[i]].m_sample2d == m_handles2d[i]) {
			m_sounds[m_handleSounds2d[i]].m_sample2d = NULL;
		}
	}

	m_handleSounds2d[i] = handle;
	m_handleFlags2d[i] = flag;
	m_sounds[handle].m_sample2d = m_handles2d[i];

	AIL_init_sample(m_handles3d[i], 2);

	return m_handles2d[i];
}

// miles.cpp:1576
_SAMPLE *WMilesSoundSystem::GetAllocateSample3D(int handle, bool flag) {
	unsigned int i;
	for (i = 0; i < 16; i++) {
		if (m_handles3d[i] == NULL) {
			m_handles3d[i] = AIL_allocate_sample_handle(m_dig);
			break;
		}

		unsigned int status = AIL_sample_status(m_handles3d[i]);
		if (status == 2 || status == 8) {
			break;
		}
	}

	if (i == 16 || m_handles3d[i] == NULL) {
		for (i = 0; i < 16; i++) {
			if (m_handles3d[i] != NULL && !m_handleFlags3d[i]) {
				break;
			}
		}

		if (i == 16) {
			return NULL;
		}
	}

	if (m_handleSounds3d[i] > 0) {
		if (m_sounds[m_handleSounds3d[i]].m_sample3d == m_handles3d[i]) {
			m_sounds[m_handleSounds3d[i]].m_sample3d = NULL;
		}
	}

	m_handleSounds3d[i] = handle;
	m_handleFlags3d[i] = flag;
	m_sounds[handle].m_sample3d = m_handles3d[i];

	return m_handles3d[i];
}

// miles.cpp:1652
unsigned int __stdcall WMilesSoundSystem::Open_callback(const char *name, unsigned int *handle) {
	for (unsigned int i = 1; i < 16; i++) {
		if (g_files[i] == NULL) {
			cFile *file = g_resrcmng->GetCFile(name, 0xFFFF);

			if (file == NULL) {
				return 0;
			}

			g_files[i] = file;
			g_offset[i] = -1;
			*handle = i;
			return 1;
		}
	}

	return 0;
}

// miles.cpp:1690
void __stdcall WMilesSoundSystem::Close_callback(unsigned int handle) {
	CloseCFile(g_files[handle]);
	g_files[handle] = NULL;
}

// miles.cpp:1702
int __stdcall WMilesSoundSystem::Seek_callback(unsigned int handle, int offset, unsigned int type) {
	switch (type) {
	case 1:
		offset += g_files[handle]->Tell();
		break;
	case 2:
		offset += g_files[handle]->Length();
		break;
	}

	g_offset[handle] = offset;
	return offset;
}

// miles.cpp:1724
unsigned int __stdcall WMilesSoundSystem::Read_callback(unsigned int handle, void *buffer, unsigned int bytes) {
	if (g_offset[handle] != -1) {
		g_files[handle]->Seek(g_offset[handle], 0);
		g_offset[handle] = -1;
	}

	return g_files[handle]->Read(buffer, bytes);
}
