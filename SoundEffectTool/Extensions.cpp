#include "stdafx.h"
#include "Extensions.h"

template<class T>
vector<unique_ptr<T>>&& Extensions::MakeUniqueVector() {
	return vector<unique_ptr<T>>;
}