#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <algorithm>
#include <random>

#include "head.h"
#include "exception.hpp"

namespace nn {

	class vector
	{
	public:
		using value_type = nn::value_type;
		using data_type = std::vector<value_type>;
		using init_func_type = std::function<value_type()>;


		vector(size_type size) : data(size)
		{
			std::for_each(data.begin(), data.end(), [](value_type& v) { v = 0; });
		}

		vector(size_type size, init_func_type function) : data(size)
		{
			std::for_each(data.begin(), data.end(), [function](value_type& v) { v = function(); });
		}

		vector(const vector& vector) : data(vector.data) {}

		vector(nn::vector&& vector) : data(std::move(vector.data)) {}

		vector(const vector::data_type& vector_data) : data(vector_data) {}

		vector(vector::data_type&& vector_data) : data(std::move(vector_data)) {}

		vector(std::initializer_list<value_type> init_list) : data(init_list) {}

		vector& operator= (const vector& vector)
		{
			if (this == &vector)
				return *this;

			data = vector.data;

			return *this;
		}

		vector& operator= (nn::vector&& vector)
		{
			if (this == &vector)
				return *this;

			data = std::move(vector.data);

			return *this;
		}

		auto size() const -> size_type
		{
			return data.size();
		}

		auto operator[](size_type index) const -> const value_type&
		{
			return data[index];
		}

		auto operator[](size_type index) -> value_type&
		{
			return const_cast<value_type&>(static_cast<const vector&>(*this)[index]);
		}

		auto operator+=(const vector& rhs) -> vector&
		{
			auto size = this->size();

			if (size != rhs.size())
				throw nn::exc::dimensional_mismatch(size, rhs.size());

			for (size_type i = 0; i < size; ++i)
				data[i] += rhs.data[i];

			return *this;
		}

		auto operator-=(const vector& rhs) -> vector&
		{
			auto size = this->size();

			if (size != rhs.size())
				throw nn::exc::dimensional_mismatch(size, rhs.size());

			for (size_type i = 0; i < size; ++i)
				data[i] -= rhs.data[i];

			return *this;
		}

		auto operator*=(value_type coefficient) -> vector&
		{
			for (size_type i = 0; i < size(); ++i)
				data[i] *= coefficient;

			return *this;
		}

		friend auto operator-(const vector& rhs) -> vector
		{
			vector result(rhs.size());

			for (size_type i = 0; i < rhs.size(); ++i)
				result[i] = -rhs[i];

			return result;
		}

		friend auto operator*(const vector& v, value_type coefficient) -> vector
		{
			auto result = v;
			result *= coefficient;
			return result;
		}

		friend auto operator*(value_type coefficient, const vector& v) -> vector
		{
			auto result = v;
			result *= coefficient;
			return result;
		}

		friend auto operator+(const vector& lhs, const vector& rhs) -> vector
		{
			auto result = lhs;
			result += rhs;
			return result;
		}

		friend auto operator-(const vector& lhs, const vector& rhs) -> vector
		{
			auto result = lhs;
			result -= rhs;
			return result;
		}

		friend auto operator==(const vector& lhs, const vector& rhs) -> bool
		{
			auto size = lhs.size();

			if (size != rhs.size())
				throw nn::exc::dimensional_mismatch(size, rhs.size());

			for (auto i = 0; i < size; ++i)
				if (lhs[i] != rhs[i])
					return false;

			return true;
		}

		friend auto operator!=(const vector& lhs, const vector& rhs) -> bool
		{
			auto size = lhs.size();

			if (size != rhs.size())
				throw nn::exc::dimensional_mismatch(size, rhs.size());
			for (auto i = 0; i < size; ++i)
				if (lhs[i] != rhs[i])
					return true;

			return false;
		}

		friend auto operator,(const vector& lhs, const vector& rhs) -> value_type
		{
			auto size = lhs.size();

			if (size != rhs.size())
				throw nn::exc::dimensional_mismatch(size, rhs.size());

			value_type scalar(0);

			for (size_type i = 0; i < size; ++i)
				scalar += lhs[i] * rhs[i];

			return scalar;
		}

		friend std::ostream& operator<<(std::ostream& ostr, const vector& v)
		{
			ostr << "(  ";

			for (size_type i = 0; i < v.size(); ++i)
				ostr << to_string(v[i]) << "  ";

			ostr << ")";

			return ostr;
		}


	private:
		data_type data;
	};



	vector zero(size_type size)
	{
		return vector(size);
	}

	vector unit(size_type size)
	{
		vector vector(size);

		for (size_type i = 0; i < size; ++i)
			vector[i] = nn::vector::value_type(1);

		return vector;
	}

	vector make_vector(size_type size, value_type min_value, value_type max_value)
	{
		std::default_random_engine engine; // Let's trust the compiler

        std::uniform_real_distribution<nn::value_type> distribution(min_value, max_value);

        return vector(size, [&distribution, &engine]() { return distribution(engine); });;


	}

}

