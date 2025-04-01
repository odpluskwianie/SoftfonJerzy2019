#pragma once
#include "SoftfonJerzyCore.h"

using namespace System;


namespace SoftfonJerzyCore {
	public ref class StringChecker
	{
	public:
		virtual bool check(String^ string) = 0;
	};
	public ref class StringConsumer {
	public:
		virtual void consume(String^ string) = 0;
	};


	public delegate void callInfoDelegate(std::string);
	public delegate void regInfoDelegate(std::string);
	public delegate bool ifAnswerTheCallDelegate(std::string);

	public ref class SoftfonJerzyWrapper
	{
	private:
		callInfoDelegate^ managedCallInfoDelegate;
		regInfoDelegate^ managedRegInfoDelegate;
		ifAnswerTheCallDelegate^ managedIfAnswerTheCallDelegate;
		SoftfonJerzyCore* core;
		static const char * StringToConstCharArray(String^ string);
		StringConsumer^ callInfoUpdater;
		StringConsumer^ regInfoUpdater;
		StringChecker^ ifAnswerTheCallChecker;
		/*
		void_with_string_arg_funct_pointer updateCallInfo;
		void_with_string_arg_funct_pointer updateRegInfo;
		bool_with_string_arg_funct_pointer onIncomingCallNotification;
		*/
	public:
		void updateCallInfo(std::string callInfo);
		void updateRegInfo(std::string regInfo);
		bool checkIfAnswerTheCall(std::string callInfo);
		SoftfonJerzyWrapper(StringConsumer^ callInfoUpdater, StringConsumer^ regInfoUpdater, StringChecker^ ifAnswerTheCall);
		~SoftfonJerzyWrapper();
		!SoftfonJerzyWrapper();
		void registerAccount(String^ uri, String^ registrarUri, String^ user, String^ password);
		void call(String^ uri);
		void hangUpAllCalls();

		
	};

}
