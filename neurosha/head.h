#pragma once

#include <string>
#include <functional>

namespace nn {

	using value_type = double;
	using size_type = std::size_t;
	using string_type = std::string;
	using function_t = std::function<value_type(value_type)>;

	using std::to_string;

	namespace exc {}
}

