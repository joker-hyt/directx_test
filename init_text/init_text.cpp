#include "stdafx.h"
#include "init_text.h"
#include "d3dUtility.h"

IDirect3DDevice9* device;
IDirect3DVertexBuffer9* vb;
IDirect3DIndexBuffer9* ib;
IDirect3DTexture9* text;

struct Vertex
{
	Vertex() {}
	Vertex(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{}

	float x, y, z;
	static const DWORD FVF;
};

struct ColorVertex
{
	float x, y, z;
	D3DXCOLOR color;
	static const DWORD FVF;
};

struct NormalVertex
{
	NormalVertex() {}
	NormalVertex(float _x, float _y, float _z, float _nx, float _ny, float _nz)
		: x(_x), y(_y), z(_z), nx(_nx), ny(_ny), nz(_nz)
	{}

	float x, y, z;
	float nx, ny, nz;
	static const DWORD FVF;
};

struct TextVertex
{
public:
	TextVertex() {}
	TextVertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _u, float _v)
		: x(_x), y(_y), z(_z), nx(_nx), ny(_ny), nz(_nz), u(_u), v(_v)
	{}

	float x, y, z;
	float nx, ny, nz;
	float u, v;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ;
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD NormalVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
const DWORD TextVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
const float Width = 800.0f;
const float Height = 600.0f;

bool Setup()
{
	device->CreateVertexBuffer(6 * sizeof(TextVertex), D3DUSAGE_WRITEONLY, TextVertex::FVF, D3DPOOL_MANAGED, &vb, 0);
	
	TextVertex* vertex;
	vb->Lock(0, 0, (void**)&vertex, 0);
	vertex[ 0] = TextVertex(-1.0f, -1.0f,  1.25f,  0.0f,  0.0f, -1.0f,  0.0f,  3.0f);
	vertex[ 1] = TextVertex(-1.0f,  1.0f,  1.25f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f);
	vertex[ 2] = TextVertex( 1.0f,  1.0f,  1.25f,  0.0f,  0.0f, -1.0f,  3.0f,  0.0f);

	vertex[ 3] = TextVertex(-1.0f, -1.0f,  1.25f,  0.0f,  0.0f, -1.0f,  0.0f,  3.0f);
	vertex[ 4] = TextVertex( 1.0f,  1.0f,  1.25f,  0.0f,  0.0f, -1.0f,  3.0f,  0.0f);
	vertex[ 5] = TextVertex( 1.0f, -1.0f,  1.25f,  0.0f,  0.0f, -1.0f,  3.0f,  3.0f);
	vb->Unlock();

	HRESULT hr = D3DXCreateTextureFromFile(device, _T("text.jpg"), &text);
	device->SetTexture(0, text);

	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

	//device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//device->SetRenderState(D3DRS_SPECULARENABLE, true);
	device->SetRenderState(D3DRS_LIGHTING, false);
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	//device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	D3DXVECTOR3 position(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3	target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);
	device->SetTransform(D3DTS_VIEW, &v);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, Width / Height, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	return true;
}

void Cleanup()
{
	d3d::Release(vb);
	d3d::Release(ib);
}

bool Display(float timeDelta)
{
	if(device)
	{
		D3DXMATRIX rx, ry;
		D3DXMatrixRotationX(&rx, 3.14f / 4.0f);

		static float y = 0.0f;
		D3DXMatrixRotationY(&ry, y);

		y += timeDelta;
		if(y >= 6.28f)
		{
			y = 0.0f;
		}

		D3DXMATRIX p = ry;//rx * ry;
		device->SetTransform(D3DTS_WORLD, &p);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
		device->BeginScene();
		device->SetStreamSource(0, vb, 0, sizeof(TextVertex));
		device->SetFVF(TextVertex::FVF);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);
		device->EndScene();
		device->Present(0, 0, 0, 0);
	}
	return true;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if(!d3d::InitD3D(hInstance, 1000, 800, true, nCmdShow, D3DDEVTYPE_HAL, &device))
	{
		MessageBox(0, _T("InitD3D FAILED"), 0, 0);
		return 0;
	}

	if(!Setup())
	{
		MessageBox(0, _T("Setup FAILED"), 0, 0);
		return 0;
	}
	d3d::EnterMsgLoop(Display);
	Cleanup();

	d3d::Release(device);

	return 0;
}
