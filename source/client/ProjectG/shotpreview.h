// Original: d:\build\custom\temp\code\client\projectg\shotpreview.h
#ifndef PG_CLIENT_PROJECTG_SHOTPREVIEW_H
#define PG_CLIENT_PROJECTG_SHOTPREVIEW_H

#include <string>
#include <vector>

#include "..\..\defines.h"
#include "..\..\padding.h"
#include "actor.h"
#include "scenemanager.h"
#include "tweaker.h"
#include "../client/Wangreal/include/wmath.h"

class C3dFont;
class CGolfBall;
class FrButton;
class MsgObject;

// The shot preview actor: predicts the flight of the ball for the current
// shot and draws the guide line, the indicator and the pang coins along it.
// Its size (0x80) is the `new` of ShotPreviewMakeInstance.
class ShotPreview : public IActor, public CRenderFuncPtr, public tweaker_cmd_target {
	LAYOUT_CHECK_FRIEND;

public:
	class PangCoin;
	class UpdateSensor;

	struct PredictType {
		enum Enum {
		};
	};

	static const WRTTI m_RTTI;
	// shotpreview.h:35
	virtual const WRTTI* GetRTTI(void) const { return &m_RTTI; }

private:
	ShotPreview(void);
	virtual ~ShotPreview(void);
	friend IObject* ShotPreviewMakeInstance(void);   // the factory creates it

public:
	virtual void OnLoad(void);
	virtual void OnInit(void);
	virtual void OnReload(void);
	virtual void OnProcess(float delta);
	virtual void Display(void);

	std::string TwAddOption(const tweaker_param& param);
	std::string TwSubOption(const tweaker_param& param);

private:
	virtual void HandleMsg(const MsgObject& msg);

	bool IsSkipState(void);
	void DrawIndicator(const WVector& pos, unsigned long color, float size);
	bool Predict(std::vector<WVector>& path, std::vector<WVector>& bounces, int* count);
	unsigned short GetBallInitSpecialValueFromPredictType(PredictType::Enum type);

	// shotpreview.h:81
	unsigned int GetOptionFlag(void) const { return m_optionFlag; }
	// shotpreview.h:82
	void SetOptionFlag(unsigned int flag) { m_optionFlag = flag; }

	CGolfBall* m_pBall;                   // 0x20, the ball simulated by Predict
	std::vector<WVector> m_path;          // 0x24, points of the predicted flight (OnInit empties it)
	std::vector<WVector> m_path2;         // 0x34, second point list of the prediction
	UpdateSensor* m_pSensor;              // 0x44, the shot parameters of the last prediction
	float m_updateElapsed;                // 0x48, seconds since the last prediction check (OnProcess, every 0.5 s)
	int m_indicatorTexture;               // 0x4C, "{indicator.bmp"
	PredictType::Enum m_predictType;      // 0x50, CHANGE_PREDICT_TYPE
	unsigned int m_optionFlag;            // 0x54, GetOptionFlag / SetOptionFlag (10 at construction)
	int m_visible;                        // 0x58, 0: IsSkipState skips the preview (1 at construction, ENABLE_DISPLAY)
	C3dFont* m_pFont;                     // 0x5C, "[font_bar.jpg"
	std::vector<FrButton*> m_predictButtons;   // 0x60, the GolfRule's predict_* buttons (OnLoad)
	std::vector<PangCoin*> m_pangCoins;   // 0x70, deleted by the destructor
};

CHECK_SIZE(ShotPreview, 0x80);
BEGIN_LAYOUT_CHECK(ShotPreview)
	CHECK_FIELD(ShotPreview, m_pBall, 0x20);
	CHECK_FIELD(ShotPreview, m_path, 0x24);
	CHECK_FIELD(ShotPreview, m_path2, 0x34);
	CHECK_FIELD(ShotPreview, m_pSensor, 0x44);
	CHECK_FIELD(ShotPreview, m_updateElapsed, 0x48);
	CHECK_FIELD(ShotPreview, m_indicatorTexture, 0x4C);
	CHECK_FIELD(ShotPreview, m_predictType, 0x50);
	CHECK_FIELD(ShotPreview, m_optionFlag, 0x54);
	CHECK_FIELD(ShotPreview, m_visible, 0x58);
	CHECK_FIELD(ShotPreview, m_pFont, 0x5C);
	CHECK_FIELD(ShotPreview, m_predictButtons, 0x60);
	CHECK_FIELD(ShotPreview, m_pangCoins, 0x70);
END_LAYOUT_CHECK

#endif
