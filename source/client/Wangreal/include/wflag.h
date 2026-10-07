#pragma once

class WFlags
{
public:
	WFlags()
		: m_flag(0)
	{
	}
	__forceinline WFlags(const unsigned long& flag)
		: m_flag(flag)
	{
	}
	void Set(unsigned long value) { this->m_flag = value; }
	__forceinline void Enable(unsigned long flag) { this->m_flag |= flag; }
	__forceinline void Disable(unsigned long flag) { this->m_flag &= ~flag; }
	__forceinline void Turn(unsigned long flag, bool on)
	{
		if (on == true)
			this->m_flag |= flag;
		else
			this->m_flag &= ~flag;
	}
	void Reset() { this->m_flag = 0; }
	bool GetFlag(unsigned long flag) const
	{
		return (this->m_flag & flag) ? true : false;
	}
	__forceinline operator unsigned long() const { return m_flag; }

private:
	unsigned long m_flag;
};
