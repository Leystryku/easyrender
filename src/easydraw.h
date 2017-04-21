
#include <cstdint>

class Easycolor;
class Easymaterial;
class Easydraw;

class Easycolor
{
public:

	Easycolor()
	{
		r = g = b = a = 0;
	}

	Easycolor(uint8_t red, uint8_t green, uint8_t blue)
	{
		r = red;
		g = green;
		b = blue;
		a = 255;
	}

	Easycolor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
	{
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}

	void operator = (Easycolor col)
	{
		r = col.r;
		g = col.g;
		b = col.b;
		a = col.a;
	}

	void SetFullWhite()
	{
		r = g = b = a = 255;
	}

	void SetFullBlack()
	{
		r = g = b = 0;
		a = 255;
	}

	void SetFullRed()
	{
		r = 255;
		g = 0;
		b = 0;
		a = 255;
	}

	void SetFullGreen()
	{
		r = 0;
		g = 255;
		b = 0;
		a = 255;
	}

	void SetFullBlue()
	{
		r = 0;
		g = 0;
		b = 255;
		a = 255;
	}

	uint32_t GetUint32ARGB()
	{
		uint32_t color = 0;
		color |= (a << 24);
		color |= (r << 16);
		color |= (g << 8);
		color |= (b << 0);

		return color;
	}

	uint32_t GetUint32RGBA()
	{
		uint32_t color = 0;
		color |= (r << 24);
		color |= (g << 16);
		color |= (b << 8);
		color |= (a << 0);

		return color;
	}

	uint8_t r, g, b, a;

};


class Easymaterial
{
private:
	char file[256];
	void* d3dtexture;
	Easydraw *drawinst;
	int32_t width;
	int32_t height;
	int32_t memwidth;
	int32_t memheight;

public:

	Easymaterial();
	Easymaterial(Easydraw *draw, const char* filename);

	void operator = (Easymaterial* mat);

	bool IsValid()
	{
		if (d3dtexture)
			return true;

		return false;
	}

	char* GetName()
	{
		if (!IsValid())
			return 0;

		return file;
	}

	void *GetTexture()
	{
		if (!IsValid())
			return 0;

		return d3dtexture;
	}

	int32_t GetWidth()
	{
		return width;
	}

	int32_t GetHeight()
	{
		return height;
	}

	int32_t GetMemWidth()
	{
		return memwidth;
	}

	int32_t GetMemHeight()
	{
		return memheight;
	}

};

#define DRAWTEXT_TOP                      0x00000000
#define DRAWTEXT_LEFT                     0x00000000
#define DRAWTEXT_CENTER                   0x00000001
#define DRAWTEXT_RIGHT                    0x00000002
#define DRAWTEXT_VCENTER                  0x00000004
#define DRAWTEXT_BOTTOM                   0x00000008
#define DRAWTEXT_WORDBREAK                0x00000010
#define DRAWTEXT_SINGLELINE               0x00000020
#define DRAWTEXT_EXPANDTABS               0x00000040
#define DRAWTEXT_TABSTOP                  0x00000080
#define DRAWTEXT_NOCLIP                   0x00000100
#define DRAWTEXT_EXTERNALLEADING          0x00000200
#define DRAWTEXT_CALCRECT                 0x00000400
#define DRAWTEXT_NOPREFIX                 0x00000800
#define DRAWTEXT_INTERNAL                 0x00001000

#define FONTWEIGHT_DONTCARE         0
#define FONTWEIGHT_THIN             100
#define FONTWEIGHT_EXTRALIGHT       200
#define FONTWEIGHT_LIGHT            300
#define FONTWEIGHT_NORMAL           400
#define FONTWEIGHT_MEDIUM           500
#define FONTWEIGHT_SEMIBOLD         600
#define FONTWEIGHT_BOLD             700
#define FONTWEIGHT_EXTRABOLD        800
#define FONTWEIGHT_HEAVY            900

class Easyfont
{
	void* d3dfont;

	int32_t fontsize;
	int32_t weight;

	char name[256];
	char ourname[256];

	Easydraw *drawinst;

public:
	Easyfont();
	Easyfont(Easydraw *draw, int32_t logpixelsx, const char*ourname, const char *fontname, int32_t fontsize, int32_t fontweight);

	void Easyfont::operator = (Easyfont* font);

	bool IsValid()
	{
		if (!d3dfont)
		{
			return false;
		}

		return true;
	}

	void *GetFont()
	{
		return d3dfont;
	}
	char* GetName()
	{
		return ourname;
	}

	char* GetFontName()
	{
		return name;
	}

	int32_t GetSize()
	{
		return fontsize;
	}
	int32_t GetWeight()
	{
		return weight;
	}
};

struct easydraw_fontrect
{
	int32_t    left;
	int32_t    top;
	int32_t    right;
	int32_t    bottom;
};

class Easydraw
{
private:
	void*d3ddevice;

	Easycolor curcol;
	Easymaterial curmat;
	Easyfont curfont;

	Easymaterial* matbuffer[0xFFFF];
	Easyfont* fontbuffer[0xFFFF];

	uint16_t matbuffer_loaded;
	uint16_t fontbuffer_loaded;

	int32_t logpixelsx;
	int32_t logpixelsy;
public:
	Easydraw(void *device);

	void* GetDevice()
	{
		return d3ddevice;
	}

	void Test();

	void Pixel(int32_t x, int32_t y);
	void Line(int32_t startx, int32_t starty, int32_t endx, int32_t endy);

	void Rectangle(int32_t x, int32_t y, int32_t width, int32_t height);
	void OutlinedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t bordersize = 1);
	void TexturedRectangle(int32_t x, int32_t y, int32_t width, int32_t height);

	void Circle(int32_t x, int32_t y, int32_t radius, int32_t edgecount= 30, int32_t rotation=1);
	void OutlinedCircle(int32_t x, int32_t y, int32_t radius, int32_t edgecount = 30, int32_t rotation = 1);

	void Text(const char *text, int32_t x, int32_t y, int32_t format=0, easydraw_fontrect* rect=0);


	void SetColor(Easycolor col);
	void SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

	void SetFont(Easyfont* font);
	void SetMaterial(Easymaterial* mat);
	
	int32_t GetRawTextSize(const char*text, int32_t&w, int32_t& h);
	int32_t GetTextSize(const char*text, int32_t&w, int32_t& h);

	Easymaterial* GetMaterial(const char *file);
	Easyfont* GetFont(const char*ourname, const char *fontname, int32_t fontsize, int32_t fontweight);
};

extern Easydraw *easydraw;
