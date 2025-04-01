#include "stdafx.h"
#include "CLRWrapper.h"
using namespace System;
using namespace System::Runtime::InteropServices;

namespace SoftfonJerzyCore {


	void SoftfonJerzyWrapper::updateCallInfo(std::string callInfo) {
		String^ clrCallInfo = gcnew String(callInfo.c_str());
		callInfoUpdater->consume(clrCallInfo);
	}
	void SoftfonJerzyWrapper::updateRegInfo(std::string regInfo) {
		String^ clrRegInfo = gcnew String(regInfo.c_str());
		regInfoUpdater->consume(clrRegInfo);
	}
	bool  SoftfonJerzyWrapper::checkIfAnswerTheCall(std::string callInfo) {
		String^ clrCallInfo = gcnew String(callInfo.c_str());
		if (ifAnswerTheCallChecker->check(clrCallInfo)) {
			return true;
		}
		return false;
	}

	const char * SoftfonJerzyWrapper::StringToConstCharArray(String^ string)
	{
		const char* arrayPointer = 
			(const char*)(System::Runtime::InteropServices::
				Marshal::
				StringToHGlobalAnsi(string)).ToPointer();
		return arrayPointer;
	}


	SoftfonJerzyWrapper::SoftfonJerzyWrapper(StringConsumer^ callInfoUpdater, StringConsumer^ regInfoUpdater, StringChecker^ ifAnswerTheCallChecker)
	{
		this->callInfoUpdater = callInfoUpdater;
		this->regInfoUpdater = regInfoUpdater;
		this->ifAnswerTheCallChecker = ifAnswerTheCallChecker;
		core = new SoftfonJerzyCore(/*
			updateCallInfo,
			updateRegInfo,
			checkIfAnswerTheCall*/);

		//CliCpp::OnConnectDelegate^ managed_on_connect = gcnew CliCpp::OnConnectDelegate(this, &WrapperClass::OnConnect);
		managedCallInfoDelegate = gcnew callInfoDelegate(this, &SoftfonJerzyWrapper::updateCallInfo);
		managedRegInfoDelegate = gcnew regInfoDelegate(this, &SoftfonJerzyWrapper::updateRegInfo);
		managedIfAnswerTheCallDelegate = gcnew ifAnswerTheCallDelegate(this, &SoftfonJerzyWrapper::checkIfAnswerTheCall);
		/*
		IntPtr stub_ptr = Marshal::GetFunctionPointerForDelegate(managed_on_connect);
		CppClass::Callback fptr = static_cast<CppClass::Callback>(stub_ptr.ToPointer());
		m_native->SetOnCallback(fptr);
		GC::KeepAlive(managed_on_connect);
		*/
		IntPtr stubPtrCallInfo = Marshal::GetFunctionPointerForDelegate(managedCallInfoDelegate);
		IntPtr stubPtrRegInfo = Marshal::GetFunctionPointerForDelegate(managedRegInfoDelegate);
		IntPtr stubPtrIfAnswerCall = Marshal::GetFunctionPointerForDelegate(managedIfAnswerTheCallDelegate);

		void_string_func_ptr ptrCallInfo = static_cast<void_string_func_ptr>(stubPtrCallInfo.ToPointer());
		void_string_func_ptr ptrRegInfo = static_cast<void_string_func_ptr>(stubPtrRegInfo.ToPointer());
		bool_string_func_ptr ptrIfAnsCall = static_cast<bool_string_func_ptr>(stubPtrIfAnswerCall.ToPointer());

		core->assignCallbacks(ptrCallInfo, ptrRegInfo, ptrIfAnsCall);

	}

	SoftfonJerzyWrapper::~SoftfonJerzyWrapper()
	{
		//core->registerThread();
		delete core;
	}

	SoftfonJerzyWrapper::!SoftfonJerzyWrapper()
	{
		//core->registerThread();
		delete core;
	}

	void SoftfonJerzyWrapper::registerAccount(String^ uri, String^ registrarUri, String^ user, String^ password)
	{
		const char * ccuri, *ccregistratUri, *ccuser, *ccpassword;
		ccuri = StringToConstCharArray(uri);
		ccregistratUri = StringToConstCharArray(registrarUri);
		ccuser = StringToConstCharArray(user);
		ccpassword = StringToConstCharArray(password);
		core->registerAccount(ccuri, ccregistratUri, ccuser, ccpassword);
	}

	void SoftfonJerzyWrapper::call(String^ uri)
	{
		core->call(StringToConstCharArray(uri));
	}

	void SoftfonJerzyWrapper::hangUpAllCalls()
	{
		core->hangUpAllCalls();
	}



}
