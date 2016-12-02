#include "Crypt.hpp"

# define SHA_DIGEST_LENGTH 20

using namespace Crypt;

Cryptor::Cryptor()
{
	ChangeLen = 0;
	ChangeData = 0;
	Key[0].set(55, 31);
	Key[1].set(57, 50);
	Key[2].set(58, 39);
}

void Cryptor::FillKey(byte *src, byte *buf)
{
	for (int i = 0; i < 680; i++)
		buf[i] = src[i % 128];
	buf[0] = 128;
}

void Cryptor::GenerateKey(byte *src)
{
	byte buf[680];
	FillKey(src, buf);
	for (int i = 0; i < 680; i += 20)
	{
		Sha_l *sha = new Sha_l(buf, 680);
		memcpy(buf + i, sha->MessageDigest, SHA_DIGEST_LENGTH);
		delete sha;
	}
	memcpy(Key[0].Buffer, buf + 000, 220 - 000);
	memcpy(Key[1].Buffer, buf + 220, 448 - 220);
	memcpy(Key[2].Buffer, buf + 448, 680 - 448);
}

void Cryptor::ApplyCryptor(byte *buffer, int size)
{
	int pre = (size >= ChangeLen) ? ChangeLen : size;
	char *containerInt2;

	if (pre != 0)
	{
		if (pre > 0)
		{
			containerInt2 = (char*)&ChangeData;
			for (int j = 0; j < pre; j++)
				buffer[j] ^= containerInt2[(4 - ChangeLen) + j];
		}
		ChangeLen -= pre;
		size -= pre;
	}

	int offset = pre;
	for (int i = 0; i < size / 4; i++)
	{
		int result = (((Key[0].Key & Key[1].Key) | Key[2].Key) & (Key[0].Key | Key[1].Key));
		for (int j = 0; j < 3; j++)
		{
			if (result == Key[j].Key)
			{
				uint *t1, *t2;
				t1 = (uint*)(Key[j].Buffer + Key[j].Pos1 * 4);
				t2 = (uint*)(Key[j].Buffer + Key[j].Pos2 * 4);
				uint t3 = *t1 > *t2 ? *t2 : *t1;

				uint sum = *t1 + *t2;
				sum = sum > 0xFFFFFFFFL ? (uint)((uint)*t1 + (uint)*t2) & 0xFFFFFFFFL : sum;
				Key[j].Sum = sum;
				Key[j].Key = t3 <= sum ? 0 : 1;

				Key[j].Pos1 = (Key[j].Pos1 + 1) % Key[j].Size;
				Key[j].Pos2 = (Key[j].Pos2 + 1) % Key[j].Size;
			}
			if (i + 4 <= size) {
				uint *unsBuf = (uint*)&buffer[offset + i * 4];
				*unsBuf ^= Key[j].Sum;
			}
			else {
				uint unsBuf;
				memcpy(&unsBuf, buffer + offset + i * 4, 4);
				unsBuf ^= Key[j].Sum;
				memcpy(buffer + offset + i * 4, &unsBuf, 4);
			}
		}
	}

	int remain = size & 3;
	if (remain != 0)
	{
		int result = (((Key[0].Key & Key[1].Key) | Key[2].Key) & (Key[0].Key | Key[1].Key));
		ChangeData = 0;
		for (int j = 0; j < 3; j++)
		{
			if (result == Key[j].Key)
			{
				uint *t1, *t2;
				t1 = (uint*)(Key[j].Buffer + Key[j].Pos1 * 4);
				t2 = (uint*)(Key[j].Buffer + Key[j].Pos2 * 4);
				uint t3 = *t1 > *t2 ? *t2 : *t1;

				uint sum = *t1 + *t2;
				sum = sum > 0xFFFFFFFFL ? (uint)((uint)*t1 + (uint)*t2) & 0xFFFFFFFFL : sum;
				Key[j].Sum = sum;
				Key[j].Key = t3 <= sum ? 0 : 1;

				Key[j].Pos1 = (Key[j].Pos1 + 1) % Key[j].Size;
				Key[j].Pos2 = (Key[j].Pos2 + 1) % Key[j].Size;
			}
			ChangeData ^= Key[j].Sum;
		}

		containerInt2 = (char*)&ChangeData;
		for (int j = 0; j < remain; j++)
			buffer[((size + pre) - remain) + j] ^= containerInt2[j];

		ChangeLen = 4 - remain;
	}
}
