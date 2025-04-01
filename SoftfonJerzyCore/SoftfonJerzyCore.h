#pragma once
#include <iostream>

typedef void(__stdcall * void_string_func_ptr)(std::string arg);
typedef bool(__stdcall * bool_string_func_ptr)(std::string arg);

namespace SoftfonJerzyCore {
	public class SoftfonJerzyCore {
	private:
		void_string_func_ptr updateCallInfoUICallback;
		void_string_func_ptr updateRegInfoUICallback;
		bool_string_func_ptr onIncomingCallUICallback;
	public:
		static void registerThread();
		void assignCallbacks(
			void_string_func_ptr updateCallInfoUICallback,
			void_string_func_ptr updateRegInfoUICallback,
			bool_string_func_ptr onIncomingCallUICallback
		);
		SoftfonJerzyCore(/*
			void_string_consumer_ptr updateCallInfoUICallback,
			void_string_consumer_ptr updateRegInfoUICallback,
			bool_string_checker_ptr onIncomingCallUICallback*/);
		~SoftfonJerzyCore();
		void registerAccount(const char *uri, const char *registrarUri, const char *user, const char *password);
		void call(const char *uri);
		void hangUpAllCalls();
	};
	/*
	void_string_func_ptr SoftfonJerzyCore::updateCallInfoUICallback = nullptr;
	void_string_func_ptr SoftfonJerzyCore::updateRegInfoUICallback = nullptr;
	bool_string_func_ptr SoftfonJerzyCore::onIncomingCallUICallback = nullptr;*/
}