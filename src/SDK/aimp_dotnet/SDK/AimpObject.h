//  ----------------------------------------------------
//  AIMP DotNet SDK
//  
//  Copyright (c) 2014 - 2023 Evgeniy Bogdan
//  https://github.com/martin211/aimp_dotnet
//  
//  Mail: mail4evgeniy@gmail.com 
//  ----------------------------------------------------

#pragma once
#include <msclr\marshal_cppstd.h>

template <class TAimpObject>
public ref class AimpObject : public IAimpObject {
private:
    bool _isDisposable;
protected:
    AimpObject() {
    }

    [System::Diagnostics::DebuggerBrowsableAttribute(System::Diagnostics::DebuggerBrowsableState::Never)]
    TAimpObject* _aimpObject;
public:
    AimpObject(TAimpObject* aimpObject, bool registerAtMemoryManager) : _aimpObject(aimpObject) {
        _isDisposable = registerAtMemoryManager;
        RegisterAtMemoryManager();
    }

    AimpObject(TAimpObject* aimpObject) : AimpObject(aimpObject, false) {
        RegisterAtMemoryManager();
    }

    !AimpObject() {
        FreeResources();
        ReleaseFromMemoryManager();
    }

    ~AimpObject() {
        this->!AimpObject();
    }

protected:
    ActionResultType CheckResult(HRESULT result) {
        return Utils::CheckResult(result);
    }

    virtual void FreeResources() {

    }

    virtual void RegisterAtMemoryManager() {
        if (_isDisposable && InternalAimpObject != nullptr) {
            AimpMemoryManager::getInstance().AddObject(this->GetHashCode(), InternalAimpObject, msclr::interop::marshal_as<std::string>(this->ToString()));
            ComObject->AddRef();
        }
    }

    virtual void ReleaseFromMemoryManager() {
        if (_isDisposable) {
            AimpMemoryManager::getInstance().Release(this->GetHashCode());
            if (InternalAimpObject != nullptr) {
                ComObject->Release();
            }
        }
    }

public:
    property IUnknown* ComObject {
        IUnknown* get() {
            return safe_cast<IUnknown*>(InternalAimpObject);
        }
    }

    property TAimpObject* InternalAimpObject
    {
        TAimpObject* get() {
            return _aimpObject;
        }
    }
};
