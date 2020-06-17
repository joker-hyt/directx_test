#include <d3dx9.h>

#define MAX_LOADSTRING 100
extern IDirect3DDevice9* device;
namespace d3d
{
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	ATOM MyRegisterClass(HINSTANCE hInstance);
	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, int nCmdShow, D3DDEVTYPE deviceType, IDirect3DDevice9** device);

	int EnterMsgLoop(bool (*ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	template<class T>
	void Release(T t)
	{
		if(t)
		{
			t->Release();
			t = 0;
		}
	}

	template<class T>
	void Delete(T t)
	{
		if(t)
		{
			delete t;
			t = 0;
		}
	}

	const D3DCOLOR WHITE(D3DCOLOR_XRGB(255, 255, 255));
	const D3DCOLOR BLACK(D3DCOLOR_XRGB(0, 0, 0));
	const D3DCOLOR RED(D3DCOLOR_XRGB(255, 0, 0));
	const D3DCOLOR GREEN(D3DCOLOR_XRGB(0, 255, 0));
	const D3DCOLOR BLUE(D3DCOLOR_XRGB(0, 0, 255));
	const D3DCOLOR YELLOW(D3DCOLOR_XRGB(255, 255, 0));
	const D3DCOLOR CYAN(D3DCOLOR_XRGB(0, 255, 255));
	const D3DCOLOR MAGENTA(D3DCOLOR_XRGB(255, 0, 255));
};
