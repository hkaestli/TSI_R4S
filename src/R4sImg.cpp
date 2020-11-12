/* -------------------------------------------------------------
 *
 *  file:        R4sImg.cpp
 *
 *  description: Converts rawdata from dtb into a "readable" Image
 *
 *  author:      Beat Meier
 *  modified:    31.8.2007
 *
 *  rev:         Tilman Rohe
 *  dat:         28.08.2017 
 *
 * -------------------------------------------------------------
 */




#include "cmd.h"
#include "datalink.h"
#include "R4sImg.h"

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




int R4sImg::Get(int x, int y) {

  if ( y>IMG_HEIGHT || x>IMG_WIDTH )
    {printf("ERROR: Out of range");return -9999;}

 return data[y*IMG_WIDTH + x]; 
}

bool R4sImg::CreateRaw(const vector<uint16_t> &rawdata)
{
	Clear();
	if (rawdata.size() < IMG_WIDTH*IMG_HEIGHT) return false;
	data = new int[IMG_WIDTH*IMG_HEIGHT];

	for (int pos=0; pos<IMG_WIDTH*IMG_HEIGHT; pos++)
	{
		int value = rawdata[pos];

		if (value & 0x1000) // ADC overrange
			value = R4S_VALUE_OR;
		else if (value & 0x0800) // negative
			value -= 0x1000;

		data[pos] = value;
		dataNEW.push_back(value);
	}
	return true;
};


void R4sImg::Print(unsigned int count)
{
	if (!data) 
	{
		printf("<<empty>>\n");
		return;
	}

	if (count > IMG_WIDTH*IMG_HEIGHT) count = IMG_WIDTH*IMG_HEIGHT;

	for (unsigned int i=0; i<count; i++)
	{
		int v = data[i];
		if (v == R4S_VALUE_OR) printf("   or"); else printf(" %4i", v);
	}
	printf("\n");

	tb.Daq_Close();
}


void R4sImg::Save(const string &filename)
{

	if (!data) 
	{
		printf("<<empty>>\n");
		return;
	}

	FILE *f = fopen(filename.c_str(), "wt");
	if (!f) return;

	int i = 0, x, y;

	for (y=IMG_HEIGHT-1; y>=0; y--)
	{
		for (x=0; x<IMG_WIDTH; x++)
		{
			int	v = Get(x,y);
			if (v == R4S_VALUE_OR) fprintf(f, "   or"); else fprintf(f, " %4i", v);
		}
		fputs("\n", f);
	}

	fclose(f);
}

void R4sImg::Add(R4sImg &tmp){
    
  	for (int pos=0; pos<IMG_WIDTH*IMG_HEIGHT; pos++)
	{          
            data[pos] += tmp.Get(pos);
	}
 
    return;
}
void R4sImg::Substract(R4sImg &tmp){
    
  	for (int pos=0; pos<IMG_WIDTH*IMG_HEIGHT; pos++)
	{          
            data[pos] -= tmp.Get(pos);
	}
 
    return;
}
void R4sImg::Divide(R4sImg &tmp, int denominator){
    
  	for (int pos=0; pos<IMG_WIDTH*IMG_HEIGHT; pos++)
	{          
            data[pos] /= denominator;
	}
 
    return;
}
