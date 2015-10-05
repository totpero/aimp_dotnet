#pragma once

namespace AIMPSDK
{
	#include "aimp4_00_sdk.h"
	#include "AimpObject.h"
	#include "IUnknownInterfaceImpl.h"
	#include "Utils.h"

	using namespace AIMPSDK4;

	//typedef IAIMPCore IAIMPCore;

	typedef boost::intrusive_ptr<AIMPSDK4::IAIMPString> IAIMPString_ptr;
}