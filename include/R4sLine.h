#define IMG_WIDTH 157
#define IMG_HEIGHT 163
#define R4S_VALUE_OR 4096

class R4sLine
{
  int *data;
  //std::vector<int> dataNEW;
  
 public:
 R4sLine() : data(0) {}
  bool CreateRaw(const vector<uint16_t> &rawdata);
  void Clear() { if (data) delete[] data; data = 0; }//dataNEW.clear();}
  void Save(const string &filename);
  void Print(unsigned int count);
  int Get(int x);
  //std::vector<int> getData() {return dataNEW;}
};
