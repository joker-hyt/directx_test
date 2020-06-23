#include "stdafx.h"
#include "init_color.h"
#include "d3dUtility.h"

IDirect3DDevice9* device;
IDirect3DVertexBuffer9* vb;
IDirect3DIndexBuffer9* ib;

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
	ColorVertex() {}
	ColorVertex(float _x, float _y, float _z, D3DCOLOR _color)
		: x(_x), y(_y), z(_z), color(_color)
	{}

	float x, y, z;
	D3DCOLOR color;
	static const DWORD FVF;
};

const DWORD Vertex::FVF = D3DFVF_XYZ;
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const float Width = 800.0f;
const float Height = 600.0f;

bool Setup()
{
	device->CreateVertexBuffer(3 * sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED, &vb, 0);
	//device->CreateIndexBuffer(36 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib, 0);

	ColorVertex* vertex;
	vb->Lock(0, 0, (void**)&vertex, 0);
	vertex[0] = ColorVertex(-1.0f, 0.0f, 2.0f, d3d::RED);
	vertex[1] = ColorVertex( 0.0f, 1.0f, 2.0f, d3d::GREEN);
	vertex[2] = ColorVertex( 1.0f, 0.0f, 2.0f, d3d::BLUE);
	vb->Unlock();

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, Width / Height, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	device->SetRenderState(D3DRS_LIGHTING, false);

	return true;
}

void Cleanup()
{
	//mesh->Release();
	//mesh = 0;
	d3d::Release(vb);
	d3d::Release(ib);
}

bool Display(float timeDelta)
{
	if(device)
	{
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
		device->BeginScene();
		device->SetStreamSource(0, vb, 0, sizeof(ColorVertex));
		device->SetFVF(ColorVertex::FVF);

		D3DXMATRIX worldMatrix;

		D3DXMatrixTranslation(&worldMatrix, -1.25f, 0.0f, 0.0f);
		device->SetTransform(D3DTS_WORLD, &worldMatrix);
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		D3DXMatrixTranslation(&worldMatrix, 1.25f, 0.0f, 0.0f);
		device->SetTransform(D3DTS_WORLD, &worldMatrix);
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

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
