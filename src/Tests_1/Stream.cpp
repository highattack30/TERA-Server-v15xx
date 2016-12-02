#include "Stream.h"


Stream::Stream()
{
	_raw = 0;
	_size = 0;
	_pos = 0;
	_flag1 = false;
}

Stream::Stream(std::istream * str)
{
	str->seekg(0, std::istream::end);
	_size = (uint32)str->tellg();
	str->seekg(0, std::istream::beg);

	_raw = new byte[_size];
	str->read((char*)_raw, _size);
	_pos = 0;
}

Stream::Stream(unsigned int size)
{
	_size = size;
	_raw = (byte*)malloc(size);
	memset((void*)_raw, 0, _size);
	_pos = 0;
	_flag1 = false;
}

Stream::Stream(byte * data, unsigned int size)
{
	_size = size;
	_flag1 = false;
	_pos = 0;
	_raw = new byte[_size];
	if (memcpy_s((void*)_raw, _size, (const void*)data, _size) != 0)
	{
		if (_raw)
		{
			delete[] _raw;
			_raw = 0;

		}
		return;
	}
}

Stream::Stream(std::vector<byte>& vec)
{
	_size = (uint16)vec.size();
	_raw = new byte[_size];
	if (memcpy_s((void*)_raw, _size, (const void*)&vec[0], _size) != 0)
	{
		if (_raw)
		{
			delete[] _raw;
			_raw = 0;

		}
		return;
	}
	_pos = 0;
	_flag1 = false;
}


Stream::~Stream()
{
	if (_raw)
	{
		delete[] _raw;
		_raw = 0;
	}
	_size = 0;
	_pos = 0;
}

void Stream::Resize(unsigned int size)
{
	if (size <= 0 || _size + size > (unsigned int)STREAM_MAX_SIZE)
		return;


	if (!_raw)
	{
		_raw = new byte[size];
		_size = size;
		memset((void*)_raw, 0, _size);
	}
	else
	{
		unsigned int newSize = _size + size;
		byte * newRaw = (byte*)realloc((void*)_raw, newSize);

		memset((void*)&newRaw[_size], 0, size);
		_size = newSize;
		_raw = newRaw;
	}



}

void Stream::Write(byte * data, unsigned int size)
{
	if (_size + size > (unsigned int)STREAM_MAX_SIZE) //0xffff because the packets header has only 2 bytes [00 00] for the size
		return;

	int allocSzie = size - _size + _pos;

	if (allocSzie > 0)
		Resize(allocSzie);

	if (memcpy_s((void*)&_raw[_pos], size, (const void*)data, size) != 0) return;


	if (allocSzie > 0)
	{
		_pos = _size;
	}
	else {
		_pos += size;
	}
}

void Stream::Read(byte * out_buffer, unsigned int size)
{
	if (size > _size - _pos)
	{
		int temp = (_size - _pos);

		memcpy_s((void*)out_buffer, size, (const void*)&_raw[_pos], temp);
		_pos = temp;

		return;
	}
	memcpy_s((void*)out_buffer, size, (const void*)&_raw[_pos], size);
	_pos += size;
}

unsigned short Stream::NextPos()
{
	short tempPos = _pos;
	WriteInt16(0);
	return tempPos;
}

short Stream::ReadInt16()
{
	if (_pos > _size - 2)
		return -1;

	short out = (_raw[_pos + 1] << 8) | _raw[_pos];
	_pos += 2;
	return out;
}

int Stream::ReadInt32()
{
	if (_size - 4 < _pos)
		return -999999;

	//int out = ((_raw[_pos + 3] << 24) | (_raw[_pos + 2] << 16) | (_raw[_pos + 1] << 8) | _raw[_pos]);
	int out = ((_raw[_pos + 3] << 24) | (_raw[_pos + 2] << 16) | (_raw[_pos + 1] << 8) | _raw[_pos]);
	_pos += 4;

	return out;
}

float Stream::ReadFloat()
{
	if (_size < _pos + 4)
		return FLT_MAX;

	float out = *(float*)(&_raw[_pos]);
	_pos += 4;
	return out;
}

long long Stream::ReadInt64()
{
	if (_size < _pos + 8)
		return 0xffffffffffffffff;

	long long l = 0xffffffffffffffff;
	for (int i = _pos; i < 8; ++i) {
		l = l | ((unsigned long long)_raw[i] << (8 * i));
	}
	_pos += 8;
	return l;
}

byte Stream::ReadByte()
{
	short temp = (short)_pos;
	_pos++;
	return _raw[temp];
}


//emmpy TODO
void Stream::WriteHexString(std::string data)
{
	//byte *out_byte = ServerUtils::HexStringToByteArray(data);
	//if (out_byte)
	//{
	//	Write(out_byte, data.length() / 2);
	//}
	//
	//delete[] out_byte;
	//out_byte = 0;
}

