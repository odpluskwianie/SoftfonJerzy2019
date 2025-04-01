#pragma once

#include <iostream>
#include <string>
#include "pjsua2.hpp"
//#include "pjlib.h"
#include "SoftfonJerzyCore.h"

namespace SoftfonJerzyCore {
	class SJEndpoint;
	void SoftfonJerzyCore::registerThread() {
			if (&pj::Endpoint::libIsThreadRegistered == false) {
				pj::Endpoint::instance().libRegisterThread("thread");
			}
	}

	public class SJCallbacks {
	public:
		static void_string_func_ptr updateCallInfoUICallback, updateRegInfoUICallback;
		static bool_string_func_ptr onIncomingCallUICallback;

	};
	void_string_func_ptr SJCallbacks::updateCallInfoUICallback = nullptr;
	void_string_func_ptr SJCallbacks::updateRegInfoUICallback = nullptr;
	bool_string_func_ptr SJCallbacks::onIncomingCallUICallback = nullptr;


	class SJAccount;

	public class SJCall : public pj::Call
	{
	private:
		SJAccount *myAcc;

	public:
		SJCall(pj::Account &acc, int call_id = PJSUA_INVALID_ID)
			: Call(acc, call_id)
		{
			//registerThread();
			myAcc = (SJAccount *)&acc;
		}
		~SJCall() {
			//registerThread();
		}
		virtual void onCallState(pj::OnCallStateParam &prm);
		virtual void onCallMediaState(pj::OnCallMediaStateParam & prm);
	};

	public class SJAccount : public pj::Account
	{
	public:
		std::vector<pj::Call *> calls;

	public:
		SJAccount()
		{
			//registerThread();
		}

		~SJAccount()
		{
			shutdown();
			
			//registerThread();
			std::cout << "*** Account is being deleted: No of calls="
				<< calls.size() << std::endl;
		}

		void removeCall(pj::Call *call)
		{
			//registerThread();
			for (std::vector<pj::Call *>::iterator it = calls.begin();
				it != calls.end(); ++it)
			{
				if (*it == call) {
					calls.erase(it);
					break;
				}
			}
		}

		virtual void onRegState(pj::OnRegStateParam &prm)
		{
			//registerThread();
			pj::AccountInfo ai = getInfo();
			std::cout << (ai.regIsActive ? "*** Register: code=" : "*** Unregister: code=")
				<< prm.code << std::endl;
			std::string regInfo = (std::string)(ai.regIsActive ? "Zarejestrowany (kod: " : "Niezarejestrowany (kod: ");
			regInfo += std::to_string(prm.code);
			regInfo += " " + ai.regStatusText + ")";
			SJCallbacks::updateRegInfoUICallback(regInfo);
		}

		virtual void onIncomingCall(pj::OnIncomingCallParam &iprm);
		
	};

	public class SJEndpoint : public pj::Endpoint{
	public:
		static SJAccount * account;
	};
	SJAccount* SJEndpoint::account = nullptr;

	void SJAccount::onIncomingCall(pj::OnIncomingCallParam &iprm){
		//registerThread();
		pj::Call *call = new SJCall(*this, iprm.callId);
		pj::CallInfo ci = call->getInfo();
		pj::CallOpParam prm;

		std::cout << "*** Incoming Call: " << ci.remoteUri << " ["
			<< ci.stateText << "]" << std::endl;

		/*prm.statusCode = (pjsip_status_code)180;
		for (int i = 0; i < 50; i++) {
			call->answer(prm);
			System::Threading::Thread::Sleep(500);
		}*/

		if (SJEndpoint::account->calls.size() != 0) {
			prm.statusCode = PJSIP_SC_BUSY_HERE;
			call->answer(prm);
			return;
		}

		calls.push_back(call);

		std::string callInfo = "Po³¹czenie przychodz¹ce od:" + ci.remoteUri;
		if (SJCallbacks::onIncomingCallUICallback(callInfo)) {
			prm.statusCode = PJSIP_SC_OK;//(pjsip_status_code)200;
		}

		else {
			prm.statusCode = PJSIP_SC_DECLINE;//(pjsip_status_code)603;
		}

		call->answer(prm);
	}

	void SJCall::onCallState(pj::OnCallStateParam &prm)
	{
		//registerThread();
		PJ_UNUSED_ARG(prm);

		pj::CallInfo ci = getInfo();
		std::cout << "*** Call: " << ci.remoteUri << " [" << ci.stateText
			<< "]" << std::endl;
		std::string callInfo = "Po³¹czenie z " + ci.remoteUri + ",\nstan:" + ci.stateText;
		SJCallbacks::updateCallInfoUICallback(callInfo);
		if (ci.state == PJSIP_INV_STATE_DISCONNECTED) {
			myAcc->removeCall(this);
			// Delete the call 
			delete this;
		}
	}

