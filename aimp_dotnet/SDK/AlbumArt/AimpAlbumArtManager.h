#pragma once

#include "..\BaseManager.h"
#include "..\PlayList\AimpFileInfo.h"

namespace AIMP
{
    namespace SDK
    {
        using namespace System;
        using namespace System::IO;
        using namespace AIMP::SDK;
        using namespace AIMP::SDK::AlbumArtManager;

        public ref class AimpAlbumArtManager : public AimpBaseManager2<IAIMPServiceAlbumArt>, public IAimpAlbumArtManager
        {
        public:
            explicit AimpAlbumArtManager(ManagedAimpCore^ core) : AimpBaseManager2<IAIMPServiceAlbumArt>(core)
            {
                //IAIMPServiceAlbumArt *service = (IAIMPServiceAlbumArt*)GetService(IID_IAIMPServiceAlbumArt);
                //IAIMPPropertyList *prop;
                //service->QueryInterface(IID_IAIMPPropertyList, (void**)&prop);
                //_properties = prop;
            }

            ~AimpAlbumArtManager()
            {
                System::Diagnostics::Debug::WriteLine("Dispose AimpAlbumArtManager");
                _properties->Release();
                delete _findCallback;
                delete _properties;
            }

            event EventHandler<AimpGetAlbumArtEventArgs^>^ Completed
            {
                virtual void add(EventHandler<AimpGetAlbumArtEventArgs^>^ onCompleted)
                {
                    if (this->_onComplete == nullptr)
                    {
                        this->_onComplete = (EventHandler<AimpGetAlbumArtEventArgs^>^)Delegate::Combine(this->_onComplete, onCompleted);
                    }
                }
                virtual void remove(EventHandler<AimpGetAlbumArtEventArgs^>^ onCompleted)
                {
                    delete _onComplete;
                    _onComplete = nullptr;
                }
                void raise(Object^ sender, AimpGetAlbumArtEventArgs^ args)
                {
                    if (_onComplete != nullptr)
                    {
                        _onComplete(sender, args);
                    }
                }
            }

            virtual property bool FindInFiles
            {
                bool get()
                {
                    int val;
                    _properties->GetValueAsInt32(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES, &val);
                    return val != 0;
                }
                void set(bool value)
                {
                    _properties->SetValueAsInt32(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES, value);
                }
            }

            virtual property bool FindInInternet
            {
                bool get()
                {
                    int val;
                    _properties->GetValueAsInt32(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_INTERNET, &val);
                    return false;
                }
                void set(bool value)
                {
                    _properties->SetValueAsInt32(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_INTERNET, value);
                }
            }

            virtual property array<System::String^>^ FileMasks
            {
                array<System::String^>^ get()
                {
                    IAIMPString *str;
                    _properties->GetValueAsObject(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES_MASKS, IID_IAIMPString, (void**)&str);
                    String^ result = gcnew String(str->GetData());
                    str->Release();
                    return result->Split(gcnew array<WCHAR>{';'});
                }
                void set(array<System::String^>^ val)
                {
                    String^ str;
                    for (int i = 0; i < val->Length; i++)
                    {
                        str += val[i] + ";";
                    }
                    IAIMPString *s = Converter::MakeAimpString(_core->GetAimpCore(), str);

                    _properties->SetValueAsObject(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES_MASKS, s);
                    s->Release();
                }
            }

            virtual property array<System::String^>^ FileExtensions
            {
                array<System::String^>^ get()
                {
                    IAIMPString *str;
                    _properties->GetValueAsObject(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES_EXTS, IID_IAIMPString, (void**)&str);
                    String^ result = gcnew String(str->GetData());
                    str->Release();
                    return result->Split(gcnew array<WCHAR>{';'});
                }
                void set(array<System::String^>^ val)
                {
                    String^ str;
                    for (int i = 0; i < val->Length; i++)
                    {
                        str += val[i] + ";";
                    }

                    IAIMPString *s = Converter::MakeAimpString(_core->GetAimpCore(), str);
                    _properties->SetValueAsObject(AIMP_SERVICE_ALBUMART_PROPID_FIND_IN_FILES_EXTS, s);
                    s->Release();
                }
            }

            virtual property int MaxFileSize
            {
                int get()
                {
                    return 0;
                }
                void set(int val)
                {
                }
            }

            void OnAlbumArtReceive(IAIMPImage* image, IAIMPImageContainer* image_container, void* user_data)
            {
                AimpGetAlbumArtEventArgs^ args = gcnew AimpGetAlbumArtEventArgs();

                if (image == NULL && image_container == NULL)
                {
                    Completed(this, args);
                    return;
                }

                if (image_container != NULL && image == NULL)
                {
                    args->CoverImage = Converter::GetBitmap(image_container);
                }
                else if (image != NULL)
                {
                    args->CoverImage = Converter::GetBitmap(image);
                }

                Completed(this, args);
            }

            virtual IntPtr GetImage(String^ fileUrl, String^ artist, String^ album, AimpFingCovertArtType flags, Object^ userData)
            {
                void* taskId;
                _findCallback = gcnew OnFindCoverCallback(this, &AIMP::AimpAlbumArtManager::OnAlbumArtReceive);
                //TAIMPServiceAlbumArtReceiveProc *f = &test;
                IntPtr thunk = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_findCallback);
                IAIMPString *sFileUrl = Converter::MakeAimpString(_core->GetAimpCore(), fileUrl);
                IAIMPString *sArtist = Converter::MakeAimpString(_core->GetAimpCore(), artist);
                IAIMPString *sAlbum = Converter::MakeAimpString(_core->GetAimpCore(), album);


                IAIMPServiceAlbumArt *service;
                if (GetService(IID_IAIMPServiceAlbumArt, &service) == AimpActionResult::Ok)
                {
                    service->Get(
                        sFileUrl,
                        sArtist,
                        sAlbum,
                        (DWORD)flags,
                        (TAIMPServiceAlbumArtReceiveProc(_stdcall *))thunk.ToPointer(),
                        reinterpret_cast<void*>(&userData), &taskId);

                    sFileUrl->Release();
                    sAlbum->Release();
                    sAlbum->Release();

                    return (IntPtr)taskId;
                }

                return IntPtr::Zero;
            }

