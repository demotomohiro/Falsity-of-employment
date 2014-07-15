#pragma once

namespace AA
{
const static wchar_t dance0[] =
L"    ∧,,__∧\r\n"
L"    ( `･ω･) ))\r\n"
L" (( (  つ  ヽ、\r\n"
L"     〉 とノ  )))\r\n"
L"    （__ノ^(＿)\r\n"
;
const static wchar_t dance1[] =
L"♪  ∧__,,∧\r\n"
L"  (( (･ω･´ )\r\n"
L"    /  ⊂  ) ))\r\n"
L"  ((( ヽつ 〈\r\n"
L"    (＿)^ヽ__）\r\n"
;
#if 0
const static wchar_t dance2[] =
L"     ∧,,__∧\r\n"
L"    (         ) ))\r\n"
L" (( (        ヽ、\r\n"
L"     _〉    ノ  )  ))\r\n"
L"   （___ノ^(＿)\r\n"
;
const static wchar_t dance3[] =
L"     ∧,＿∧\r\n"
L"  (( (         )\r\n"
L"     /        _) ))\r\n"
L" (( (   (    〈_\r\n"
L"    (＿)^ヽ__）\r\n"
;
#endif

static LOGFONTA baselogfont =
{
	-16, 0,							//Height, Width
	0, 0,
	400,							//Weight
	FALSE, FALSE, FALSE,
	SHIFTJIS_CHARSET,
	OUT_DEFAULT_PRECIS,				//Out Precision
	CLIP_DEFAULT_PRECIS,			//Clip Precision
	DEFAULT_QUALITY,				//Quality
	VARIABLE_PITCH | FF_SWISS,		//Pitch And Family
	"MS PGothic"
};
void aa(HWND hWnd)
{
	HFONT hFont = CreateFontIndirectA(&baselogfont);
	assert(hFont);
	SendMessage(hWnd, WM_SETFONT, (WPARAM)hFont, TRUE);
//	LRESULT using_font = SendMessage(hWnd_picture, WM_GETFONT, 0, 0);
//	SetWindowTextW(hWnd, L"TESTテスト\n\r表示\r\nあいうえお");
	for(int i=0; i<4; ++i)
	{
		SetWindowTextW(hWnd, dance0);
		Sleep(256);
		SetWindowTextW(hWnd, dance1);
		Sleep(256);
	}
}
}
