#include "stdafx.h"
#include "SMDevice.h"


CSMDevice::CSMDevice()
{
	hMapFile = NULL;
	pBuf = NULL;
	this->gSMIni = TEXT("./SM.ini");
}

CSMDevice::CSMDevice(LPTSTR SMName)
{
	hMapFile = NULL;
	pBuf = NULL;
	this->gSMIni = SMName;
}

CSMDevice::~CSMDevice()
{
		if(pBuf != NULL)
			UnmapViewOfFile(pBuf);
		if(hMapFile != NULL)
			CloseHandle(hMapFile);
		MessageBox(NULL, TEXT("Delete"), TEXT("close"), MB_OK);
}

int CSMDevice::Initial()
{
	memset(this->gSMName,'\0', sizeof(char));
	GetPrivateProfileString("Comm", "SMName", "NO_VALUE", 
		this->gSMName, sizeof(this->gSMName), this->gSMIni);
	this->gComm_SMName = (LPTSTR)this->gSMName;
	this->gComm_SMsize = (unsigned long)GetPrivateProfileInt("Comm", "SMSize", 0, this->gSMIni);
	this->gAlarm_Address = (unsigned int)GetPrivateProfileInt("Alarm", "Address", 0, this->gSMIni);
	this->gAlarm_Length = (unsigned int)GetPrivateProfileInt("Alarm", "Length", 0, this->gSMIni);
	this->gWarring_Address = (unsigned int)GetPrivateProfileInt("Warring", "Address", 0, this->gSMIni);
	this->gWarring_Length = (unsigned int)GetPrivateProfileInt("Warring", "Length", 0, this->gSMIni);
	if(this->gComm_SMsize == 0)
	{
		MessageBox(NULL, TEXT("SMSize Paramater Miss or Value is 0 "), TEXT("Initial"), MB_OK);
		return 1;
	}
	if( (this->gAlarm_Address == 0) || (this->gAlarm_Length == 0) || (this->gWarring_Address == 0) || (this->gWarring_Length == 0) )
	{
		MessageBox(NULL, TEXT("Alarm/Warring Paramater Miss or Value is 0 "), TEXT("Initial"), MB_OK);
		return 2;
	}
	if( (this->gAlarm_Address + this->gAlarm_Length) > this->gComm_SMsize )
	{
		MessageBox(NULL, TEXT("Alarm Value is out range "), TEXT("Initial"), MB_OK);
		return 3;
	}
	if( (this->gWarring_Address + this->gWarring_Length) > this->gComm_SMsize )
	{
		MessageBox(NULL, TEXT("Warring Value is out range "), TEXT("Initial"), MB_OK);
		return 4;
	}
	if(strcmp(this->gComm_SMName,"NO_VALUE") == 0)
	{
		MessageBox(NULL, TEXT("SMName Paramater Miss"), TEXT("Initial"), MB_OK);
		return 5;
	}

	return 0;
}

int CSMDevice::chkAddress(unsigned int address)
{
	if( (unsigned int)this->gComm_SMsize > address )
		return 0;

	MessageBox(NULL, TEXT("Address no > SM Size"), TEXT("CSMDevice::chkAddress"), MB_OK);
	return 1;
}

int CSMDevice::Open()
{
	if(Initial() != 0)
		return 1;

	hMapFile = CreateFileMapping(
					INVALID_HANDLE_VALUE,				// use paging file
					NULL,								// default security
					PAGE_READWRITE,						// read/write access
					0,									// maximum object size (high-order DWORD)
					this->gComm_SMsize,					// maximum object size (low-order DWORD)
					this->gComm_SMName);				// name of mapping object

	if (hMapFile == NULL)
	{
		MessageBox(NULL, TEXT("CreateFileMapping Fail"), TEXT("CSMDevice::Open"), MB_OK);
		return 2;
	}
	pBuf = (unsigned short*) MapViewOfFile(hMapFile,	// handle to map object
					FILE_MAP_ALL_ACCESS,				// read/write permission
					0,
					0,
					this->gComm_SMsize);

	if (pBuf == NULL)
	{
		MessageBox(NULL, TEXT("MapViewOfFile Fail"), TEXT("CSMDevice::Open"), MB_OK);
		CloseHandle(hMapFile);
		return 3;
	}

	return 0;
}

int CSMDevice::AddIndex(unsigned int address)
{
	unsigned short *index;
	if( 0 != this->chkAddress(address) )
		return 100;
    index = pBuf+address;
	if(*index < 9999)
		*index = *index + 1;
	else
		*index = 1;
	return 0;
}

int CSMDevice::WriteShort(unsigned int address, short value)
{
	if( 0 != this->chkAddress(address) )
		return 100;

	memcpy((void *)(pBuf+address), &value, sizeof(short));
	return 0;
}

int CSMDevice::WriteInt(unsigned int address, int value)
{
	if( 0 != this->chkAddress(address+1) )
		return 100;

	memcpy((void *)(pBuf+address), &value, sizeof(int));
	return 0;
}

int CSMDevice::WriteDouble(unsigned int address, double value)
{
	if( 0 != this->chkAddress(address+3) )
		return 100;

	memcpy((void *)(pBuf+address), &value, sizeof(double));
	return 0;
}

