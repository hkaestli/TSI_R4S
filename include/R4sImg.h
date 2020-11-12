#define IMG_WIDTH 157
#define IMG_HEIGHT 163
#define R4S_VALUE_OR 4096

class R4sImg
{
  int *data;
  int *dataSum; // sum of images
  int nImgs;    // number of images averaged
  std::vector<int> dataNEW;
  
 public:
 R4sImg() : data(0), dataSum(0), nImgs(1) {}
  bool CreateRaw(const vector<uint16_t> &rawdata);
  void Clear() { if (data) delete[] data; data = 0; }//dataNEW.clear();}
  void Save(const string &filename);
  void Print(unsigned int count);
  int Get(int x, int y);
  int Get(int pos) {return data[pos];}
  void Add(R4sImg &tmp);
  void Substract(R4sImg &tmp);
  void Divide(R4sImg &tmp, int denominator);
  std::vector<int> getData() {return dataNEW;}
};