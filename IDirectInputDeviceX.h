#pragma once

class m_IDirectInputDeviceX
{
private:
	IDirectInputDevice8W *ProxyInterface;
	m_IDirectInputDevice7W *WrapperInterface;
	DWORD DirectXVersion;
	REFIID WrapperID;
	DWORD StringType;
	ULONG RefCount = 1;

	// For DeviceData
	bool dodThreadFlag = false;
	DIDEVICEOBJECTDATA *pdod = nullptr;
	DWORD dodSize = 0;

public:
	m_IDirectInputDeviceX(IDirectInputDevice8W *aOriginal, DWORD Version, REFIID riid, m_IDirectInputDevice7W *Interface) : ProxyInterface(aOriginal), DirectXVersion(Version), WrapperID(riid), WrapperInterface(Interface)
	{
		StringType = GetStringType(WrapperID);

		Logging::LogDebug() << "Creating device " << __FUNCTION__ << "(" << this << ")" << " converting device from v" << Version << " to v8 using " << ((StringType == DEFAULT_CHARSET) ? "UNICODE" : "ANSI");
	}
	~m_IDirectInputDeviceX()
	{
		Logging::LogDebug() << __FUNCTION__ << "(" << this << ")" << " deleting device!";

		if (pdod)
		{
			delete pdod;
		}
	}

	/*** IUnknown methods ***/
	STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID * ppvObj);
	STDMETHOD_(ULONG, AddRef)(THIS);
	STDMETHOD_(ULONG, Release)(THIS);

	/*** IDirectInputDevice methods ***/
	STDMETHOD(GetCapabilities)(THIS_ LPDIDEVCAPS);
	STDMETHOD(EnumObjectsA)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKA, LPVOID, DWORD);
	STDMETHOD(EnumObjectsW)(THIS_ LPDIENUMDEVICEOBJECTSCALLBACKW, LPVOID, DWORD);
	STDMETHOD(GetProperty)(THIS_ REFGUID, LPDIPROPHEADER);
	STDMETHOD(SetProperty)(THIS_ REFGUID, LPCDIPROPHEADER);
	STDMETHOD(Acquire)(THIS);
	STDMETHOD(Unacquire)(THIS);
	STDMETHOD(GetDeviceState)(THIS_ DWORD, LPVOID);
	STDMETHOD(GetDeviceData)(THIS_ DWORD, LPDIDEVICEOBJECTDATA, LPDWORD, DWORD);
	STDMETHOD(SetDataFormat)(THIS_ LPCDIDATAFORMAT);
	STDMETHOD(SetEventNotification)(THIS_ HANDLE);
	STDMETHOD(SetCooperativeLevel)(THIS_ HWND, DWORD);
	STDMETHOD(GetObjectInfoA)(THIS_ LPDIDEVICEOBJECTINSTANCEA, DWORD, DWORD);
	STDMETHOD(GetObjectInfoW)(THIS_ LPDIDEVICEOBJECTINSTANCEW, DWORD, DWORD);
	STDMETHOD(GetDeviceInfoA)(THIS_ LPDIDEVICEINSTANCEA);
	STDMETHOD(GetDeviceInfoW)(THIS_ LPDIDEVICEINSTANCEW);
	STDMETHOD(RunControlPanel)(THIS_ HWND, DWORD);
	STDMETHOD(Initialize)(THIS_ HINSTANCE, DWORD, REFGUID);

	/*** IDirectInputDevice2 methods ***/
	STDMETHOD(CreateEffect)(THIS_ REFGUID, LPCDIEFFECT, LPDIRECTINPUTEFFECT *, LPUNKNOWN);
	STDMETHOD(EnumEffectsA)(THIS_ LPDIENUMEFFECTSCALLBACKA, LPVOID, DWORD);
	STDMETHOD(EnumEffectsW)(THIS_ LPDIENUMEFFECTSCALLBACKW, LPVOID, DWORD);
	STDMETHOD(GetEffectInfoA)(THIS_ LPDIEFFECTINFOA, REFGUID);
	STDMETHOD(GetEffectInfoW)(THIS_ LPDIEFFECTINFOW, REFGUID);
	STDMETHOD(GetForceFeedbackState)(THIS_ LPDWORD);
	STDMETHOD(SendForceFeedbackCommand)(THIS_ DWORD);
	STDMETHOD(EnumCreatedEffectObjects)(THIS_ LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD);
	STDMETHOD(Escape)(THIS_ LPDIEFFESCAPE);
	STDMETHOD(Poll)(THIS);
	STDMETHOD(SendDeviceData)(THIS_ DWORD, LPCDIDEVICEOBJECTDATA, LPDWORD, DWORD);

	/*** IDirectInputDevice7 methods ***/
	STDMETHOD(EnumEffectsInFileA)(THIS_ LPCSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD);
	STDMETHOD(EnumEffectsInFileW)(THIS_ LPCWSTR, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD);
	STDMETHOD(WriteEffectToFileA)(THIS_ LPCSTR, DWORD, LPDIFILEEFFECT, DWORD);
	STDMETHOD(WriteEffectToFileW)(THIS_ LPCWSTR, DWORD, LPDIFILEEFFECT, DWORD);

	// Helper functions
	void IncRef() { InterlockedIncrement(&RefCount); }
	IDirectInputDevice8A *GetProxyInterfaceA() { return (IDirectInputDevice8A*)ProxyInterface; }
	IDirectInputDevice8W *GetProxyInterfaceW() { return ProxyInterface; }
};
