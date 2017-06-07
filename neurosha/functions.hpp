#pragma once

#include "head.h"
#include "exception.hpp"
#include <functional>
#include <cmath>
#include <map>

namespace nn {

    enum class ftype
    {
        sigmoid,
        approximate_relu,
        tangent
    };

    auto function(ftype type, value_type h = 1) -> function_t
    {
        using result_type = function_t::result_type;

        switch (type) {

            case ftype::sigmoid:
                return [h](value_type x) {
                    if (h <= 0)
                        throw exc::activation_func_error("sigmoid");
                    return static_cast<result_type>(1 / (1 + std::exp(-h*x)));
                };
            case ftype::approximate_relu:
                return [h](value_type x) {
                    if (h <= 0)
                        throw exc::activation_func_error("real ReLU");
                    return static_cast<result_type>(x + std::log(1 + std::exp(-h*x)) / h);
                };
            case ftype::tangent:
                return [h](value_type x) {
                    if (h <= 0)
                        throw exc::activation_func_error("tangent");
                    return static_cast<result_type>(1 / (2 * atan(1)) * atan(h*x));
                };
            default:
                return [h](value_type x){ return h*x; };
        }
    }

    auto derivative(ftype type, value_type h = 1) -> function_t
    {
        using result_type = function_t::result_type;

        switch (type) {

            case ftype::sigmoid:
                return [h](value_type x) {
                    if (h <= 0)
                        throw exc::activation_func_error("derivative(sigmoid)");
                    return static_cast<result_type>(h * std::exp(-h*x) / pow((1 + std::exp(-h*x)), 2));
                };
            case ftype::approximate_relu:
                return [h](value_type x) {
                    if (h <= 0)
                        throw exc::activation_func_error("derivative(real ReLU)");
                    return static_cast<result_type>(1 / (1 + std::exp(-h*x)));
                };
            case ftype::tangent:
                return [h](value_type x) {
                    if (h <= 0)
                        throw exc::activation_func_error("derivative(tangent)");
                    return static_cast<result_type>(h / (2 * atan(1) * ( 1 + pow(h*x, 2))));
                };
            default:
                return [h](value_type x){ return h; };
        }
    }
}