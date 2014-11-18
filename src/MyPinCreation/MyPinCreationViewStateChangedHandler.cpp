// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#include "MyPinCreationViewStateChangedHandler.h"

namespace ExampleApp
{
    namespace MyPinCreation
    {
		MyPinCreationViewStateChangedHandler::MyPinCreationViewStateChangedHandler(IMyPinCreationModel& myPinCreationModel,
											 ExampleAppMessaging::UiToNativeMessageBus& uiToNativeMessageBus)
		: m_myPinCreationModel(myPinCreationModel)
		, m_uiToNativeMessageBus(uiToNativeMessageBus)
		, m_handler(this, &MyPinCreationViewStateChangedHandler::OnMyPinCreationViewStateChangedMessageReceived)
		{
			m_uiToNativeMessageBus.Subscribe(m_handler);
		}

		MyPinCreationViewStateChangedHandler::~MyPinCreationViewStateChangedHandler()
		{
			m_uiToNativeMessageBus.Unsubscribe(m_handler);
		}

		void MyPinCreationViewStateChangedHandler::OnMyPinCreationViewStateChangedMessageReceived(const MyPinCreationViewStateChangedMessage& message)
		{
			m_myPinCreationModel.SetCreationStage(message.GetMyPinCreationStage());
		}
    }
}