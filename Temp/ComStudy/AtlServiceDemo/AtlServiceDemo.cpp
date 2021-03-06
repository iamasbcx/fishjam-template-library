// AtlServiceDemo.cpp : Implementation of WinMain


#include "stdafx.h"
#include "resource.h"
#include "AtlServiceDemo_i.h"
#include <stdio.h>
#include <ftlbase.h>

class CAtlServiceDemoModule : public CAtlServiceModuleT< CAtlServiceDemoModule, IDS_SERVICENAME >
{
public :
	DECLARE_LIBID(LIBID_AtlServiceDemoLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLSERVICEDEMO, "{80E054F5-8629-42B1-8CD2-05C39F467BAB}")
	HRESULT InitializeSecurity() throw()
	{
		// TODO : Call CoInitializeSecurity and provide the appropriate security settings for 
		// your service
		// Suggested - PKT Level Authentication, 
		// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
		// and an appropiate Non NULL Security Descriptor.
        HRESULT hr = E_FAIL;
        
        COM_VERIFY(CoInitializeSecurity( NULL, -1, NULL, NULL, 
            RPC_C_AUTHN_LEVEL_NONE, RPC_C_IMP_LEVEL_IDENTIFY, NULL, EOAC_NONE, NULL));

		return hr;
	}

    HRESULT RegisterAppId(bool bService = false) throw()
    {
        HRESULT hr = E_FAIL;
        BOOL bRet = FALSE;

        COM_VERIFY(__super::RegisterAppId(bService));
        if (bService)
        {
            if (IsInstalled())
            {
                SC_HANDLE hSCM = ::OpenSCManagerW(NULL, NULL, SERVICE_CHANGE_CONFIG);
                if (hSCM)
                {
                    SC_HANDLE hService = ::OpenService(hSCM, m_szServiceName, SERVICE_CHANGE_CONFIG);
                    if (hService)
                    {
                        API_VERIFY(ChangeServiceConfig(hService, SERVICE_NO_CHANGE, 
                            SERVICE_DEMAND_START, // 修改服务为手动启动
                            NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                            m_szServiceName)); // 通过修改资源IDS_SERVICENAME 修改服务的显示名字

                        SERVICE_DESCRIPTION Description = {0};
                        TCHAR  szDescription[1024] = {0};
                        StringCchCopy(szDescription, _countof(szDescription), TEXT("Atl Service Demo, copy right fishjam")); 
                        Description.lpDescription = szDescription;

                        API_VERIFY(ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &Description));
                        API_VERIFY(CloseServiceHandle(hService));
                    }
                    else
                    {
                        hr = AtlHresultFromLastError();
                    }

                    API_VERIFY(CloseServiceHandle(hSCM));
                }
                else
                {
                    hr = AtlHresultFromLastError();
                }
            }
        }
        COM_VERIFY(hr);

        return hr;
    }

    HRESULT PreMessageLoop(int nShowCmd) throw()
    {
        HRESULT hr = S_OK;
        COM_VERIFY(__super::PreMessageLoop(nShowCmd));

        FTLTRACE(TEXT("PreMessageLoop, nShowCmd=%d, hr=0x%x\n"), nShowCmd, hr);
        return hr;
    }

    HRESULT PostMessageLoop() throw()
    {
        HRESULT hr = S_OK;
        COM_VERIFY(__super::PostMessageLoop());

        FTLTRACE(TEXT("PostMessageLoop, hr=0x%x\n"), hr);

        return hr;
    }
};

CAtlServiceDemoModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
                                LPTSTR /*lpCmdLine*/, int nShowCmd)
{
    int nRet = 0;
    FTLTRACE(TEXT("Enter _tWinMain, nShowCmd = %d\n"), nShowCmd);

    nRet = _AtlModule.WinMain(nShowCmd);
    FTLTRACE(TEXT("Leave _tWinMain, nRet = %d\n"), nRet);
    return nRet;
}

