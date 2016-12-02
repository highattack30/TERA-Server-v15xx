#include "Crypt.hpp"

using namespace Crypt;

void CryptorKey::set(int size, int maxPos)
{
	Pos1 = Key = Sum = 0;
	Size = size;
	Pos2 = MaxPos = maxPos;
}
