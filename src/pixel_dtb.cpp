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
    prog[ 0] = 0xe5e5e5e5;
    prog[ 1] = 0xe5e5e5e5;
    prog[ 2] = 0x0fe5e5;
    r4s_SetSequence(prog);
}


void CTestboard::DACScan(int DAC, int start, int stop, int step, std::vector<double> &result)
{
    int old_value=GetDAC(DAC);
    int x,y;
    GetPixCal(x,y);   // could be overwritten in the mean time
    SetPixCal(x,y);
    std::vector<uint16_t> data;
    // programm pixel
    SignalProbeADC(PROBEA_SDATA1, GAIN_1);
    vector<uint32_t> prog(1);
    prog[ 0] = 0x054321;
    r4s_SetSequence(prog);
    r4s_Start();
    uDelay(3000);
    Flush();

    // DAC scan
    uint8_t roMode = 3;
    Daq_Open(50000);

    // prepare ADC
    r4s_AdcDelay(7);
    r4s_Enable(roMode);
    uDelay(400);

    r4s_SetSeqMeasureValue();
    Daq_Start();

    for(int i=start; i<=stop; i+=step)
    {
        SetDAC(DAC, i);
        // take data
        r4s_Start();
        uDelay(3000);
        Flush();
    }

    Daq_Stop();
    // stop ADC
    r4s_Enable(0);
    Daq_Read(data);
    Daq_Close();
    Flush();

    int n=0;
    double mean;
    for(int i=start; i<=stop; i+=step)
    {
        mean=0;
        for (int j=0; j<10; j++) {
            int value = (int) data[10*n+j];
            if ((value & 0x1000) != 0) // ADC overrange
                   value = -5000;
            else if ((value & 0x0800) != 0) // negative
                    value -= 0x1000;
            mean+=(double)value;
        }
        mean=mean/10.0;
        result.push_back(mean);
        n++;
    }
    SetDAC(DAC, old_value);
}

void CTestboard::SetDAC(int DAC, int value)
{
   switch(DAC){
        case VanaN:
           SetVanaN(value);
           break;
        case VanaP:
           SetVanaP(value);
           break;
        case Vdig:
           SetVdig(value);
           break;
        case VDDIO:
           SetVddio(value);
           break;
        case V18:
           SetV18(value);
           break;
        case BiasD:
           SetVbiasD(value);
           break;
        case BiasR:
           SetVbiasR(value);
           break;
        case VcascN:
           SetVcascN(value);
           break;
        case Vn0:
           SetVn0(value);
           break;
        case Vn1:
           SetVn1(value);
           break;
        case Vn2:
           SetVn2(value);
           break;
        case Vfb:
           SetVfb(value);
           break;
        case Vprfb:
           SetVprefb(value);
           break;
        case VcascP:
           SetVcascP(value);
           break;
        case Vp0:
           SetVp0(value);
           break;
        case Vp1:
           SetVp1(value);
           break;
        case Vp2:
           SetVp2(value);
           break;
        case Vcal:
           SetVcal(value);
           break;
        case Hold:
           SetThold(value);
           break;
        case IBiasRO:
           SetIbiasRO(value);
           break;
        case IBiasIO:
           SetIbiasIO(value);
           break;
        case VOffset:
           SetVoffset(value);
           break;
        }
}


int CTestboard::GetDAC(int DAC)
{
    int value=0;
    switch(DAC){
        case VanaN:
           value = GetVanaN();
           break;
        case VanaP:
           value = GetVanaP();
           break;
        case Vdig:
           value = GetVdig();
           break;
        case VDDIO:
           value = GetVddio();
           break;
        case V18:
           value = GetV18();
           break;
        case BiasD:
           value = GetVbiasD();
           break;
        case BiasR:
           value = GetVbiasR();
           break;
        case VcascN:
           value = GetVcascN();
           break;
        case Vn0:
           value = GetVn0();
           break;
        case Vn1:
           value = GetVn1();
           break;
        case Vn2:
           value = GetVn2();
           break;
        case Vfb:
           value = GetVfb();
           break;
        case Vprfb:
           value = GetVprefb();
           break;
        case VcascP:
           value = GetVcascP();
           break;
        case Vp0:
           value = GetVp0();
           break;
        case Vp1:
           value = GetVp1();
           break;
        case Vp2:
           value = GetVp2();
           break;
        case Vcal:
           value = GetVcal();
           break;
        case Hold:
           value = GetThold();
           break;
        case IBiasRO:
           value = GetIbiasRO();
           break;
        case IBiasIO:
           value = GetIbiasIO();
           break;
        case VOffset:
           value = GetVoffset();
           break;
        }
    return value;
}
