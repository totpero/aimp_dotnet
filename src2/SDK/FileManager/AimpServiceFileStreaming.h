/*
 * AIMP DotNet SDK
 *
 * (C) 2017
 * Mail: mail4evgeniy@gmail.com
 * https://github.com/martin211/aimp_dotnet
 */
#pragma once
#include "..\BaseManager.h"
#include "..\AimpStream.h"

namespace AIMP
{
    namespace SDK
    {
        using namespace System;
        using namespace AIMP::SDK::FileManager;

        public ref class AimpServiceFileStreaming : public AimpBaseManager<IAIMPServiceFileStreaming>, public IAimpServiceFileStreaming
        {
        public:
            AimpServiceFileStreaming(ManagedAimpCore^ core) : AimpBaseManager<IAIMPServiceFileStreaming>(core)
            {}

            // Inherited via IAimpServiceFileStreaming
            virtual AimpActionResult CreateStreamForFile(String ^fileName, FileStreamingType flags, long long ofset, long long size, IAimpStream ^%stream);

            virtual AimpActionResult CreateStreamForFileUri(String ^fileUrl, IAimpVirtualFile ^%virtualFile, IAimpStream ^%stream);
        };
    }
}
