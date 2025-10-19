#pragma once
#include <Windows.h>
#include <istream>
class DialogExpressionParser
{
	HWND  hEditExpression             ;
	HWND  hTree                       ;
	HWND  hEditErrMsg                 ;
	LONG  dx_dlg_and_edit             ;
	LONG  left_of_edit                ;
	LONG  cy_of_edit                  ;
	SIZE  right_bottom_margin_for_tree;
	DialogExpressionParser
	( _In_ HWND  hEditExpression             
	, _In_ HWND  hTree                       
	, _In_ HWND  hEditErrMsg                 
	, _In_ LONG  dx_dlg_and_edit             
	, _In_ LONG  left_of_edit                
	, _In_ LONG  cy_of_edit                  
	, _In_ SIZE  right_bottom_margin_for_tree
	) noexcept;
	void parse(_In_ HWND hDlg, std::istream&& istm);
	void resize_dialog(_In_ HWND hDlg, _In_ const SIZE& size_of_client);
	INT_PTR DialogProc(_In_ HWND hDlg, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept;
	INT_PTR OnInitDialog(_In_ HWND hDlg, _In_ WPARAM wParam);
public:
	static INT_PTR Show(_In_opt_ HWND hWndParent) noexcept;
};

