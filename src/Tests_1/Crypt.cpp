#include "Crypt.hpp"

void Crypt::XorKey(byte *result, byte *key1, int Length1, byte *key2, int Length2)
{
	int Length = Length1 < Length2 ? Length1 : Length2;
	for (int i = 0; i < Length; i++)
		result[i] = (byte)(key1[i] ^ key2[i]);
}

void Crypt::ShiftKey(byte *result, byte *src, int n, bool direction)
{
	int Length = 128;
	for (int i = 0; i < Length; i++)
		if (direction)
			result[(i + n) % Length] = src[i];
		else
			result[i] = src[(i + n) % Length];
}

