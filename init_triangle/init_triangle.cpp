#include "stdafx.h"
#include "init_triangle.h"
#include "d3dUtility.h"
#include "../mathlib/dMathHeader.h"

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

const int triangle_num = 36;
const int color_num = 3;
const D3DCOLOR color[color_num] = { d3d::RED, d3d::GREEN, d3d::BLUE };

bool Setup()
{
#if 1
	device->CreateVertexBuffer((triangle_num + 1) * sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED, &vb, 0);
	ColorVertex* vertex = NULL;
	vb->Lock(0, 0, (void**)&vertex, 0);

	float x = H3DMath::Tan((2 * D3DX_PI) / (2 * triangle_num));
	D3DXVECTOR3 vec;
	vec.x = -	 x; vec.y = 1.0f; vec.z = 2.0f;
	
	for(int i = 0; i < triangle_num; ++i)
	{
		D3DXMATRIX rotationMatrix;
		D3DXMatrixRotationZ(&rotationMatrix, -2 * D3DX_PI / triangle_num * i);

		D3DXVECTOR3 res_vec;
		D3DXVec3TransformNormal(&res_vec, &vec, &rotationMatrix);

		vertex[i] = ColorVertex(res_vec.x, res_vec.y, res_vec.z, color[i % color_num]);
	}

	vertex[triangle_num] = ColorVertex(0.0f, 0.0f, 2.0f, d3d::WHITE);

	vb->Unlock();
#endif

#if 1
	device->CreateIndexBuffer(triangle_num * 3 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib, 0);
	WORD* indices = NULL;
	ib->Lock(0, 0, (void**)&indices, 0);

	for(int i = 0; i < triangle_num; ++i)
	{
		indices[i * 3 + 0] = (i + 0) % triangle_num;
		indices[i * 3 + 1] = (i + 1) % triangle_num;
		indices[i * 3 + 2] = triangle_num;
	}

	ib->Unlock();
#endif

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2.0f, Width / Height, 1.0f, 1000.0f);
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
	static float accTimeDelta = timeDelta;
	accTimeDelta += timeDelta;
	if(accTimeDelta < 1.0f)
		return true;
	accTimeDelta = 0;

	static int update_times = 0;
	if(device)
	{
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
		device->BeginScene();
		device->SetStreamSource(0, vb, 0, sizeof(ColorVertex));
		device->SetIndices(ib);
		device->SetFVF(ColorVertex::FVF);

		//for(int i = 0; i < triangle_num; ++i)
		{
			D3DXMATRIX worldMatrix;

			D3DXMatrixRotationZ(&worldMatrix, update_times % triangle_num * D3DX_PI / triangle_num);
			device->SetTransform(D3DTS_WORLD, &worldMatrix);
			device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
			//device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, triangle_num);
			device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, triangle_num + 1, 0, triangle_num);
		}

		device->EndScene();
		device->Present(0, 0, 0, 0);
		++update_times;
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
