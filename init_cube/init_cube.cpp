#include "stdafx.h"
#include "init_cube.h"
#include "d3dUtility.h"

IDirect3DDevice9* device;
ID3DXMesh* mesh;

bool Setup()
{
	D3DXCreateTeapot(device, &mesh, 0);
	return true;
}

void Cleanup()
{
	mesh->Release();
	mesh = 0;
}

bool Display(float timeDelta)
{
	if(device)
	{
		device->BeginScene();
		mesh->DrawSubset(0);
		device->EndScene();
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
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
