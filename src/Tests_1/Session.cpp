#include "Crypt.hpp"
#include "socket.h"

using namespace Crypt;

//void Crypt::Session::Lock()
//{
//	_lockMutex.lock();
//}
//
//void Crypt::Session::Unlock()
//{
//	_lockMutex.unlock();
//}

Session::Session()
{
	memset(ClientKey1, 0, 128);
	memset(ClientKey2, 0, 128);
	memset(ServerKey1, 0, 128);
	memset(ServerKey2, 0, 128);
}


bool Crypt::Session::send_server_key(SOCKET sock, uint8 key_no)
{
	if (key_no == 1 || key_no == 2)
		return send(sock, (const char*)(key_no == 1 ? ServerKey1 : ServerKey2), 128, 0) ? true : false;

	return false;
}

bool Crypt::Session::fill_key(byte key[128], uint8 key_id)
{
	if (key_id == 1)
		return memcpy(ClientKey1, key, 128) ? true : false;
	else if (key_id == 2)
		return memcpy(ServerKey1, key, 128) ? true : false;
	else if (key_id == 3)
		return memcpy(ClientKey2, key, 128) ? true : false;
	else if (key_id == 4)
		return memcpy(ServerKey2, key, 128) ? true : false;

	return false;
}


bool Crypt::Session::init_session()
{
	byte TmpKey1[128], TmpKey2[128];
	Encryptor = new Cryptor();
	Decryptor = new Cryptor();


	ShiftKey(TmpKey1, ServerKey1, 67, true);
	XorKey(TmpKey2, TmpKey1, 128, ClientKey1, 128);

	ShiftKey(TmpKey1, ClientKey2, 29, false);
	XorKey(DecryptKey, TmpKey1, 128, TmpKey2, 128);

	Decryptor->GenerateKey(DecryptKey);


	ShiftKey(EncryptKey, ServerKey2, 41, true);
	Decryptor->ApplyCryptor(EncryptKey, 128);

	Encryptor->GenerateKey(EncryptKey);

	return true;
}

Session::~Session()
{
	delete Encryptor;
	delete Decryptor;
}

void Session::Encrypt(byte *data, int length)
{
	Encryptor->ApplyCryptor(data, length);
}

void Session::Decrypt(byte *data, int length)
{
	Decryptor->ApplyCryptor(data, length);
}

