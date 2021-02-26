/* -------------------------------------------------------------
 *
 *  file:        command.cpp
 *
 *  description: command line interpreter for Chip/Wafer tester
 *
 *  author:      Beat Meier
 *  modified:    31.8.2007
 *
 *  rev:
 *
 * -------------------------------------------------------------
 */


#include "cmd.h"
#include "mainwindow.h"

#define IMG_WIDTH 42 // 157 // 42
#define IMG_HEIGHT 23 // 163 // 23
#define R4S_VALUE_OR 4096


class R4sImg
{
	int *data;
public:
	R4sImg() : data(0) {}
	bool CreateRaw(const vector<uint16_t> &rawdata);
	void Clear() { if (data) delete[] data; data = 0; }
	void Save(FILE *f);
	void Save(const string &filename);
	void Print(unsigned int count);
	int Get(int x, int y) { return data[y*IMG_WIDTH + x]; }
};


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


void R4sImg::Save(FILE *f)
{
    int x, y;

	for (y = IMG_HEIGHT - 1; y >= 0; y--)
	{
		for (x = 0; x<IMG_WIDTH; x++)
		{
			int	v = Get(x, y);
			if (v == R4S_VALUE_OR) fprintf(f, "   or"); else fprintf(f, " %4i", v);
		}
		fputs("\n", f);
	}
}


void R4sImg::Save(const string &filename)
{
	FILE *f = fopen(filename.c_str(), "wt");
	if (!f) return;

	Save(f);

	fclose(f);
}

void SetDAC(int DAC, int value);

void ReadImage(R4sImg &map)
{
	tb.Daq_Open(50000);

	// prepare ADC
	tb.SignalProbeADC(PROBEA_SDATA1, GAIN_1);
//	tb.r4s_AdcDelay(0);
	tb.r4s_Enable(3); // slow readout
	tb.uDelay(100);

	// take data
	tb.Daq_Start();
	tb.r4s_Start();
	tb.uDelay(3000);
	tb.Daq_Stop();

	// stop ADC
	tb.r4s_Enable(0);

	// read buffer
	vector<uint16_t> data;
	unsigned int ret = tb.Daq_Read(data);

	printf("--- status = %u; n = %u\n", ret, (unsigned int)(data.size()));

	tb.Daq_Close();

	map.CreateRaw(data);
}


CMD_PROC(seqstart)
{
	// Eigene Sequenz ins DTB laden (Beispiel)
	vector<uint32_t> prog(5);
	prog[0] = 0xF1F1F1F1;
	prog[1] = 0xF2F2F2F2;
	prog[2] = 0x00000000;
	prog[3] = 0x00000000;
	prog[4] = 0x00000000;
	tb.r4s_SetSequence(prog);

	// Sequenz starten
	tb.r4s_Start();
	DO_FLUSH
}


CMD_PROC(getimg)
{
	R4sImg map;
	ReadImage(map);
	map.Print(200);
	map.Save("map.txt");
}


CMD_PROC(seqreadout)
{
    tb.r4s_SetSeqMeasureReadout();
    DO_FLUSH
}


CMD_PROC(seqcolreadout)
{
    tb.r4s_SetSeqMeasureColumnReadout();
    DO_FLUSH
}


CMD_PROC(seqcalscan)
{
    tb.r4s_SetSeqCalScan();
    DO_FLUSH
}


CMD_PROC(xscan)
{
    vector<uint32_t> prog(2);
    prog[ 0] = 0x0ffff321;
    prog[ 1] = 0xffffffff;
    tb.r4s_SetSequence(prog);

    vector<uint32_t> calx(5, 0x00000000);
    for(int column=0; column<40; column++){
       tb.SetPixCal(column, 0);
       printf("Column %d\n",column);
       tb.r4s_Start();
       tb.mDelay(50);
       DO_FLUSH
    }
}
CMD_PROC(yscan)
{
    vector<uint32_t> prog(2);
    prog[ 0] = 0x0ffff421;
    prog[ 1] = 0xffffffff;
    tb.r4s_SetSequence(prog);

    for(int row=0; row<20; row++){
       tb.SetPixCal(0,row);
       printf("Row %d\n",row);
       tb.r4s_Start();
       tb.mDelay(50);
       DO_FLUSH
    }
}


CMD_PROC(dacscan)
{
    std::map<int,double> result;
    int DAC,start, stop, step;
    PAR_INT(DAC, 0, 21);
    PAR_INT(start, 0, 10000);
    PAR_INT(stop, 0, 10000);
    PAR_INT(step, -10000, 10000 );
    tb.DACScan(DAC, start, stop, step, result);

    for(int i=start; i<=stop; i+=step)
    {
        printf("%f\n",result[i]);
    }
}

CMD_PROC(dacdacscan)
{
    std::vector<double> result;
    int DAC1,start1, stop1, step1;
    int DAC2,start2, stop2, step2;
    PAR_INT(DAC1, 0, 21);
    PAR_INT(start1, 0, 10000);
    PAR_INT(stop1, 0, 10000);
    PAR_INT(step1, -10000, 10000 );
    PAR_INT(DAC2, 0, 21);
    PAR_INT(start2, 0, 10000);
    PAR_INT(stop2, 0, 10000);
    PAR_INT(step2, -10000, 10000 );
    tb.DACDACScan(DAC1, start1, stop1, step1,
               DAC2, start2, stop2, step2, result);

    int nbins2=(stop2-start2)/step2+1;
    int a=0;
    int b=0;
    for(int i=start1; i<=stop1; i+=step1)
    {
        b=0;
        for(int j=start2; j<=stop2; j+=step2)
        {
            printf("%d  %d: %f\n",i, j, result[b+a*nbins2]);
            b++;
        }
        a++;
        printf("\n");
    }
}

CMD_PROC(gui)
{
    ((MyQApplication*)qApp)->Init();
    ((MyQApplication*)qApp)->Show();
	printf("Connection to GUI closed.\n");
}


CMD_PROC(takedata)
{
	int n;
	PAR_INT(n, 1, 100000);
	
	FILE *f = fopen("r4s_events.txt", "wt");
	if (!f) return;

	fprintf(f, "#event = %i\n", n);
	
	R4sImg map;
	tb.Daq_Open(50000);

	// prepare ADC
	tb.SignalProbeADC(PROBEA_SDATA1, GAIN_1);
	tb.r4s_Enable(3); // slow readout
	tb.uDelay(100);

	vector<uint16_t> data;

	for (int i = 0; i < n; i++)
	{

		// --- take data
		tb.Daq_Start();
		tb.r4s_Start();
		tb.uDelay(3000 + int(tspan*0.00625));
		tb.Daq_Stop();

		// read DTB data buffer
        tb.Daq_Read(data);

		map.CreateRaw(data);

		fprintf(f, "event = %i\n", i+1);
		map.Save(f);

		if ((i % 1000) == 999) printf("%i\n", i + 1);

		if (keypressed()) break;
	}
	
	// stop ADC
	tb.r4s_Enable(0);

	tb.Daq_Close();
	fclose(f);

	DO_FLUSH
}


