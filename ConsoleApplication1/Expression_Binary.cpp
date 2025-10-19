#include "Expression_Binary.h"
Expression_Binary::Expression_Binary(Expression* left , Operator op, Expression* right) noexcept
	: left (left )
	, right(right)
	, op   (op   )
{
}
Expression_Binary::~Expression_Binary() noexcept
{
	delete right;
	delete left ;
}
long long Expression_Binary::Evaluate() const noexcept
{
	return (*op.function_pointer)(left->Evaluate(), right->Evaluate());
}

HTREEITEM Expression_Binary::insert_item(HWND hTree, HTREEITEM hParent, HTREEITEM hInsertAfter) const noexcept
{
	const auto result = op.symbol + (" (" + std::to_string(Evaluate()) + ")");
	TVINSERTSTRUCTA  tvi
	{ /*HTREEITEM hParent     ;*/hParent
	, /*HTREEITEM hInsertAfter;*/hInsertAfter
	, /*TV_ITEMA  item        ;*/
		{ /*UINT      mask          */TVIF_TEXT
		, /*HTREEITEM hItem         */nullptr
		, /*UINT      state         */0
		, /*UINT      stateMask     */0
		, /*LPSTR     pszText       */const_cast<LPSTR>(result.c_str())
		, /*int       cchTextMax    */0
		, /*int       iImage        */0
		, /*int       iSelectedImage*/0
		, /*int       cChildren     */0
		, /*LPARAM    lParam        */0
		}
	};
	const auto hItem = (HTREEITEM)SendMessageA(hTree, TVM_INSERTITEMA, 0, LPARAM(static_cast<LPTV_INSERTSTRUCTA>(&tvi)));
	const auto hLeft = left ->insert_item(hTree, hItem, TVI_FIRST);
	(void)             right->insert_item(hTree, hItem, hLeft    );
	return hItem;
}