void Stream::WriteString(std::string s, bool reverse)
{
	byte * data = new byte[s.size() * 2];
	byte * pos = &data[0];

	for (size_t i = 0; i < s.size(); i++)
	{
		if (!reverse)
		{
			pos[0] = (byte)s[i];
			pos[1] = 0x00;
		}
		else
		{
			pos[1] = (byte)s[i];
			pos[0] = 0x00;
		}
		pos += 2;
	}

	Write(data, (uint32)(s.size() * 2));
	WriteInt16(0); // string end
}

void Stream::WriteByte(byte val)
{
	Write(&val, 1);
}

void Stream::WriteFloat(float val)
{
	Write(reinterpret_cast<byte*>(&val), 4);
}

void Stream::WriteDouble(double val)
{
	byte * doubleBytes = reinterpret_cast<byte*>(&val);
	Write(doubleBytes, 4);
}

void Stream::WriteInt16(short data, bool swapped)
{
	byte shortBytes[2];

	if (!swapped)
	{
		shortBytes[0] = (data & 0xff);
		shortBytes[1] = ((data >> 8) & 0xff);
	}
	else {
		shortBytes[1] = (data & 0xff);
		shortBytes[0] = ((data >> 8) & 0xff);
	}

	Write(shortBytes, 2);
}

void Stream::WriteInt32(int data, bool swapped)
{
	byte intBytes[4];
	if (!swapped)
	{
		intBytes[0] = data & 0x000000ff;
		intBytes[1] = (data & 0x0000ff00) >> 8;
		intBytes[2] = (data & 0x00ff0000) >> 16;
		intBytes[3] = (data & 0xff000000) >> 24;
	}
	else
	{
		intBytes[3] = data & 0x000000ff;
		intBytes[2] = (data & 0x0000ff00) >> 8;
		intBytes[1] = (data & 0x00ff0000) >> 16;
		intBytes[0] = (data & 0xff000000) >> 24;
	}
	Write(intBytes, 4);
}

void Stream::WriteInt64(int64_t data)
{
	byte intBytes[8];

	intBytes[0] = ((data >> 0) & 0xff);
	intBytes[1] = ((data >> 8) & 0xff);
	intBytes[2] = ((data >> 16) & 0xff);
	intBytes[3] = ((data >> 24) & 0xff);
	intBytes[4] = ((data >> 32) & 0xff);
	intBytes[5] = ((data >> 40) & 0xff);
	intBytes[6] = ((data >> 48) & 0xff);
	intBytes[7] = ((data >> 56) & 0xff);

	Write(intBytes, 8);
}

void Stream::WritePos(short s, short offset)
{
	short temp = (short)_pos;
	_pos = s;
	WriteInt16(temp + offset);
	_pos = temp;
}


std::string Stream::ReadUTF16StringBigEdianToASCII() {

	std::string out;
	int count = 0;
	for (size_t i = _pos; i < _size; i += 2)
	{
		if (_raw[i] == 0x00 && _raw[i + 1] == 0x00)
			break;
		count += 2;
		out.push_back(_raw[i]);
	}
	count += 2;
	_pos += count;

	return out;
}

std::string Stream::ReadASCIIString()
{
	if (_size - _pos < 1)
		return "-1";

	std::string out;
	int kk = 0;
	for (size_t i = _pos; i < _size; i++)
	{
		if (_raw[i] != 0x00)
		{
			if (_raw[i] != '\r' || _raw[i] != '\n')
			{
				out += (char)_raw[i];
			}
			kk++;
		}
		else
			break;
	}
	_pos += kk;
	return out;
}

bool Stream::ReadASCIIStringTo(byte * out, unsigned short max_len)
{
	if (!out)
		return false;

	std::string text = ReadUTF16StringBigEdianToASCII();
	if (text.size() > max_len)
		return false;


	return memcpy(out, text.c_str(), text.size() + 1) ? true : false;
}

std::string Stream::ReadUTF16StringLittleEdianToASCII()
{
	std::string out;
	for (size_t i = _pos; i < _size; i += 2)
	{
		if (_raw[i] == 0x00 && _raw[i + 1] == 0x00)
			break;

		out.push_back(_raw[i + 1]);
	}
	_pos += (uint16)(out.size() * 2) + 2;

	return out;
}

void Stream::Clear()
{
	if (_raw)
	{
		delete[] _raw;
		_raw = 0;
	}
	_size = 0;
	_pos = 0;
}

unsigned int Stream::SetEnd()
{
	_pos = _size;
	return _pos;
}

unsigned int Stream::SetFront()
{
	_pos = 0;
	return 0;

}

void Stream::MoveForward(unsigned int val)
{
	if (_size < (val + _pos))
		return;

	_pos += val;
}

void Stream::MoveBackwards(unsigned int val)
{
	if (_size - (_pos + val) < 0)
		return;

	_pos -= val;
}