	void SJCall::onCallMediaState(pj::OnCallMediaStateParam &prm) {
		//registerThread();
		pj::CallInfo ci = getInfo();
		//Iterate all the call medias
		for (unsigned i = 0; i < ci.media.size(); i++) {
			if (ci.media[i].type == PJMEDIA_TYPE_AUDIO && getMedia(i)) {
				pj::AudioMedia *aud_med = (pj::AudioMedia *)getMedia(i);

				//Connect the call audio media to sound device
				pj::AudDevManager& mgr = pj::Endpoint::instance().audDevManager();
				aud_med->startTransmit(mgr.getPlaybackDevMedia());
				mgr.getCaptureDevMedia().startTransmit(*aud_med);
			}
		}
	}

	void SoftfonJerzyCore::assignCallbacks(
		void_string_func_ptr updateCallInfoUICallback,
		void_string_func_ptr updateRegInfoUICallback,
		bool_string_func_ptr onIncomingCallUICallback
	) {
		this->updateCallInfoUICallback = updateCallInfoUICallback;
		SJCallbacks::updateCallInfoUICallback = updateCallInfoUICallback;
		this->updateRegInfoUICallback = updateRegInfoUICallback;
		SJCallbacks::updateRegInfoUICallback = updateRegInfoUICallback;
		this->onIncomingCallUICallback = onIncomingCallUICallback;
		SJCallbacks::onIncomingCallUICallback = onIncomingCallUICallback;
	}

	SoftfonJerzyCore::SoftfonJerzyCore(/*
		void_string_consumer_ptr updateCallInfoUICallback,
		void_string_consumer_ptr updateRegInfoUICallback,
		bool_string_checker_ptr onIncomingCallUICallback*/) {
		int ret = 0;
		pj::Endpoint * ep = new SJEndpoint();

		try {
			ep->libCreate();

			pj::EpConfig ep_cfg;
			ep_cfg.logConfig.level = 4;
			ep->libInit(ep_cfg);

			// Transport
			pj::TransportConfig tcfg;
			tcfg.port = 5060;
			ep->transportCreate(PJSIP_TRANSPORT_UDP, tcfg);

			// Start library
			ep->libStart();
			std::cout << "*** PJSUA2 STARTED ***" << std::endl;

			ret = PJ_SUCCESS;
		}
		catch (pj::Error & err) {
			std::cout << "Exception: " << err.info() << std::endl;
			ret = 1;
		}
	}
	SoftfonJerzyCore::~SoftfonJerzyCore() {
		registerThread();
		pj::Endpoint::instance().hangupAllCalls();

		pj_thread_sleep(4000);

		std::cout << "*** PJSUA2 SHUTTING DOWN ***" << std::endl;
		try {
			if (SJEndpoint::account)
				//delete call;
				delete SJEndpoint::account;
		}
		catch (pj::Error &err) {
			std::cout << "Error deleting object" << std::endl;
		}
	}

	void SoftfonJerzyCore::registerAccount(const char *uri, const char *registrarUri, const char *user, const char *password) {
		registerThread();
		pj::AccountConfig acc_cfg;
		acc_cfg.idUri = uri;//"sip:222";//"sip:222@192.168.0.111";
		acc_cfg.regConfig.registrarUri = registrarUri;//"sip:192.168.0.111";
		const std::string  userString = std::string(user);
		const std::string  passwordString = std::string(password);
		acc_cfg.sipConfig.authCreds.push_back(pj::AuthCredInfo("digest", "*",
			userString, 0, passwordString));

		
		if (SJEndpoint::account) {
			//hangUpAllCalls();
			if (SJEndpoint::account->calls.size() != 0) {
				SJCallbacks::updateRegInfoUICallback("Najpierw zakoñcz bie¿¹ce po³¹czenie.");
				return;
			}
			delete SJEndpoint::account;
		}
		SJAccount *acc(new SJAccount);
		acc->create(acc_cfg);
		SJEndpoint::account = acc;
	}

	void SoftfonJerzyCore::call(const char *uri) {
		registerThread();

		if (!SJEndpoint::account) {
			SJEndpoint::account = new SJAccount();
			/////////////////////////////////////////////
			// Acc config - ip hosta?
			//System::Net::IPAddress::GetAddressBytes;
			pj::AccountConfig acc_cfg;
			acc_cfg.idUri = "sip:SoftfonJerzy";

			//MyAccount *acc = new MyAccount;
			try {
				SJEndpoint::account->create(acc_cfg);

			}
			catch (pj::Error& err) {
				std::cout << "Account creation error: " << err.info() << std::endl;
			}
			return;
		}
		pj::Call *call = new SJCall(*SJEndpoint::account);
		SJEndpoint::account->calls.push_back(call);
		pj::CallOpParam prm(true);
		prm.opt.audioCount = 1;
		prm.opt.videoCount = 0;
		const std::string * uriString = new std::string(uri);
		call->makeCall(*uriString, prm);
	}
	void SoftfonJerzyCore::hangUpAllCalls() {
		registerThread();
		pj::Endpoint::instance().hangupAllCalls();
	}
}