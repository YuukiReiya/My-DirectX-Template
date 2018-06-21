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
	/*! DirectInput8�C���^�[�t�F�[�X�擾 */
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDInput, NULL);
	if (hr != DI_OK) { return false; }

	/****************/
	/*	�L�[�{�[�h	*/
	/****************/

	/*! �f�o�C�X�擾 */
	hr = m_pDInput->CreateDevice(GUID_SysKeyboard, &m_pDIDevice, NULL);
	if (hr != DI_OK) { return false; }

	/*! �f�[�^�t�H�[�}�b�g�̎w�� */
	m_pDIDevice->SetDataFormat(&c_dfDIKeyboard);

	/*! �������[�h�̐ݒ� */
	m_pDIDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		
	return true;
}
