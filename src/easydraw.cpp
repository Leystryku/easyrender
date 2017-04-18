#include "easydraw.h"
#include <d3dx9.h>
#include <DxErr.h>

#include <stdio.h>


Easydraw *easydraw = 0;

Easymaterial::Easymaterial()
{
	d3dtexture = 0;
	drawinst = 0;
	memset(file, 0, sizeof(file));

}


Easymaterial::Easymaterial(Easydraw* draw, const char* filename)
{

	width = 0;
	height = 0;
	d3dtexture = 0;
	drawinst = 0;
	memset(file, 0, sizeof(file));

	if (!draw || !filename)
		return;

	IDirect3DTexture9 *loadedtex = 0;

	HRESULT hr = 0;
	
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)draw->GetDevice();

	D3DCOLOR colorkey = 0;

	D3DXIMAGE_INFO imginfo;

	if (FAILED(hr = D3DXCreateTextureFromFileExA(device, filename, 0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, colorkey, &imginfo, 0, &loadedtex)) || !loadedtex)
	{
		const char *errorString = DXGetErrorString(hr); // Here you get error string
		const char *errorDesc = DXGetErrorDescription(hr); // Here you get error description
		
		printf("[ERROR!] Show this a dev!\nFailed loading texture: %s - %s\n", errorString, errorDesc);

		return;
	}

	D3DSURFACE_DESC desc;
	loadedtex->GetLevelDesc(0, &desc);
	memheight = desc.Height;
	memwidth = desc.Width;
	height = imginfo.Height;
	width = imginfo.Width;

	drawinst = draw;
	strncpy(file, filename, sizeof(file));
	d3dtexture = loadedtex;

}

void Easymaterial::operator = (Easymaterial* mat)
{
	d3dtexture = 0;
	memset(file, 0, sizeof(file));
	drawinst = 0;

	if (!mat)
	{
		return;
	}

	if (!mat->IsValid())
	{
		return;
	}


	strncpy(file, mat->file, sizeof(file));
	drawinst = mat->drawinst;
	d3dtexture = mat->d3dtexture;

}

Easyfont::Easyfont()
{
	drawinst = 0;
	width = 0;
	height = 0;
	weight = 0;
	memset(name, 0, sizeof(name));
	memset(ourname, 0, sizeof(ourname));
}

Easyfont::Easyfont(Easydraw *draw, const char*newfontname, const char *fontname, int32_t fontwidth, int32_t fontheight, int32_t fontweight)
{
	drawinst = 0;
	d3dfont = 0;
	width = 0;
	height = 0;
	weight = 0;
	memset(name, 0, sizeof(name));
	memset(ourname, 0, sizeof(ourname));

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)draw->GetDevice();

	HRESULT hr = 0;

	ID3DXFont* loadfont;

	if (FAILED(hr = D3DXCreateFont(device, fontheight, fontwidth, fontweight, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, fontname, &loadfont)) || !loadfont)
	{
		const char *errorString = DXGetErrorString(hr); // Here you get error string
		const char *errorDesc = DXGetErrorDescription(hr); // Here you get error description

		printf("[ERROR!] Show this a dev!\nFailed loading font: %s - %s\n", errorString, errorDesc);

		return;
	}

	weight = fontweight;
	width = fontwidth;
	height = fontheight;
	drawinst = draw;
	strncpy(name, fontname, sizeof(name));
	strncpy(ourname, newfontname, sizeof(ourname));

	d3dfont = loadfont;
}

void Easyfont::operator = (Easyfont* font)
{
	drawinst = 0;
	d3dfont = 0;
	width = 0;
	height = 0;
	weight = 0;
	memset(name, 0, sizeof(name));
	memset(ourname, 0, sizeof(ourname));

	if (!font)
	{
		return;
	}

	if (!font->IsValid())
	{
		return;
	}

	drawinst = font->drawinst;
	width = font->width;
	height = font->height;
	weight = font->weight;

	strncpy(name, font->name, sizeof(name));
	strncpy(ourname, font->ourname, sizeof(ourname));

	d3dfont = font->d3dfont;

}

