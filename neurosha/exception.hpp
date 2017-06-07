#pragma once

#include "head.h"


namespace nn{
	namespace exc {


		class exception
		{
		public:
			string_type message;
		public:
			exception() : message("something wrong ¯\\_(ツ)_/¯") {};

			exception(const char *str) : message(str) {};

			exception(string_type str) : message(str) {};

			auto wtf() const -> const string_type &{
				return this->message;
			}

		};

		class dimensional_mismatch : public exception
		{
		public:
			dimensional_mismatch(size_type size1, size_type size2)
			{
				this->message = string_type("dimensional mismatch : ")
					+ to_string(size1)
					+ string_type(" != ")
					+ to_string(size2);
			}
		};

		class activation_func_error : public exception
		{
		public:
			activation_func_error(string_type func_type)
			{
				this->message = string_type("it is strange version of ") +
								func_type +
								string_type("activation function");
			}
		};


	}
}

