#pragma once

inline bool IsLicenseLimitedItem(unsigned long tid)
{
	const int LICENSEITEMSIZE = 1;

	unsigned long aLicenseLimitedItem[LICENSEITEMSIZE] = { 0x14000004 };

	for (int i = 0; i < LICENSEITEMSIZE; i++)
	{
		if (aLicenseLimitedItem[i] == tid)
			return true;
	}

	return false;
}
