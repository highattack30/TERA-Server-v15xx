#include "Crypt.hpp"

using namespace Crypt;

ulong Sha_l::CircularShift(int bits, ulong word)
{
	return ((((word) << bits) & 0xFFFFFFFF) | ((word) >> (32 - bits)));
}

int Sha_l::Result()
{
	if (Corrupted != 0)
		return 0;

	if (Computed == 0) {
		PadMessage();
		Computed = 1;
	}

	return 1;
}

void Sha_l::Input(byte *messageArray, int Length)
{
	if (Length == 0)
		return;

	if (Computed > 0 || Corrupted > 0) {
		Corrupted = 1;
		return;
	}

	int counter = 0;
	while (counter < Length && Corrupted == 0) {
		MessageBlock[MessageBlockIndex++] = messageArray[counter];
		LengthLow += 8;
		LengthLow &= 0xFFFFFFFF; //Force it to 32 bits

		if (LengthLow == 0) {
			LengthHigh++;
			LengthHigh &= 0xFFFFFFFF; //Force it to 32 bits
			if (LengthHigh == 0) //Message is too long
				Corrupted = 1;
		}

		if (MessageBlockIndex == 64)
			ProcessMessageBlock();
		counter++;
	}
}

void Sha_l::ProcessMessageBlock()
{
	// Constants defined in SHA
	ulong k[4] = {
		0x5A827999,
		0x6ED9EBA1,
		0x8F1BBCDC,
		0xCA62C1D6
	};

	int t; // Loop counter
	ulong temp; // Temporary word value
	ulong w[80]; // Word sequence
				 // ReSharper disable JoinDeclarationAndInitializer
	ulong a, b, c, d, e; // Word buffers
						 // ReSharper restore JoinDeclarationAndInitializer

						 // Initialize the first 16 words in the array W
	for (t = 0; t < 16; t++) {
		w[t] = ((ulong)MessageBlock[t * 4]) << 24;
		w[t] |= ((ulong)MessageBlock[t * 4 + 1]) << 16;
		w[t] |= ((ulong)MessageBlock[t * 4 + 2]) << 8;
		w[t] |= MessageBlock[t * 4 + 3];
	}

	for (t = 16; t < 80; t++)
		w[t] = w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16];

	a = MessageDigest[0];
	b = MessageDigest[1];
	c = MessageDigest[2];
	d = MessageDigest[3];
	e = MessageDigest[4];

	for (t = 0; t < 20; t++) {
		temp = CircularShift(5, a) + ((b & c) | ((~b) & d)) + e + w[t] + k[0];
		temp &= 0xFFFFFFFF;
		e = d;
		d = c;
		c = CircularShift(30, b);
		b = a;
		a = temp;
	}
	for (t = 20; t < 40; t++) {
		temp = CircularShift(5, a) + (b ^ c ^ d) + e + w[t] + k[1];
		temp &= 0xFFFFFFFF;
		e = d;
		d = c;
		c = CircularShift(30, b);
		b = a;
		a = temp;
	}
	for (t = 40; t < 60; t++) {
		temp = CircularShift(5, a) + ((b & c) | (b & d) | (c & d)) + e + w[t] + k[2];
		temp &= 0xFFFFFFFF;
		e = d;
		d = c;
		c = CircularShift(30, b);
		b = a;
		a = temp;
	}
	for (t = 60; t < 80; t++) {
		temp = CircularShift(5, a) + (b ^ c ^ d) + e + w[t] + k[3];
		temp &= 0xFFFFFFFF;
		e = d;
		d = c;
		c = CircularShift(30, b);
		b = a;
		a = temp;
	}
	MessageDigest[0] = (uint)((MessageDigest[0] + a) & 0xFFFFFFFF);
	MessageDigest[1] = (uint)((MessageDigest[1] + b) & 0xFFFFFFFF);
	MessageDigest[2] = (uint)((MessageDigest[2] + c) & 0xFFFFFFFF);
	MessageDigest[3] = (uint)((MessageDigest[3] + d) & 0xFFFFFFFF);
	MessageDigest[4] = (uint)((MessageDigest[4] + e) & 0xFFFFFFFF);

	MessageBlockIndex = 0;
}

void Sha_l::PadMessage()
{
	/*
	*  Check to see if the current message block is too small to hold
	*  the initial padding bits and length.  If so, we will pad the
	*  block, process it, and then continue padding into a second
	*  block.
	*/
	if (MessageBlockIndex > 55) {
		MessageBlock[MessageBlockIndex++] = 0x80;
		while (MessageBlockIndex < 64)
			MessageBlock[MessageBlockIndex++] = 0;
		ProcessMessageBlock();
		while (MessageBlockIndex < 56)
			MessageBlock[MessageBlockIndex++] = 0;
	}
	else {
		MessageBlock[MessageBlockIndex++] = 0x80;
		while (MessageBlockIndex < 56)
			MessageBlock[MessageBlockIndex++] = 0;
	}

	// Store the message length as the last 8 octets
	MessageBlock[56] = (byte)(LengthHigh >> 24);
	MessageBlock[57] = (byte)(LengthHigh >> 16);
	MessageBlock[58] = (byte)(LengthHigh >> 8);
	MessageBlock[59] = (byte)(LengthHigh);
	MessageBlock[60] = (byte)(LengthLow >> 24);
	MessageBlock[61] = (byte)(LengthLow >> 16);
	MessageBlock[62] = (byte)(LengthLow >> 8);
	MessageBlock[63] = (byte)(LengthLow);

	ProcessMessageBlock();
}

Sha_l::Sha_l(byte *src, int size)
{
	LengthLow = 0;
	LengthHigh = 0;
	MessageBlockIndex = 0;
	Computed = 0;
	Corrupted = 0;
	memset(MessageBlock, 0, sizeof(byte) * 64);
	MessageDigest[0] = 0x67452301;
	MessageDigest[1] = 0xEFCDAB89;
	MessageDigest[2] = 0x98BADCFE;
	MessageDigest[3] = 0x10325476;
	MessageDigest[4] = 0xC3D2E1F0;
	Input(src, size);
	Result();
}
