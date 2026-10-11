// Original: d:\build\custom\temp\code\client\projectg\petal.h
#ifndef PG_CLIENT_PROJECTG_PETAL_H
#define PG_CLIENT_PROJECTG_PETAL_H

#include <string>

#include "..\..\defines.h"
#include "wmath.h"
#include "weather.h"

class CPetBody;

// Fluttering petals (and pets) around the camera.
class ntFlutter {
	LAYOUT_CHECK_FRIEND;
public:
	struct sFrag {
		WVector pos;            // 0x0, center + offset
		WVector center;         // 0xc
		float unknown18;        // 0x18
		WVector offset;         // 0x1c
		float yaw;              // 0x28, random 0..2pi
		float pitch;            // 0x2c, random 0..pi
		bool visible;           // 0x30
	};

	ntFlutter(int count, const std::string& name, int unused);
	~ntFlutter(void);

	virtual void Process(float fDelta, float fTime);
	virtual void Display(void);
	virtual void DisplayFullScreenOverlay(void);
	virtual void Reset(int nType, float fValue);

private:
	void ReleasePets(void);

	float m_size;               // 0x4, Reset's fValue
	Waabb m_area;               // 0x8
	sFrag* m_frags;             // 0x20
	CPetBody* m_pets[10];       // 0x24
	int m_count;                // 0x4c
	const std::string& m_name;  // 0x50
};
CHECK_SIZE(ntFlutter::sFrag, 0x34);
CHECK_FIELD(ntFlutter::sFrag, pos, 0x0);
CHECK_FIELD(ntFlutter::sFrag, center, 0xc);
CHECK_FIELD(ntFlutter::sFrag, offset, 0x1c);
CHECK_FIELD(ntFlutter::sFrag, yaw, 0x28);
CHECK_FIELD(ntFlutter::sFrag, pitch, 0x2c);
CHECK_FIELD(ntFlutter::sFrag, visible, 0x30);
CHECK_SIZE(ntFlutter, 0x54);
BEGIN_LAYOUT_CHECK(ntFlutter)
	CHECK_FIELD(ntFlutter, m_size, 0x4);
	CHECK_FIELD(ntFlutter, m_area, 0x8);
	CHECK_FIELD(ntFlutter, m_frags, 0x20);
	CHECK_FIELD(ntFlutter, m_pets, 0x24);
	CHECK_FIELD(ntFlutter, m_count, 0x4c);
END_LAYOUT_CHECK

// A lobby weather effect built on ntFlutter: T holds the pet file name
// pattern the flutter loads its pets from (m_holder is constructed before
// m_flutter, which keeps a reference to its name).
template <class T>
class ntImplFlutter : public CWeatherType {
public:
	ntImplFlutter(int num, const char* name, int type)
		: m_flutter(num, m_holder.m_petName, type) {
	}

	virtual ~ntImplFlutter(void) {
	}

	virtual void Process(float fDelta) {
		m_flutter.Process(fDelta, m_gravity);
	}

	virtual void Display(void) {
		m_flutter.Display();
	}

	virtual void DisplayFullScreenOverlay(void) {
		m_flutter.DisplayFullScreenOverlay();
	}

	virtual void Reset(int nType, float fValue) {
		m_flutter.Reset(nType, fValue);
	}

private:
	T m_holder;           // 0x8
	ntFlutter m_flutter;  // 0x24
};

// Pink flower petals (lobby map 11).
struct sPetalNameHolder {
	sPetalNameHolder(void) : m_petName("pinkflower%02d.pet") {
	}

	std::string m_petName; // 0x0
};
CHECK_SIZE(sPetalNameHolder, 0x1c);

// Hell powder particles (lobby map 13).
struct sSilicaParticleNameHolder {
	sSilicaParticleNameHolder(void) : m_petName("Hell_powder%02d.pet") {
	}

	std::string m_petName; // 0x0
};
CHECK_SIZE(sSilicaParticleNameHolder, 0x1c);
CHECK_SIZE(ntImplFlutter<sPetalNameHolder>, 0x78);

#endif // PG_CLIENT_PROJECTG_PETAL_H
