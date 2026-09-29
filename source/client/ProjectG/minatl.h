// Minimalist stubbed ATL. Though there is no exact known header like this, it
// does seem to reproduce a pattern often seen in the executable, so presumably
// they did do something along these lines somehow.
#include <windows.h>
#include <stdlib.h>

namespace ATL
{
	template <class T>
	class CSimpleArrayEqualHelper
	{
	public:
		static bool IsEqual(const T& t1, const T& t2) { return (t1 == t2); }
	};

	template <class T, class TEqual = CSimpleArrayEqualHelper<T> >
	class CSimpleArray
	{
	public:
		CSimpleArray()
			: m_aT(NULL), m_nSize(0), m_nAllocSize(0)
		{
		}

		~CSimpleArray() { RemoveAll(); }

		int GetSize() const { return m_nSize; }

		void RemoveAll()
		{
			if (m_aT != NULL)
			{
				for (int i = 0; i < m_nSize; i++)
					m_aT[i].~T();
				free(m_aT);
				m_aT = NULL;
			}
			m_nSize = 0;
			m_nAllocSize = 0;
		}

		T& operator[](int nIndex)
		{
			if (nIndex < 0 || nIndex >= m_nSize)
				RaiseException((DWORD)EXCEPTION_ARRAY_BOUNDS_EXCEEDED,
					EXCEPTION_NONCONTINUABLE, 0, NULL);
			return m_aT[nIndex];
		}

		T* m_aT;
		int m_nSize;
		int m_nAllocSize;
	};

	class CComCriticalSection
	{
	public:
		HRESULT Term()
		{
			DeleteCriticalSection(&m_sec);
			return S_OK;
		}

		CRITICAL_SECTION m_sec;
	};

	struct _ATL_WIN_MODULE70
	{
		UINT cbSize;
		CComCriticalSection m_csWindowCreate;
		struct _AtlCreateWndData* m_pCreateWndList;
		CSimpleArray<ATOM> m_rgWindowClassAtoms;
	};

	class CAtlBaseModule
	{
	public:
		HINSTANCE GetModuleInstance() { return m_hInst; }

		UINT cbSize;
		HINSTANCE m_hInst;
	};

	extern CAtlBaseModule _AtlBaseModule;

	HRESULT __stdcall AtlWinModuleTerm(_ATL_WIN_MODULE70* pWinModule,
		HINSTANCE hInst);

	class CAtlWinModule : public _ATL_WIN_MODULE70
	{
	public:
		CAtlWinModule();

		~CAtlWinModule() { Term(); }

		void Term()
		{
			AtlWinModuleTerm(this, _AtlBaseModule.GetModuleInstance());
		}
	};

	__declspec(selectany) CAtlWinModule _AtlWinModule;

	inline HRESULT __stdcall AtlWinModuleTerm(_ATL_WIN_MODULE70* pWinModule,
		HINSTANCE hInst)
	{
		if (pWinModule == NULL)
			return E_INVALIDARG;
		if (pWinModule->cbSize == 0)
			return S_OK;
		if (pWinModule->cbSize != sizeof(_ATL_WIN_MODULE70))
			return E_INVALIDARG;

		for (int i = 0; i < pWinModule->m_rgWindowClassAtoms.GetSize(); i++)
			UnregisterClassA((LPCSTR)pWinModule->m_rgWindowClassAtoms[i],
				hInst);
		pWinModule->m_rgWindowClassAtoms.RemoveAll();
		pWinModule->m_csWindowCreate.Term();
		pWinModule->cbSize = 0;
		return S_OK;
	}
}
