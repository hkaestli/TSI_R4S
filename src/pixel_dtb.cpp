// psi46_tb.cpp

#include "pixel_dtb.h"
#include <stdio.h>

#ifndef _WIN32
#include <unistd.h>
#include <iostream>
#endif


bool CTestboard::RpcLink(bool verbose)
{
	bool error = false;
	for (unsigned short i = 2; i < rpc_cmdListSize; i++)
	{
		try
		{
			rpc_GetCallId(i);
		}
		catch (CRpcError &e)
		{
			e.SetFunction(0);
			if (verbose)
			{
				if (!error) printf("\nMissing DTB functions:\n");
				std::string fname(rpc_cmdName[i]);
				std::string fname_pretty;
				rpc_TranslateCallName(fname, fname_pretty);
				printf("%s\n", fname_pretty.c_str());
			}
			error = true;
		}
	}
	return !error;
}


bool CTestboard::EnumNext(string &name)
{
	char s[64];
	if (!usb.EnumNext(s)) return false;
	name = s;
	return true;
}


bool CTestboard::Enum(unsigned int pos, string &name)
{
	char s[64];
	if (!usb.Enum(s, pos)) return false;
	name = s;
	return true;
}


bool CTestboard::FindDTB(string &usbId)
{
	string name;
	vector<string> devList;
	unsigned int nDev;
	unsigned int nr;

	try
	{
		if (!EnumFirst(nDev)) throw int(1);
		for (nr=0; nr<nDev; nr++)
		{
			if (!EnumNext(name)) continue;
			if (name.size() < 4) continue;
			if (name.compare(0, 4, "DTB_") == 0) devList.push_back(name);
		}
	}
	catch (int e)
	{
		switch (e)
		{
		case 1: printf("Cannot access the USB driver\n"); return false;
		default: return false;
		}
	}

	if (devList.size() == 0)
	{
		printf("No DTB connected.\n");
		return false;
	}

	if (devList.size() == 1)
	{
		usbId = devList[0];
		return true;
	}

	// If more than 1 connected device list them
	printf("\nConnected DTBs:\n");
	for (nr=0; nr<devList.size(); nr++)
	{
		printf("%2u: %s", nr, devList[nr].c_str());
		if (Open(devList[nr], false))
		{
			try
			{
				unsigned int bid = GetBoardId();
				printf("  BID=%2u\n", bid);
			}
			catch (...)
			{
				printf("  Not identifiable\n");
			}
			Close();
		}
		else printf(" - in use\n");
	}

	printf("Please choose DTB (0-%u): ", (nDev-1));
	char choice[8];
	fgets(choice, 8, stdin);
	sscanf (choice, "%d", &nr);
	if (nr >= devList.size())
	{
		nr = 0;
		printf("No DTB opened\n");
		return false;
	}

	usbId = devList[nr];
	return true;
}


bool CTestboard::Open(string &usbId, bool init)
{
	rpc_Clear();
	if (!usb.Open(&(usbId[0]))) return false;

	if (init) Init();
	return true;
}


void CTestboard::Close()
{
//	if (usb.Connected()) Daq_Close();
	usb.Close();
	rpc_Clear();
}


void CTestboard::mDelay(uint16_t ms)
{
	Flush();
#ifdef _WIN32
	Sleep(ms);			// Windows
#else
	usleep(ms*1000);	// Linux
#endif
}


void CTestboard::r4s_SetPixCal(uint8_t x, uint8_t y)
{
	vector<uint32_t> regx(5);
	vector<uint32_t> regy(5);
	
	unsigned int wpos, bpos;

	wpos = x / 32; bpos = 1 << (x % 32);
	if (wpos < 5) regx[wpos] = bpos;

	wpos = y / 32; bpos = 1 << (y % 32);
	if (wpos < 5) regy[wpos] = bpos;

	r4s_SetRegX(regx);
	r4s_SetRegY(regy);
}


void CTestboard::r4s_SetSeqMeasure()
{
	vector<uint32_t> prog(5);
	prog[ 0] = 0xf2154321;
	prog[ 1] = 0x88888887;
	prog[ 2] = 0x88888888;
	prog[ 3] = 0x88888888;
	prog[ 4] = 0x08888888;
	r4s_SetSequence(prog);
}


void CTestboard::r4s_SetSeqReadout()
{
	vector<uint32_t> prog(6);
	prog[ 0] = 0x68686868;
	prog[ 1] = 0x68686868;
	prog[ 2] = 0x68686868;
	prog[ 3] = 0x68686868;
	prog[ 4] = 0x68686868;
	prog[ 5] = 0x08686868;
	r4s_SetSequence(prog);
}


void CTestboard::r4s_SetSeqMeasureReadout()
{
    vector<uint32_t> prog(7);
	prog[ 0] = 0xf2154321;
	prog[ 1] = 0x68686867;
	prog[ 2] = 0x68686868;
	prog[ 3] = 0x68686868;
	prog[ 4] = 0x68686868;
	prog[ 5] = 0x68686868;
    prog[ 6] = 0x00006868;
    r4s_SetSequence(prog);
 }


void CTestboard::r4s_SetSeqMeasureColumnReadout()
{
        vector<uint32_t> prog(12);
        prog[ 0] = 0xf2154321;
        prog[ 1] = 0xbababab9;
        prog[ 2] = 0xbabababa;
        prog[ 3] = 0xbabababa;
        prog[ 4] = 0xbabababa;
        prog[ 5] = 0xbabababa;
        prog[ 6] = 0xbabababa;
        prog[ 7] = 0xbabababa;
        prog[ 8] = 0xbabababa;
        prog[ 9] = 0xbabababa;
        prog[10] = 0xbabababa;
        prog[11] = 0x0000baba;
        r4s_SetSequence(prog);
}



void CTestboard::r4s_SetSeqCalScan()
{
    vector<uint32_t> prog(15);
	prog[ 0] = 0x86153721;
	prog[ 1] = 0x15386153; prog[ 2] = 0x38615386; prog[ 3] = 0x61538615; prog[ 4] = 0x53861538;	prog[ 5] = 0x86153861;
	prog[ 6] = 0x15386153; prog[ 7] = 0x38615386; prog[ 8] = 0x61538615; prog[ 9] = 0x53861538;	prog[10] = 0x86153861;
	prog[11] = 0x15386153; prog[12] = 0x38615386; prog[13] = 0x61538615; prog[14] = 0x00061538;
	r4s_SetSequence(prog);

	vector<uint32_t> calx(5, 0xffffffff);
	r4s_SetRegX(calx);

}

void CTestboard::r4s_SetSeqMeasureValue()
{
    vector<uint32_t> prog(3);
    prog[ 0] = 0xefefefe5;
    prog[ 1] = 0xefefefef;
    prog[ 2] = 0x0fefef;
    r4s_SetSequence(prog);
}

void CTestboard::r4s_SetSeqTest(int xy)
{
    r4s_SetPixCal(2,2);
    vector<uint32_t> prog(2);
 /*
    if(xy==0) prog[ 0] = 0xfffff321;
    else prog[ 0] = 0xfffff421;
    prog[ 1] = 0xffffffff;
    */
    prog[ 0] = 0x054321;
    prog[ 1] = 0x00000000;
    r4s_SetSequence(prog);
}
