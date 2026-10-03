#pragma once

#include <vector>

class WBone;

// TODO: incomplete
class WPolySoup
{
public:
	struct sTriangle
	{
		WVector v[3];
		WPlane plane;
		bool bDoubleSide;
		int texHandle;
		int propIndex;
		bool bEdge;
		WBone* bone;

		sTriangle()
			: texHandle(0), propIndex(0), bone(NULL)
		{
		}
	};

	bool LoadMapCheckData(unsigned char hole);
	const std::vector<sTriangle*>* GetTriArray(int texHandle) const;

	bool Load(const char* filename, bool bOption);
	void EndLoad(bool bPlaceHoleCup);
	void Clear();

	void MakeDataStructure();
	void MakeStatistics();
	void UpdateBright();
	void UpdateBgFxWind();
	void EnableShadowmap(bool bEnable);
	void SetRenderMode(int mode);
	bool IsOutOfBound(const WVector& pos);
	void ChangePointType(unsigned char from, unsigned char to);
	bool IsResisteredBgSequence(const std::string& name);
	int GetPropertyIndex(const char* name);
	int GetObjAlpha(int index);
	float GetObjWindFactor(int index);
	WVector GetCameraPos(const WVector& from, const WVector& to);
	void SetAdditionModel(const char* name, const WVector& pos);

	void ChangeFloorTexture(const char* from, const char* to);
	void ChangeObjectTexture(const char* from, const char* to, int index);
	Waabb GetExtraHoleStartArea() { return m_extraHoleStartArea; }

private:
	unsigned char m_unused0[0xec];
	Waabb m_extraHoleStartArea;
	// TODO: incomplete
};
