#pragma once

#include <type_traits> 

namespace Val {
	//Neet idea to show ownership of initialized returned variables for other people working with the code
	//Based on Owner from https://github.com/Microsoft/GSL/blob/master/include/gsl/pointers

	template<class T = std::enable_if_t<std::is_pointer<T>::value>>

	using Owner = T;
}