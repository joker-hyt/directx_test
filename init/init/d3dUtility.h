#include <d3dx9.h>

#define MAX_LOADSTRING 100

namespace d3d
{
	// 全局变量:
	HINSTANCE hInst;								// 当前实例
	TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
	TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

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
};