class VertexList
{
public:
	FLOAT x, y, z, rhw;
	DWORD color;

	FLOAT tu, tv;
};


Easydraw::Easydraw(void* device)
{
	matbuffer_loaded = 0;
	fontbuffer_loaded = 0;
	
	memset(&matbuffer, 0, sizeof(matbuffer));
	memset(&fontbuffer, 0, sizeof(fontbuffer));

	d3ddevice = device;
	curcol.SetFullWhite();
}

void Easydraw::Test()
{

	curcol.SetFullWhite();
	for (int32_t x = 0; x < 150; x++)
	{
		for (int32_t y = 0; y < 150; y++)
		{
			Pixel(x, y);
		}
	}

	curcol.SetFullRed();

	Line(150+20, 30, 250, 30);

	curcol.SetFullGreen();

	Rectangle(270, 30, 100, 100);


	curcol.SetFullBlue();

	OutlinedRectangle(400, 30, 100, 100, 10);

	Easymaterial *mat = GetMaterial("C:/test.png");
		
	SetMaterial(mat);

	TexturedRectangle(550, 30, mat->GetMemWidth()/4, mat->GetMemHeight()/4);

	Easyfont *font = GetFont("nicefontnig", "arial", 12, 0, FONTWEIGHT_NORMAL);
	SetFont(font);

	curcol.r = 128;
	curcol.g = 200;
	curcol.b = 0;
	curcol.a = 255;

	Text("Bigfazssbatt", 560, 15, DT_NOCLIP);

	Circle(700, 73, 30, 15, 1);


}

void Easydraw::Pixel(int32_t x, int32_t y)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();


	VertexList vertexes[] =
	{
		{ (float)x, (float)y, 0.0f, 1.0f, d3dcol }, //pos
	};

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_POINTLIST, 1, vertexes, sizeof(VertexList));

}
void Easydraw::Line(int32_t startx, int32_t starty, int32_t endx, int32_t endy)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	VertexList vertexes[] =
	{

		{ (float)startx, (float)(starty), 0.0f, 1.0f, d3dcol }, // start pos
		{ (float)endx, (float)(endy), 0.0f, 1.0f, d3dcol }, // end pos
	};

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_LINELIST, 1, vertexes, sizeof(VertexList));

}

void Easydraw::Rectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	float startx = (float)x;
	float starty = (float)y;

	float endx = (float)(startx + width);
	float endy = (float)(starty + height);

	VertexList vertexes[] =
	{
		


		{ startx, starty, 0.0f, 1.0f, d3dcol }, // top left
		{ endx, starty, 0.0f, 1.0f, d3dcol }, // top right
		{ startx, endy, 0.0f, 1.0f, d3dcol }, // bottom left
		{ endx,  endy, 0.0f, 1.0f, d3dcol }, // bottom right

	};

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(VertexList));
}

void Easydraw::OutlinedRectangle(int32_t x, int32_t y, int32_t width, int32_t height, int32_t bordersize)
{
	
	
	Rectangle(x, y, width, bordersize);// top line
	Rectangle(x, y+height-bordersize, width, bordersize);// bottom line
	Rectangle(x, y, bordersize, height);// left line
	Rectangle(x+width-bordersize, y, bordersize, height);// right line



}

void Easydraw::TexturedRectangle(int32_t x, int32_t y, int32_t width, int32_t height)
{
	if (!curmat.IsValid())
	{
		return;
	}
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	float startx = (float)x;
	float starty = (float)y;

	float endx = (float)(startx + width);
	float endy = (float)(starty + height);

	VertexList vertexes[] =
	{



		{ startx, starty, 0.0f, 1.0f, d3dcol, 0.0f, 0.0f}, // top left
		{ endx, starty, 0.0f, 1.0f, d3dcol, 1.0f, 0.0f }, // top right
		{ startx, endy, 0.0f, 1.0f, d3dcol, 0.0f, 1.0f }, // bottom left
		{ endx,  endy, 0.0f, 1.0f, d3dcol ,1.0f, 1.0f }, // bottom right

	};

	device->SetTexture(0, (LPDIRECT3DTEXTURE9)curmat.GetTexture());
	device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertexes, sizeof(VertexList));
}

