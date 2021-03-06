// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>
#include <strsafe.h>
#include <time.h>
// TODO:  在此处引用程序需要的其他头文件
#include <d3d12.h>
#pragma comment(lib,"d3d12.lib")
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include <string>
#include <wrl.h>
#include <shellapi.h>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define SAFE_DELETE(p){ if((p)){ delete(p);(p)=NULL;}}