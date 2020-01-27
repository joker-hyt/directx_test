#include "stdafx.h"
#include "init_cube.h"
#include "d3dUtility.h"

IDirect3DDevice9* device;
ID3DXMesh* mesh;
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

const DWORD Vertex::FVF = D3DFVF_XYZ;
const float Width = 100.0f;
const float Height = 80.0f;

bool Setup()
{
	//D3DXCreateTeapot(device, &mesh, 0);

	device->CreateVertexBuffer(8 * sizeof(Vertex), D3DUSAGE_WRITEONLY, Vertex::FVF, D3DPOOL_MANAGED, &vb, 0);
	device->CreateIndexBuffer(36 * sizeof(DWORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib, 0);

	Vertex* vertex;
	vb->Lock(0, 0, (void**)&vertex, 0);
	vertex[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertex[1] = Vertex(-1.0f,  1.0f, -1.0f);
	vertex[2] = Vertex( 1.0f,  1.0f, -1.0f);
	vertex[3] = Vertex( 1.0f, -1.0f, -1.0f);
	vertex[4] = Vertex(-1.0f, -1.0f,  1.0f);
	vertex[5] = Vertex(-1.0f,  1.0f,  1.0f);
	vertex[6] = Vertex( 1.0f,  1.0f,  1.0f);
	vertex[7] = Vertex( 1.0f, -1.0f,  1.0f);
	vb->Unlock();

	DWORD* index;
	ib->Lock(0, 0, (void**)&index, 0);
	index[0] = 0; index[1] = 1; index[2] = 2;
	index[3] = 0; index[4] = 2; index[5] = 3;

	index[6] = 4; index[7] = 6; index[8] = 5;
	index[9] = 4; index[10] = 7; index[11] = 6;

	index[12] = 4; index[13] = 5; index[14] = 1;  
	index[15] = 4; index[16] = 1; index[17] = 0;

	index[18] = 3; index[19] = 2; index[20] = 6;  
	index[21] = 3; index[22] = 6; index[23] = 7;

	index[24] = 1; index[25] = 5; index[26] = 6;  
	index[27] = 1; index[28] = 6; index[29] = 2;

	index[30] = 4; index[31] = 0; index[32] = 3;
	index[33] = 4; index[34] = 3; index[35] = 7;
	ib->Unlock();

	D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3	target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMATRIX v;
	D3DXMatrixLookAtLH(&v, &position, &target, &up);
	device->SetTransform(D3DTS_VIEW, &v);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f, Width / Height, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

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
		D3DXMATRIX rx, ry;
		D3DXMatrixRotationX(&rx, 3.14f / 4.0f);

		static float y = 0.0f;
		D3DXMatrixRotationY(&ry, y);

		y += timeDelta;
		if(y >= 6.28f)
		{
			y = 0.0f;
		}

		D3DXMATRIX p = rx * ry;
		device->SetTransform(D3DTS_WORLD, &p);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
		device->BeginScene();
		device->SetStreamSource(0, vb, 0, sizeof(Vertex));
		device->SetIndices(ib);
		device->SetFVF(Vertex::FVF);
		device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
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
