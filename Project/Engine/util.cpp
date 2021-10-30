#include "pch.h"
#include "util.h"

ComPtr<IDXGIAdapter> GrapicsAdapterUtil::EnumratesAdapters(const wchar_t* _name)
{
	ComPtr<IDXGIAdapter> pAdapter;
	vector<ComPtr<IDXGIAdapter>> vAdapters;
	IDXGIFactory1* pFactory = NULL;

	if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&pFactory)))
	{
		return nullptr;
	}

	for (UINT i = 0; pFactory->EnumAdapters(i, pAdapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		vAdapters.push_back(pAdapter);
	}

	if (pFactory)
	{
		pFactory->Release();
	}

	if (pAdapter)
	{
		pAdapter->Release();
	}
	
	pAdapter = SelectAdapters(vAdapters, _name);
	wstring output = L"*** NUM_ADAPTER = " + to_wstring(vAdapters.size())+L"\n";
	OutputDebugString(output.c_str());

	return pAdapter;
}

DXGI_MODE_DESC* GrapicsAdapterUtil::EnumratesOutputs(ComPtr<IDXGIAdapter> _adapter)
{
	IDXGIOutput* pOutput = NULL;
	HRESULT hr;
	hr = _adapter->EnumOutputs(1, &pOutput);

	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes = NULL;
	DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UINT;

	// Get the number of elements
	hr = pOutput->GetDisplayModeList(format, 0, &numModes, NULL);
	wstring output = L"*** NUM_OUTPUTS_DEFALUT_ADAPTER = " + to_wstring(numModes) + L"\n";
	OutputDebugString(output.c_str());
	// Get the list
	hr = pOutput->GetDisplayModeList(format, 0, &numModes, displayModes);

	OutputDebugString(L"************* DisplayModeList  *************\n");
	OutputDebugString(L"\n\n");

	for (UINT i = 0; i < numModes; ++i)
	{
		wstring output = L"*** WIDTH = "+ to_wstring(displayModes[i].Width) + L" HEIGHT = "+ to_wstring(displayModes[i].Height) + L" REFRESH = "+to_wstring(displayModes[i].RefreshRate.Numerator/ displayModes[i].RefreshRate.Denominator)+L"\n";
		OutputDebugString(output.c_str());
	}
	return displayModes;
}

ComPtr<IDXGIAdapter> GrapicsAdapterUtil::SelectAdapters(vector<ComPtr<IDXGIAdapter>> _adapters, const wchar_t* _name)
{
	DXGI_ADAPTER_DESC descs = {};
	int i = 0;
	for (ComPtr<IDXGIAdapter> _adapter : _adapters)
	{
		_adapter->GetDesc(&descs);
		if (wcscmp(descs.Description, _name) != 0)
		{
			return _adapter;
		}
	}

	return nullptr;
}

