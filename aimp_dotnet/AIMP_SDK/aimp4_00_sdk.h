#include <InitGuid.h> // need to force macro DEFINE_GUID to define guid instead declare.
#include <windows.h>

namespace AIMPSDK4
{
	// Add all AIMP SDK functionality to AIMP360SDK namespace.
	#include "AIMP400\apiPlugin.h"
	#include "AIMP400\apiPlayer.h"
	#include "AIMP400\apiObjects.h"
	#include "AIMP400\apiFileManager.h"
	#include "AIMP400\apiMessages.h"
	#include "AIMP400\apiAlbumArt.h"
	#include "AIMP400\apiMenu.h"
	#include "AIMP400\apiPlaylists.h"
	#include "AIMP400\apiMenu.h"
	#include "AIMP400\apiMUI.h"
	#include "AIMP400\apiAlbumArt.h"
	#include "AIMP400\apiCore.h"
	#include "AIMP400\apiActions.h"
	#include "AIMP400\apiOptions.h"
}