#pragma once
#include "..\BaseManager.h"
#include "AimpPlaybackQueueItem.h"

namespace AIMP
{
	namespace SDK
	{
		using namespace AIMPSDK;
		using namespace AIMP::SDK;

		using namespace AIMP::SDK::Services::PlayList;
		using namespace AIMP::SDK::Services::Playback;

		public ref class AimpServicePlaybackQueue : public AimpBaseManager,  public IAimpPlaybackQueueService
		{
		private:
			IAIMPServicePlaybackQueue *_service;
			AimpCheckUrl ^_checkUrlHandler;

		public:
			explicit AimpServicePlaybackQueue(ManagedAimpCore ^core) : AimpBaseManager(core)
			{
				IAIMPServicePlaybackQueue *service;
				if (!CheckResult(core->GetService(IID_IAIMPServicePlaybackQueue, (void**) &service)))
				{
					throw gcnew System::ApplicationException("Unable create AIMP service");
				}

				_service = service;
			}

			virtual IAimpPlaybackQueueItem^ GetNextTrack()
			{
				IAIMPPlaybackQueueItem *item;
				if (!CheckResult(_service->GetNextTrack(&item)))
				{
		//			return gcnew AimpPlaybackQueueItem(item);
				}

				return nullptr;
			}

			virtual IAimpPlaybackQueueItem^ GetPrevTrack()
			{
				IAIMPPlaybackQueueItem *item;
				if (!CheckResult(_service->GetPrevTrack(&item)))
				{
//					return gcnew AimpPlaybackQueueItem(item);
				}

				return nullptr;
			}

			virtual event AimpCheckUrl ^OnCheckURL
			{
				void add(AimpCheckUrl ^onEvent)
				{
					bool tmp = _checkUrlHandler == nullptr;
					if (tmp)
					{
						_checkUrlHandler = (AimpCheckUrl^)System::Delegate::Combine(_checkUrlHandler, onEvent);
					}
				}

				void remove(AimpCheckUrl ^onEvent)
				{
					bool tmp = _checkUrlHandler != nullptr;
					if (tmp)
					{
						_checkUrlHandler = (AimpCheckUrl^)System::Delegate::Remove(_checkUrlHandler, onEvent);
					}
				}
			}
		internal:
			bool RaiseCheckUrl(String^ %url)
			{
				bool tmp = this->_checkUrlHandler != nullptr;
				if (tmp)
				{
					return _checkUrlHandler(url);
				}
			}
		};
	}
}