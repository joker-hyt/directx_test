#include "stdafx.h"
#include "init_cube.h"
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

// 自转速度
const float speed_rotation_x = 1.0f;
const float speed_rotation_y = 2.0f;
const float speed_rotation_z = 3.0f;

// 公转速度
const float speed_revolution_x = 1.0f;
const float speed_revolution_y = 2.0f;
const float speed_revolution_z = 3.0f;

// 初始位置
const float x = 5.0f;
const float y = 5.0f;
const float z = 5.0f;

bool Setup()
{
#if 1
	// 顶点缓存
	device->CreateVertexBuffer(8 * sizeof(ColorVertex), D3DUSAGE_WRITEONLY, ColorVertex::FVF, D3DPOOL_MANAGED, &vb, 0);
	ColorVertex* vertex = NULL;
	vb->Lock(0, 0, (void**)&vertex, 0);

	vertex[0] = ColorVertex(-1.0f, -1.0f, -1.0f, d3d::RED);
	vertex[1] = ColorVertex(-1.0f,  1.0f, -1.0f, d3d::GREEN);
	vertex[2] = ColorVertex( 1.0f,  1.0f, -1.0f, d3d::BLUE);
	vertex[3] = ColorVertex( 1.0f, -1.0f, -1.0f, d3d::YELLOW);
	vertex[4] = ColorVertex(-1.0f, -1.0f,  1.0f, d3d::CYAN);
	vertex[5] = ColorVertex(-1.0f,  1.0f,  1.0f, d3d::MAGENTA);
	vertex[6] = ColorVertex( 1.0f,  1.0f,  1.0f, d3d::BLACK);
	vertex[7] = ColorVertex( 1.0f, -1.0f,  1.0f, d3d::WHITE);

	vb->Unlock();
#endif 

#if 1
	// 索引缓存
	device->CreateIndexBuffer(36 * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &ib, 0);
	WORD* index;
	ib->Lock(0, 0, (void**)&index, 0);

	index[0] = 0; index[1] = 1; index[2] = 2;
	index[3] = 0; index[4] = 2; index[5] = 3;

	index[6] = 1; index[7] = 5; index[8] = 6;
	index[9] = 1; index[10] = 6; index[11] = 2;

	index[12] = 2; index[13] = 6; index[14] = 7;  
	index[15] = 2; index[16] = 7; index[17] = 3;

	index[18] = 3; index[19] = 7; index[20] = 4;  
	index[21] = 3; index[22] = 4; index[23] = 0;

	index[24] = 4; index[25] = 5; index[26] = 1;  
	index[27] = 4; index[28] = 1; index[29] = 0;

	index[30] = 5; index[31] = 4; index[32] = 7;
	index[33] = 5; index[34] = 7; index[35] = 6;

	ib->Unlock();
#endif

	// 观察坐标系
	D3DXVECTOR3 position(-8.0f, 8.0f, -8.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &position, &target, &up);
	device->SetTransform(D3DTS_VIEW, &view);

	// 投影
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI / 2.0f, Width / Height, 1.0f, 1000.0f);
	device->SetTransform(D3DTS_PROJECTION, &proj);

	// 视口
	D3DVIEWPORT9 vp = { 0, 0, 800, 600, 0, 0 };
	device->SetViewport(&vp);

	//device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
	device->SetRenderState(D3DRS_LIGHTING, false);


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
		// 自转
		static float rotation_x = 0.0f;
		static float rotation_y = 0.0f;
		static float rotation_z = 0.0f;
		rotation_x += timeDelta * speed_rotation_x;
		rotation_y += timeDelta * speed_rotation_y;
		rotation_z += timeDelta * speed_rotation_z;
		rotation_x = rotation_x > 2 * D3DX_PI ? rotation_x - 2 * D3DX_PI : rotation_x;
		rotation_y = rotation_y > 2 * D3DX_PI ? rotation_y - 2 * D3DX_PI : rotation_y;
		rotation_z = rotation_z > 2 * D3DX_PI ? rotation_z - 2 * D3DX_PI : rotation_z;
		D3DXMATRIX rotation_x_matrix;
		D3DXMatrixRotationX(&rotation_x_matrix, rotation_x);
		D3DXMATRIX rotation_y_matrix;
		D3DXMatrixRotationY(&rotation_y_matrix, rotation_y);
		D3DXMATRIX rotation_z_matrix;
		D3DXMatrixRotationZ(&rotation_z_matrix, rotation_z);

		// 初始位置
		D3DXMATRIX xyz_matrix;
		D3DXMatrixTranslation(&xyz_matrix, x, y, z);

		// 公转
		static float revolution_x = 0.0f;
		static float revolution_y = 0.0f;
		static float revolution_z = 0.0f;
		revolution_x += timeDelta * speed_revolution_x;
		revolution_y += timeDelta * speed_revolution_y;
		revolution_z += timeDelta * speed_revolution_z;
		revolution_x = revolution_x > 2 * D3DX_PI ? revolution_x - 2 * D3DX_PI : revolution_x;
		revolution_y = revolution_y > 2 * D3DX_PI ? revolution_y - 2 * D3DX_PI : revolution_y;
		revolution_z = revolution_z > 2 * D3DX_PI ? revolution_z - 2 * D3DX_PI : revolution_z;
		D3DXMATRIX revolution_x_matrix;
		D3DXMatrixRotationX(&revolution_x_matrix, revolution_x);
		D3DXMATRIX revolution_y_matrix;
		D3DXMatrixRotationY(&revolution_y_matrix, revolution_y);
		D3DXMATRIX revolution_z_matrix;
		D3DXMatrixRotationZ(&revolution_z_matrix, revolution_z);

		// 世界坐标系
		D3DXMATRIX worldMatrix = rotation_x_matrix * rotation_y_matrix * rotation_z_matrix
			* xyz_matrix
			* revolution_x_matrix * revolution_y_matrix * revolution_z_matrix;
		device->SetTransform(D3DTS_WORLD, &worldMatrix);

		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
		device->BeginScene();
		device->SetStreamSource(0, vb, 0, sizeof(ColorVertex));
		device->SetIndices(ib);
		device->SetFVF(ColorVertex::FVF);
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
