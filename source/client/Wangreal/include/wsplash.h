#pragma once

class WPoint;
class WRect;

class WSplash
{
public:
	WSplash();
	virtual ~WSplash();
	virtual int Init(BITMAPINFO& bi, void* vram, bool screenSized) = 0;
	virtual int InitFromScreen(bool screenSized) = 0;
	virtual void Reset() = 0;
	virtual void Draw(unsigned long color) = 0;
	virtual void Draw(const WPoint* pos, const WRect* rect,
		unsigned long color);
	virtual void ResetScreenSize() = 0;
	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
	virtual void SetTexCoordOffset(float u, float v) = 0;
};