int CSMDevice::WriteString(unsigned int address, LPTSTR value, unsigned short SpecLength)
{
	if( 0 != this->chkAddress(address +  (_tcslen(value) * sizeof(TCHAR)+1)/2-1 ) )
		return 100;
    memset((void *)(pBuf+address), '\0', SpecLength);
	memcpy((void *)(pBuf+address), value, _tcslen(value) * sizeof(TCHAR)>SpecLength?SpecLength: _tcslen(value) * sizeof(TCHAR) );

	return 0;
}

int CSMDevice::WriteString(unsigned int address, CString value, unsigned short SpecLength)
{
	this->WriteString( address, value.GetBuffer(value.GetLength()), SpecLength);
	value.ReleaseBuffer(value.GetLength());
	return 0;
}

int CSMDevice::AddAlarm(unsigned short AlarmCode)
{
	unsigned short *value;
	unsigned int tmp;

	try{
	for(unsigned int i=0; i < this->gAlarm_Length; i++)
	{
		tmp = this->gAlarm_Address + i;
		if( 0 != this->chkAddress(tmp) )
			return 100;
		value = (unsigned short *)(this->pBuf+tmp);
		if( *value == 0 )
		{
			*value = AlarmCode;
			return 0;
		}
		else if(*value == AlarmCode)
		{
			return 1; //Alarm Code is exist. No Action
		}
	}
	value = (unsigned short *)(this->pBuf+this->gAlarm_Address);
    *value = AlarmCode;
	}
	catch(...)
	{

		MessageBox(NULL, TEXT("Err"), TEXT("short"), MB_OK);

	}
	
	return 99; //Alarm Code SM is Full. Replace the first alarm code
}

int CSMDevice::DelAlarm(unsigned short AlarmCode)
{
	unsigned short *value;
	unsigned int tmp;

	for(unsigned int i=0; i < this->gAlarm_Length; i++)
	{
		tmp = this->gAlarm_Address + i;
		value = (unsigned short *)(this->pBuf+tmp);
		if( *value == AlarmCode )
		{
			*value = 0;
			return 0;
		}
	}
	
	return 99; //Alarm Code is not Found. No Action.
}

int CSMDevice::ClearAllAlarm()
{
	memset( (void *)(this->pBuf+this->gAlarm_Address), 0, this->gAlarm_Length * sizeof(short) );
	return 0; //Set all Alarm Code 0.
}

int CSMDevice::AddWarring(unsigned short WarringCode)
{
	unsigned short *value;
	unsigned int tmp;

	for(unsigned int i=0; i < this->gWarring_Length; i++)
	{
		tmp = this->gWarring_Address + i;
		if( 0 != this->chkAddress(tmp) )
			return 100;
		value = (unsigned short *)(this->pBuf+tmp);
		if( *value == 0 )
		{
			*value = WarringCode;
			return 0;
		}
		else if(*value == WarringCode)
		{
			return 1; //Alarm Code is exist. No Action
		}
	}
	value = (unsigned short *)(this->pBuf+this->gWarring_Address);
    *value = WarringCode;
	
	return 99; //Warring Code SM is Full. Replace the first Warring code
}

int CSMDevice::DelWarring(unsigned short WarringCode)
{
	unsigned short *value;
	unsigned int tmp;

	for(unsigned int i=0; i < this->gWarring_Length; i++)
	{
		tmp = this->gWarring_Address + i;
		value = (unsigned short *)(this->pBuf+tmp);
		if( *value == WarringCode )
		{
			*value = 0;
			return 0;
		}
	}
	
	return 99; //Warring Code is not Found. No Action.
}

int CSMDevice::ClearAllWarring()
{
	memset( (void *)(this->pBuf+this->gWarring_Address), 0, this->gWarring_Length * sizeof(short) );
	return 0; //Set all Warring Code 0.
}

LPTSTR CSMDevice::ReadText(unsigned int address, unsigned short SpecLength)
{
	TCHAR *value = new TCHAR(SpecLength+1);
	if(this->hMapFile == NULL || this->pBuf == NULL)
		return value;
	memset(value, '\0', SpecLength+1);
	memcpy(value, (pBuf+address), SpecLength * sizeof(TCHAR));

	return value;
}

CString CSMDevice::ReadString(unsigned int address, unsigned short SpecLength)
{
	CString value;
	if(this->hMapFile == NULL || this->pBuf == NULL)
		return value;

	TCHAR *tmp = new TCHAR(SpecLength+1);
	memset(tmp, '\0', SpecLength+1);
	memcpy(tmp, (pBuf+address), SpecLength * sizeof(TCHAR));
	value.Format(tmp);

	return value;
}

short CSMDevice::ReadShort(unsigned int address)
{
	short *value;
	if(this->hMapFile == NULL || this->pBuf == NULL)
		return 0;

	value = (short *)(pBuf+address);
	return *value;
}

int CSMDevice::ReadInt(unsigned int address)
{
	int *value;
	if(this->hMapFile == NULL || this->pBuf == NULL)
		return 0;

	value = (int *)(pBuf+address);
	return *value;
}

double CSMDevice::ReadDouble(unsigned int address)
{
	double *value;
	if(this->hMapFile == NULL || this->pBuf == NULL)
		return 0;

	value = (double *)(pBuf+address);
	return *value;
}

void CSMDevice::Close()
{
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
	CloseHandle(file);
}