// this is the only func I didn't make myself because: Fuck drawing circles.
void Easydraw::Circle(int32_t x, int32_t y, int32_t radius, int32_t edgecount, int32_t rotation)
{

	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;
	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	VertexList *verts = new VertexList[edgecount + 1];

	for (int i = 0; i < edgecount + 1; i++)
	{
		verts[i].x = x + radius*cos(D3DX_PI*(i / (edgecount / 2.0f)));
		verts[i].y = y + radius*sin(D3DX_PI*(i / (edgecount / 2.0f)));
		verts[i].z = 0;
		verts[i].rhw = 1;
		verts[i].color = d3dcol;
	}

	device->SetTexture(0, 0);
	device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, edgecount-1, verts, sizeof(VertexList));

	delete[] verts;
}

void Easydraw::Text(const char *text,  int32_t x, int32_t y, int32_t format, easydraw_fontrect* rect)
{
	LPDIRECT3DDEVICE9 device = (LPDIRECT3DDEVICE9)d3ddevice;

	if (!curfont.IsValid())
	{
		return;
	}

	ID3DXFont *font = (ID3DXFont*)curfont.GetFont();
	D3DCOLOR d3dcol = (D3DCOLOR)curcol.GetUint32ARGB();

	RECT FontRect = { x,  y,  x, y };

	if (rect)
	{
		FontRect.left = rect->left;
		FontRect.right = rect->right;
		FontRect.top = rect->top;
		FontRect.bottom = rect->bottom;
	}

	font->DrawText(0, text, -1, &FontRect, format, d3dcol);

}

void Easydraw::SetColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	curcol.r = r;
	curcol.g = g;
	curcol.b = b;
	curcol.a = a;
}

void Easydraw::SetColor(Easycolor color)
{
	curcol = color;
}

void Easydraw::SetMaterial(Easymaterial* mat)
{
	curmat = mat;
}

void Easydraw::SetFont(Easyfont *font)
{
	curfont = font;
}

int32_t Easydraw::GetTextSize(const char*txt, int32_t&w, int32_t& h)
{
	if (!curfont.IsValid())
	{
		w = 0;
		h = 0;
		return 0;
	}

	ID3DXFont *font = (ID3DXFont*)curfont.GetFont();
	RECT rec = { 0,0,0,0 };

	font->DrawText(NULL, txt, -1, &rec, DT_CALCRECT, D3DCOLOR_RGBA(0, 0, 0, 0));

	w = rec.right - rec.left;
	h = rec.bottom - rec.top;

	return 1;
}
Easymaterial* Easydraw::GetMaterial(const char *filename)
{
	for (uint16_t i = 0; i < matbuffer_loaded;i++) // caching TM
	{
		Easymaterial* curmat = matbuffer[i];

		if (!curmat->IsValid())
			continue;

		if (!strcmp(curmat->GetName(), filename))
		{
			return curmat;
		}

	}

	Easymaterial *mat = new Easymaterial(this, filename);

	if (!mat)
	{
		return 0;
	}

	if (!mat->IsValid())
	{
		delete mat;
		return 0;
	}

	matbuffer[matbuffer_loaded] = mat;
	matbuffer_loaded++;

	return mat;
}

Easyfont* Easydraw::GetFont(const char* ourname, const char *fontname, int32_t fontwidth, int32_t fontheight, int32_t fontweight)
{
	for (uint16_t i = 0; i < fontbuffer_loaded; i++) // caching TM
	{
		Easyfont* curfont = fontbuffer[i];

		if (!curfont->IsValid())
			continue;

		if (!strcmp(curfont->GetName(), ourname))
		{
			return curfont;
		}

	}

	Easyfont *font = new Easyfont(this, ourname, fontname, fontwidth, fontheight, fontweight);

	if (!font)
	{
		return 0;
	}

	if (!font->IsValid())
	{
		delete font;
		return 0;
	}

	fontbuffer[fontbuffer_loaded] = font;
	fontbuffer_loaded++;

	return font;
}
