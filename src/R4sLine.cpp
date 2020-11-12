/* -------------------------------------------------------------
 *
 *  file:        R4sLine.cpp
 *
 *  description: converts rawdata of 1 pixel line from dtb to Integers
 *
 *  author:      Tilman Rohe
 *  modified:    29.9.2017
 *
 *  rev:        
 *  dat:         
 *
 * -------------------------------------------------------------
 */



#include "cmd.h"
#include "datalink.h"
#include "R4sLine.h"

//#include "pixelAlive.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TCanvas.h"


int R4sLine::Get(int x) {
  if (x>IMG_WIDTH )
    {printf("ERROR: Out of range");return -9999;}

 return data[x]; 
}

bool R4sLine::CreateRaw(const vector<uint16_t> &rawdata)
{
	Clear();
	if (rawdata.size() < IMG_WIDTH) return false;
	data = new int[IMG_WIDTH];

	for (int pos=0; pos<IMG_WIDTH; pos++)
	{
		int value = rawdata[pos];

		if (value & 0x1000) // ADC overrange
			value = R4S_VALUE_OR;
		else if (value & 0x0800) // negative
			value -= 0x1000;

		data[pos] = value;
		//dataNEW.push_back(value);
	}
	return true;
};


void R4sLine::Print(unsigned int count)
{
	if (!data) 
	{
		printf("<<empty>>\n");
		return;
	}

	if (count > IMG_WIDTH) count = IMG_WIDTH;

	for (unsigned int i=0; i<count; i++)
	{
		int v = data[i];
		if (v == R4S_VALUE_OR) printf("   or"); else printf(" %4i", v);
	}
	printf("\n");

	//tb.Daq_Close();
}


void R4sLine::Save(const string &filename)
{

	if (!data) 
	{
		printf("<<empty>>\n");
		return;
	}

	FILE *f = fopen(filename.c_str(), "wt");
	if (!f) return;

	int i = 0, x;

	
	for (x=0; x<IMG_WIDTH; x++)
	{
		int	v = Get(x);
		if (v == R4S_VALUE_OR) fprintf(f, "   or"); else fprintf(f, " %4i", v);
	}
	fputs("\n", f);
	

	fclose(f);
}
