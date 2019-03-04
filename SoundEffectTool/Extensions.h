#pragma once

#include <vector>
#include <memory>

using namespace std;

class Extensions final sealed {

public:
	template<class T>
	static vector<unique_ptr<T>>&& MakeUniqueVector();

private:
	Extensions() {}
	~Extensions() {}
};