            virtual IntPtr GetImage(AIMP::SDK::PlayList::IAimpFileInfo^ fileInfo, AimpFingCovertArtType flags, Object^ userData)
            {
                void* taskId = nullptr;
                _findCallback = gcnew OnFindCoverCallback(this, &AIMP::AimpAlbumArtManager::OnAlbumArtReceive);
                IntPtr thunk = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(_findCallback);
                AimpFileInfo^ fi = (AimpFileInfo^)fileInfo;

                //IAIMPServiceAlbumArt *service = (IAIMPServiceAlbumArt*)GetService(IID_IAIMPServiceAlbumArt);
                //service->Get2(
                //    ((AIMP::SDK::AimpFileInfo^) fileInfo)->InternalAimpObject,
                //    (DWORD)flags,
                //    (TAIMPServiceAlbumArtReceiveProc(_stdcall *))thunk.ToPointer(),
                //    reinterpret_cast<void*>(&userData),
                //    &taskId);

                return (IntPtr)taskId;
            }

            virtual void Cancel(IntPtr taskId, AimpFingCovertArtType flags)
            {
                if (taskId != IntPtr::Zero)
                {
                    //IAIMPServiceAlbumArt *service = (IAIMPServiceAlbumArt*)GetService(IID_IAIMPServiceAlbumArt);
                    //service->Cancel((void*)taskId, (DWORD)flags);
                }
            }
        private:
            delegate void OnFindCoverCallback(IAIMPImage *image, IAIMPImageContainer *imageContainer, void *UserData);
            OnFindCoverCallback^ _findCallback;
            IAIMPPropertyList *_properties;
            EventHandler<AimpGetAlbumArtEventArgs^>^ _onComplete;
        };
    }
}