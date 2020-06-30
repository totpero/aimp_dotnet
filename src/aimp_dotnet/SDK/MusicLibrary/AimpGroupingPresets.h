// ----------------------------------------------------
// AIMP DotNet SDK
// Copyright (c) 2014 - 2020 Evgeniy Bogdan
// https://github.com/martin211/aimp_dotnet
// Mail: mail4evgeniy@gmail.com
// ----------------------------------------------------

#pragma once
#include "SDK\AimpObject.h"
#include "AIMPSDK\AIMPSDK.h"
#include "InternalAimpGroupingTreeDataProvider.h"

namespace AIMP
{
    namespace SDK
    {
        using namespace System;
        using namespace Collections;
        using namespace Generic;
        using namespace SDK;
        using namespace MusicLibrary::DataStorage;
        using namespace MusicLibrary::Presets;

        public ref class AimpGroupingPresets : public AimpObject<IAIMPMLGroupingPresets>, public IAimpGroupingPresets
        {
        private:
            InternalAimpGroupingTreeDataProvider* _internalProvider;

        public:
            explicit AimpGroupingPresets(IAIMPMLGroupingPresets* aimpObject);

            virtual ActionResultType Add(String^ id, String^ name, IAimpGroupingTreeDataProvider^ provider,
                                         IAimpGroupingPreset^% preset);

            virtual ActionResultType Add(String^ id, String^ name, Generic::IList<String^>^ fieldNames,
                                         IAimpGroupingPresetStandard^% preset);

            virtual ActionResultType Add(System::String^ id, String^ name, String^ fieldName,
                                         IAimpGroupingPresetStandard^% preset);

            virtual void BeginUpdate();

            virtual void EndUpdate();

            virtual ActionResultType Delete(int index);

            virtual int GetCount(void);

            virtual ActionResultType Get(int index, IAimpGroupingPreset^% preset);

            virtual ActionResultType Get(int index, IAimpGroupingPresetStandard^% preset);

            virtual ActionResultType GetById(String^ id, IAimpGroupingPreset^% preset);

            virtual ActionResultType GetById(String^ id, IAimpGroupingPresetStandard^% preset);
        };
    }
}
