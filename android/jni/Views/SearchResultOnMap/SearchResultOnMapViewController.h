// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#pragma once

#include "AndroidNativeState.h"
#include "Types.h"
#include "SearchResultPoi.h"
#include "SearchResultPoiViewModule.h"
#include "ICallback.h"
#include "SearchResultModel.h"
#include "ISearchResultOnMapInFocusViewModel.h"
#include "IScreenControlViewModel.h"
#include "Modality.h"

namespace ExampleApp
{
    namespace SearchResultOnMap
    {
		class SearchResultOnMapViewController : private Eegeo::NonCopyable
		{
			AndroidNativeState& m_nativeState;
            ISearchResultOnMapInFocusViewModel& m_searchResultOnMapInFocusViewModel;
            ScreenControlViewModel::IScreenControlViewModel& m_searchResultPoiScreenControlViewModel;
            Modality::IModalityModel& m_modalityModel;

            Eegeo::Helpers::ICallback0* m_pSearchResultOnMapFocusOpenedCallback;
            Eegeo::Helpers::ICallback0* m_pSearchResultOnMapFocusClosedCallback;
            Eegeo::Helpers::ICallback0* m_pSearchResultOnMapFocusUpdatedCallback;
            Eegeo::Helpers::ICallback2<ScreenControlViewModel::IScreenControlViewModel&, float>* m_pOnScreenStateChangedCallback;

			jclass m_uiViewClass;
			jobject m_uiView;
			const float m_pinOffset;

		public:
			SearchResultOnMapViewController(
				AndroidNativeState& nativeState,
                ExampleApp::SearchResultOnMap::ISearchResultOnMapInFocusViewModel& searchResultOnMapInFocusViewModel,
                ScreenControlViewModel::IScreenControlViewModel& searchResultPoiScreenControlViewModel,
                Modality::IModalityModel& modalityModel,
    			float pinDiameter
			);

			~SearchResultOnMapViewController();

			void HandleSearchResultOnMapClicked();

		private:

            void SearchResultOnMapFocusOpenedCallback();

            void SearchResultOnMapFocusClosedCallback();

            void SearchResultOnMapFocusUpdatedCallback();

            void OnScreenStateChangedCallback(ScreenControlViewModel::IScreenControlViewModel &viewModel, float& onScreenState);
		};
    }
}