#pragma once

//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)
#include <Windows.h>
#include <crtdbg.h>//_ASSERT_EXPR()�ŕK�v.

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "MyMacro.h"

//ײ���ؓǂݍ���.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v.

//=================================================
//	�萔.
//=================================================
//#define WND_W 1280	//���萔�錾��#define�͎g��Ȃ�.
const int WND_W = 1280;	//����޳�̕�.1280�f�t�H���g.�t���X�N���[��1600
const int WND_H = 720;	//����޳�̍���.720�f�t�H���g.�t���X�N���[��900

const int	WND_W_HALF = WND_W / 2;	//�E�B���h�E�̕��̔���.
const int	WND_H_HALF = WND_H / 2; //�E�B���h�E�̍����̔���.
