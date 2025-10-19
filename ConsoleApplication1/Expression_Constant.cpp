#include "Expression_Constant.h"
#include <string>
Expression_Constant::Expression_Constant(long long value) noexcept
	: value(value)
{
}
long long Expression_Constant::Evaluate() const noexcept
{
	return value;
}
HTREEITEM Expression_Constant::insert_item(HWND hTree, HTREEITEM hParent, HTREEITEM hInsertAfter) const noexcept
{
	const auto s = std::to_string(value);
	TVINSERTSTRUCTA  tvi
	{ /*HTREEITEM hParent     ;*/hParent
	, /*HTREEITEM hInsertAfter;*/hInsertAfter
	, /*TV_ITEMA  item        ;*/
		{ /*UINT      mask          */TVIF_TEXT
		, /*HTREEITEM hItem         */nullptr
		, /*UINT      state         */0
		, /*UINT      stateMask     */0
		, /*LPSTR     pszText       */const_cast<LPSTR>(s.c_str())
		, /*int       cchTextMax    */0
		, /*int       iImage        */0
		, /*int       iSelectedImage*/0
		, /*int       cChildren     */0
		, /*LPARAM    lParam        */0
		}
	};
	return (HTREEITEM)SendMessageA(hTree, TVM_INSERTITEMA, 0, LPARAM(static_cast<LPTV_INSERTSTRUCTA>(&tvi)));
}
