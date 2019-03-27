#pragma once

#include <string>
#include <msclr/marshal_cppstd.h>

// Šg’£ŠÖ”‚ğ‚Ü‚Æ‚ß‚½‚à‚Ì
static std::string ToStdString(System::String^ str) {
	return msclr::interop::marshal_as<std::string>(str);
}

static System::String^ ToSystemString(std::string str) {
	return msclr::interop::marshal_as<System::String^>(str);
}

static std::wstring ToStdWString(System::String^ str) {
	return msclr::interop::marshal_as<std::wstring>(str);
}

static System::String^ ToSystemWString(std::wstring str) {
	return msclr::interop::marshal_as<System::String^>(str);
}