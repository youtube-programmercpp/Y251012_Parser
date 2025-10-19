#include "DialogExpressionParser.h"
#include "Resource.h"
#include "../ConsoleApplication1/Parser.h"
#include "../ConsoleApplication1/Tokenizer.h"
#include <memory>
#include <stdexcept>
#include <sstream>
#include <CommCtrl.h>

DialogExpressionParser::DialogExpressionParser
( _In_ HWND  hEditExpression             
, _In_ HWND  hTree                       
, _In_ HWND  hEditErrMsg                 
, _In_ LONG  dx_dlg_and_edit             
, _In_ LONG  left_of_edit                
, _In_ LONG  cy_of_edit                  
, _In_ SIZE  right_bottom_margin_for_tree
, _In_ LONG  top_margin_for_errmsg       
) noexcept
	: hEditExpression             { hEditExpression              }
	, hTree                       { hTree                        }
	, hEditErrMsg                 { hEditErrMsg                  }
	, dx_dlg_and_edit             { dx_dlg_and_edit              }
	, left_of_edit                { left_of_edit                 }
	, cy_of_edit                  { cy_of_edit                   }
	, right_bottom_margin_for_tree{ right_bottom_margin_for_tree }
	, top_margin_for_errmsg       { top_margin_for_errmsg        }
{
}
void expand_all(HWND hTree, HTREEITEM hItem)
{
	HTREEITEM hChild = TreeView_GetChild(hTree, hItem);
	while (hChild) {
		expand_all(hTree, hChild);
		hChild = TreeView_GetNextSibling(hTree, hChild);
	}
	TreeView_Expand(hTree, hItem, TVE_EXPAND);
}
void DialogExpressionParser::parse(_In_ HWND hDlg, std::istream&& istm)
{
	Parser parser(istm);
	try {
		if (const auto expr = parser.ParseExpression()) {
			TreeView_DeleteAllItems(hTree);
			expand_all(hTree, expr->insert_item(hTree, nullptr, TVI_FIRST));
		}
		SetWindowTextW(hEditErrMsg, L"");
	}
	catch (const std::exception& e) {
		SetWindowTextA(hEditErrMsg, e.what());
	}
}
static SIZE size_of_rect (const RECT& rc) noexcept
{
	return { rc.right - rc.left, rc.bottom - rc.top };
}
SIZE operator-(const SIZE& a, const SIZE& b) noexcept
{
	return { a.cx - b.cx, a.cy - b.cy };
}
RECT window_rect(HWND hWnd)
{
	RECT rc;
	if (GetWindowRect(hWnd, &rc))
		return rc;
	else
		throw std::runtime_error("GetWindowRect failed with " + std::to_string(GetLastError()));
}
RECT client_rect(HWND hWnd)
{
	RECT rc;
	if (GetClientRect(hWnd, &rc))
		return rc;
	else
		throw std::runtime_error("GetClientRect failed with " + std::to_string(GetLastError()));
}
POINT screen_to_client(HWND hDlg, POINT pt)
{
	if (ScreenToClient(hDlg, &pt))
		return pt;
	else
		throw std::runtime_error("ScreenToClient failed");
}
RECT screen_to_client(HWND hDlg, RECT rc)
{
	RECT retval;
	reinterpret_cast<POINT*>(&retval)[0] = screen_to_client(hDlg, reinterpret_cast<const POINT*>(&rc)[0]);
	reinterpret_cast<POINT*>(&retval)[1] = screen_to_client(hDlg, reinterpret_cast<const POINT*>(&rc)[1]);
	return retval;
}
void resize_window(_In_ HWND hWnd, const SIZE& new_size)
{
	if (/*WINUSERAPI BOOL WINAPI*/SetWindowPos
	( /*_In_     HWND hWnd           */hWnd
	, /*_In_opt_ HWND hWndInsertAfter*/nullptr
	, /*_In_     int  X              */0
	, /*_In_     int  Y              */0
	, /*_In_     int  cx             */new_size.cx
	, /*_In_     int  cy             */new_size.cy
	, /*_In_     UINT uFlags         */SWP_NOMOVE | SWP_NOZORDER
	))
		return;
	else
		throw std::runtime_error("SetWindowPos failed with " + std::to_string(GetLastError()));
}
void move_window(_In_ HWND hWnd, const POINT left_top, const SIZE& new_size)
{
	if (/*WINUSERAPI BOOL WINAPI*/MoveWindow
	( /*_In_ HWND hWnd    */hWnd
	, /*_In_ int  X       */left_top.x
	, /*_In_ int  Y       */left_top.y
	, /*_In_ int  nWidth  */new_size.cx
	, /*_In_ int  nHeight */new_size.cy
	, /*_In_ BOOL bRepaint*/true
	))
		return;
	else
		throw std::runtime_error("SetWindowPos failed with " + std::to_string(GetLastError()));
}
void DialogExpressionParser::resize_dialog(_In_ HWND hDlg, _In_ const SIZE& size_of_client)
{
	const auto size_of_edit = SIZE{ size_of_client.cx - dx_dlg_and_edit, cy_of_edit };
	resize_window(hEditExpression, size_of_edit);
	resize_window(hTree, size_of_client - right_bottom_margin_for_tree);
	const auto top_of_errmsg = size_of_client.cy - top_margin_for_errmsg;
	move_window(hEditErrMsg, POINT{left_of_edit, top_of_errmsg}, size_of_edit);
}
INT_PTR DialogExpressionParser::DialogProc(_In_ HWND hDlg, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam) noexcept
{
	switch (message) {
	case WM_NCDESTROY:
		delete this;
		return true;
	case WM_SIZE:
		switch (wParam) {
		case SIZE_RESTORED:
		case SIZE_MAXIMIZED:
			resize_dialog(hDlg, { LOWORD(lParam), HIWORD(lParam) });
			return true;
		default:
			return true;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			EndDialog(hDlg, IDCANCEL);
			return true;
		case IDC_EDIT_Expression:
			switch (HIWORD(wParam)) {
			case EN_CHANGE:
				if (const auto n = GetWindowTextLengthA(hEditExpression)) {
					std::string buf(n, '\0');
					const auto first = &buf.front();
					buf.resize(GetWindowTextA(hEditExpression, first, n + 1));
					parse(hDlg, std::istringstream(buf));
				}
				return true;
			default:
				return false;
			}
		default:
			return false;
		}
	default:
		return false;
	}
}
INT_PTR DialogExpressionParser::OnInitDialog(_In_ HWND hDlg, _In_ WPARAM wParam)
{
	return true;
}
extern "C" extern IMAGE_DOS_HEADER __ImageBase;

