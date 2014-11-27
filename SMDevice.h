#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

class CSMDevice
{
	
	public:
		CSMDevice();
		CSMDevice(LPTSTR SMName);
		virtual ~CSMDevice();
		int Open();
		int WriteShort(unsigned int address, short value);	//Write 16 bit.
		int WriteInt(unsigned int address, int value);				//Write 32 bit.
		int WriteDouble(unsigned int address, double value);		//Write 64 bit.
		int WriteString(unsigned int address, LPTSTR value, unsigned short SpecLength);
		int WriteString(unsigned int address, CString value, unsigned short SpecLength);
		int AddAlarm(unsigned short AlarmCode);
		int DelAlarm(unsigned short AlarmCode);
		int ClearAllAlarm();
		int AddWarring(unsigned short AlarmCode);
		int DelWarring(unsigned short AlarmCode);
		int ClearAllWarring();
		int AddIndex(unsigned int address);

		LPTSTR	ReadText(unsigned int address, unsigned short SpecLength);
		CString ReadString(unsigned int address, unsigned short SpecLength);
		short		ReadShort(unsigned int address);	//Read 16 bit.
		int			ReadInt(unsigned int address);		//Read 32 bit.
		double		ReadDouble(unsigned int address);	//Read 64 bit

		void Close();
	protected:
	private:
		int Initial();
		int chkAddress(unsigned int address);

		//INI File Paramater
		char gSMName[100];
		LPTSTR gSMIni;
		LPTSTR gComm_SMName;
		unsigned long gComm_SMsize;
		unsigned int gAlarm_Address;
        unsigned int gAlarm_Length;
		unsigned int gWarring_Address;
        unsigned int gWarring_Length;

		HANDLE hMapFile;
		unsigned short *pBuf;	//for Device Address Design 16 bit/one Device.
		HANDLE file;
};