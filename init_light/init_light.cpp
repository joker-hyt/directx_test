#include "stdafx.h"
#include "init_light.h"
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

const DWORD Vertex::FVF = D3DFVF_XYZ;
const DWORD ColorVertex::FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE;
const DWORD NormalVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;
const float Width = 800.0f;
const float Height = 600.0f;

bool Setup()
{
	device->CreateVertexBuffer(12 * sizeof(NormalVertex), D3DUSAGE_WRITEONLY, NormalVertex::FVF, D3DPOOL_MANAGED, &vb, 0);
	
	NormalVertex* vertex;
	vb->Lock(0, 0, (void**)&vertex, 0);
	vertex[ 0] = NormalVertex(-1.0f,  0.0f, -1.0f,  0.0f,  0.707f, -0.707f);
	vertex[ 1] = NormalVertex( 0.0f,  1.0f,  0.0f,  0.0f,  0.707f, -0.707f);
	vertex[ 2] = NormalVertex( 1.0f,  0.0f, -1.0f,  0.0f,  0.707f, -0.707f);

	vertex[ 3] = NormalVertex(-1.0f,  0.0f,  1.0f, -0.707f,  0.707f,  0.0f);
	vertex[ 4] = NormalVertex( 0.0f,  1.0f,  0.0f, -0.707f,  0.707f,  0.0f);
	vertex[ 5] = NormalVertex(-1.0f,  0.0f, -1.0f, -0.707f,  0.707f,  0.0f);

	vertex[ 6] = NormalVertex( 1.0f,  0.0f, -1.0f,  0.707f,  0.707f,  0.0f);
	vertex[ 7] = NormalVertex( 0.0f,  1.0f,  0.0f,  0.707f,  0.707f,  0.0f);
	vertex[ 8] = NormalVertex( 1.0f,  0.0f,  1.0f,  0.707f,  0.707f,  0.0f);

	vertex[ 9] = NormalVertex( 1.0f,  0.0f,  1.0f,  0.0f,  0.707f,  0.707f);
	vertex[10] = NormalVertex( 0.0f,  1.0f,  0.0f,  0.0f,  0.707f,  0.707f);
	vertex[11] = NormalVertex(-1.0f,  0.0f,  1.0f,  0.0f,  0.707f,  0.707f);
	vb->Unlock();

	D3DMATERIAL9 mtrl;
	mtrl.Ambient = d3d::WHITE;
	mtrl.Diffuse = d3d::WHITE;
	mtrl.Specular = d3d::WHITE;
	mtrl.Emissive = d3d::BLACK;
	mtrl.Power = 5.0f;
	device->SetMaterial(&mtrl);

	D3DLIGHT9 dir;
	ZeroMemory(&dir, sizeof(dir));
	dir.Type = D3DLIGHT_DIRECTIONAL;
	dir.Diffuse = d3d::WHITE;
	dir.Specular = d3d::WHITE * 0.3f;
	dir.Ambient = d3d::WHITE * 0.6f;
	dir.Direction = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	device->SetLight(0, &dir);
	device->LightEnable(0, true);

	device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	device->SetRenderState(D3DRS_SPECULARENABLE, true);
	device->SetRenderState(D3DRS_LIGHTING, true);
	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	//device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
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
		device->SetStreamSource(0, vb, 0, sizeof(NormalVertex));
		device->SetFVF(NormalVertex::FVF);
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