INT_PTR DialogExpressionParser::Show(_In_opt_ HWND hWndParent) noexcept
{
	return /*WINUSERAPI INT_PTR WINAPI*/DialogBoxParamW
	( /*_In_opt_ HINSTANCE hInstance     */HINSTANCE(&__ImageBase)
	, /*_In_     LPCWSTR   lpTemplateName*/MAKEINTRESOURCEW(IDD_DIALOG1)
	, /*_In_opt_ HWND      hWndParent    */hWndParent
	, /*_In_opt_ DLGPROC   lpDialogFunc  */[](HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)->INT_PTR
		{
			if (message == WM_INITDIALOG) {
				try {
					const auto hEditExpression   = GetDlgItem(hDlg, IDC_EDIT_Expression);
					const auto hTree             = GetDlgItem(hDlg, IDC_TREE_Parse     );
					const auto hEditErrMsg       = GetDlgItem(hDlg, IDC_EDIT_ErrMsg    );
					const auto size_of_client    = size_of_rect(client_rect(hDlg));
					const auto rect_of_edit      = screen_to_client(hDlg, window_rect(hEditExpression));
					const auto rect_of_tree      = screen_to_client(hDlg, window_rect(hTree          ));
					const auto rect_of_errmsg    = screen_to_client(hDlg, window_rect(hEditErrMsg    ));
					std::unique_ptr<DialogExpressionParser> dlg(new DialogExpressionParser
						{ /*_In_ HWND  hEditExpression             */hEditExpression
						, /*_In_ HWND  hTree                       */hTree
						, /*_In_ HWND  hEditErrMsg                 */hEditErrMsg
						, /*_In_ LONG  dx_dlg_and_edit             */size_of_client.cx   - (rect_of_edit.right - rect_of_edit.left)
						, /*_In_ LONG  left_of_edit                */rect_of_errmsg.left
						, /*_In_ LONG  cy_of_edit                  */rect_of_edit.bottom - rect_of_edit.top
						, /*_In_ SIZE  right_bottom_margin_for_tree*/size_of_client      - size_of_rect(rect_of_tree)
						, /*_In_ LONG  top_margin_for_errmsg       */size_of_client.cy   - rect_of_errmsg.top
						});
					SetWindowLongPtrW(hDlg, DWLP_USER, LONG_PTR(dlg.get()));
					SetWindowLongPtrW(hDlg, DWLP_DLGPROC, LONG_PTR(static_cast<DLGPROC>(
						[](HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)->INT_PTR
						{
							return reinterpret_cast<DialogExpressionParser*>(GetWindowLongPtrW(hDlg, DWLP_USER))->DialogProc(hDlg, message, wParam, lParam);;
						})));
					return dlg.release()->OnInitDialog(hDlg, wParam);
				}
				catch (const std::exception& e) {
					MessageBoxA(hDlg, e.what(), "Error", MB_ICONHAND);
					EndDialog(hDlg, IDCANCEL);
					return false;
				}
			}
			else
				return false;
		}
	, /*_In_     LPARAM    dwInitParam   */{}
	);
}
