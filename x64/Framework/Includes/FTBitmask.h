#ifndef FTBITMASK_H
#define FTBITMASK_H

#ifdef __cplusplus
extern "C" {
#endif 

typedef struct FTBitmask
{
	int flags;
} FTBitmask;

// Determines if the flag is set to 1 in a bitmask
int IsFlagEnabled(FTBitmask* bitmask, int flag);

// Sets the flag in a bitmask to 1
void EnableFlag(FTBitmask* bitmask, int flag);

// Sets the flag in a bitmask to 0
void DisableFlag(FTBitmask* bitmask, int flag);

// Toggles the flag in a bitmask
void ToggleFlag(FTBitmask* bitmask, int flag);

#ifdef __cplusplus
}
#endif 

#endif // !FTBITMASK_H