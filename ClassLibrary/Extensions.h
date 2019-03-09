#pragma once

#include <string>
#include <msclr/marshal_cppstd.h>

// �g���֐����܂Ƃ߂�����
class StringConvert {

public :

	static std::string ToStdString(System::String^ str) {
		return msclr::interop::marshal_as<std::string>(str);
	}

	static System::String^ ToSystemString(std::string str) {
		return msclr::interop::marshal_as<System::String^>(str);
	}
};
