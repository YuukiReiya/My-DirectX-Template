#include "DInput.h"
#include "../../MemoryLeaks.h"

DInput::DInput()
{
}


DInput::~DInput()
{
}

bool DInput::Init(HWND hWnd, HINSTANCE hInstance)
{
	HRESULT hr;
	/*! DirectInput8インターフェース取得 */
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, NULL);
	if (hr != DI_OK) { return false; }

	/****************/
	/*	キーボード	*/
	/****************/

	/*! デバイス取得 */
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if (hr != DI_OK) { return false; }

	/*! データフォーマットの指定 */
	m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);

	/*! 協調モードの設定 */
	m_pDIDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		
	return true;
}
