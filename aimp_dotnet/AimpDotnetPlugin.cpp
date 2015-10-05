// This is the main DLL file.

#include "stdafx.h"
#include "DotNetPlugin.h"


BOOL WINAPI AIMPPluginGetHeader(AIMPSDK::IAIMPPlugin **Header)
{
	*Header = new DotNetPlugin();
	return true;
}