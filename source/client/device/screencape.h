#pragma once
#include <windows.h>

const DWORD g_screencapeVersion = 20100329u;

namespace Nv
{
	namespace GraphicDeviceType
	{
		enum Enum
		{
			Direct3D8 = 0x0,
			Direct3D9 = 0x1,
		};
	}

	struct IScreenCape
	{
	public:
		virtual bool Initialize(HWND, void**, GraphicDeviceType::Enum) = 0;
		virtual void Release() = 0;
		virtual void Render() = 0;
		virtual void Pause() = 0;
		virtual void Resume() = 0;
	};

	namespace Factory
	{
		typedef HRESULT(__stdcall* CreateScreenCapeLayerProc)(
			IScreenCape** pInterface, DWORD dwVersion);

		class ScreenCapeFactory
		{
		public:
			ScreenCapeFactory()
			{
				this->m_pProc = 0;
				this->m_hModule = LoadLibraryA("ScreenCape.dll");
				if (!this->m_hModule)
				{
					MessageBoxW(0, L"Load failed 'ScreenCape.dll'", L"Warning!",
						0);
				}
				else
				{
					this->m_pProc = reinterpret_cast<CreateScreenCapeLayerProc>(
						GetProcAddress(this->m_hModule, "CreateScreenCape"));
				}
			}

			~ScreenCapeFactory()
			{
				if (this->m_hModule)
				{
					FreeLibrary(this->m_hModule);
				}
			}

			IScreenCape* CreateScreenCapeLayer()
			{
				IScreenCape* pInterface = 0;

				if (m_pProc)
					m_pProc(&pInterface, g_screencapeVersion);
				return pInterface;
			}

			HINSTANCE m_hModule;
			CreateScreenCapeLayerProc m_pProc;
		};
	}
}
