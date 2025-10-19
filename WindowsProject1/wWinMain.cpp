#include <Windows.h>
#include "DialogExpressionParser.h"
int APIENTRY wWinMain
( _In_     HINSTANCE hInstance    
, _In_opt_ HINSTANCE hPrevInstance
, _In_     LPWSTR    lpCmdLine    
, _In_     int       nCmdShow     
)
{
	DialogExpressionParser::Show(nullptr);
	return 0;
}
