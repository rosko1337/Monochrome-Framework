#include "FTBitmask.h"

// Determines if the flag is set to 1 in a bitmask
int IsFlagEnabled(FTBitmask* bitmask, int flag)
{
	if (bitmask->flags & flag)
	{
		return 1;
	}
	return 0;
}

// Sets the flag in a bitmask to 1
void EnableFlag(FTBitmask* bitmask, int flag)
{
	bitmask->flags |= flag;
}

// Sets the flag in a bitmask to 0
void DisableFlag(FTBitmask* bitmask, int flag)
{
	bitmask->flags &= ~flag;
}

// Toggles the flag in a bitmask
void ToggleFlag(FTBitmask* bitmask, int flag)
{
	bitmask->flags ^= flag;
}
