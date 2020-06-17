#include "stdafx.h"
#include "init.h"
#include "d3dUtility.h"

IDirect3DDevice9* device;

bool Setup()
{
	//if(device)
	//{
	//	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FF00FF, 1.0f, 0);
	//	device->Present(0, 0, 0, 0);
	//}
	return true;
}

void Cleanup()
{}

bool Display(float timeDelta)
{
#if 0
	if(device)
	{
		device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00FF00FF, 1.0f, 0);
		device->Present(0, 0, 0, 0);
	}
#else
	D3DSURFACE_DESC surface_desc;
	IDirect3DSurface9* _surface;
	device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &_surface);
	_surface->GetDesc(&surface_desc);

	D3DLOCKED_RECT locked_rect;
	_surface->LockRect(&locked_rect, 0, 0);

	DWORD* image_data = (DWORD*)locked_rect.pBits;
	for(int i = 0; i < surface_desc.Height; ++i)
	{
		for(int j = 0; j < surface_desc.Width; ++j)
		{
			int index = i * locked_rect.Pitch / 4 + j;
			image_data[index] = 0xFFFF0000;
		}
	}

	_surface->UnlockRect();
	device->Present(0, 0, 0, 0);
#endif
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
