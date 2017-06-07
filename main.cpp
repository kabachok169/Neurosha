#include <fstream>
#include <iostream>
#include <vector>
#include <list>
#include <boost/filesystem.hpp>
#include "neurosha/head.h"
#include "neurosha/functions.hpp"
#include "neurosha/vector.hpp"



namespace test {

    void activation_function(const char* directory)
    {
        auto str = [](nn::ftype ftype, nn::value_type h)
            {
                auto s = nn::string_type();

                switch(ftype) {
                    case nn::ftype::sigmoid:
                        s += "sigmoid";
                        break;
                    case nn::ftype::approximate_relu:
                        s += "real_ReLU";
                        break;
                    case nn::ftype::tangent:
                        s += "tangent";
                        break;
                }

                s += "(h=" + nn::to_string(size_t(h)) + ")";
                return s;
            };


        std::list<nn::ftype> ftypes = {nn::ftype::sigmoid, nn::ftype::approximate_relu, nn::ftype::tangent};
        std::list<nn::value_type> params = {1, 2, 4};
        std::map<nn::string_type, nn::function_t> f_map;

        for(auto ftype : ftypes)
        {
            for(auto h : params)
            {
                f_map[nn::string_type("fnc_") + str(ftype, h)] = nn::function(ftype, h);
                f_map[nn::string_type("der_") + str(ftype, h)] = nn::derivative(ftype, h);
            }
        }


        using namespace boost::filesystem;

        chdir("../");
        chdir("tests/");
        if (exists(directory))
            remove_all(directory);
        create_directory(directory);


        nn::value_type min_x = -10;
        nn::value_type max_x =  10;
        nn::value_type dx =  0.01;
        std::vector<nn::value_type> X;

        for(nn::value_type x = -10; x <= 10; x += 0.01)
            X.push_back(x);

        std::ofstream fileX(nn::string_type(directory) + "/X.txt");

        for(auto x : X)
            fileX << x << std::endl;

        for (auto it = f_map.begin(); it != f_map.end(); ++it)
        {
            std::ofstream file(nn::string_type(directory) + "/" + it->first + ".txt");
            for(auto x : X)
                file << (it->second)(x) << std::endl;
        }

    }
}

int main()
{
    auto len = 8;

    using namespace nn;

    vector r = make_vector(len, -1.0, 1.0);
    vector z = zero(len);
    vector u = unit(len);
    vector x = {1, 2, 3, 4, 5, 6, 7, 8};
    vector y = vector(len, [](){ return 2;} );

    std::cout << "r = " << r << std::endl;
    std::cout << "z = " << z << std::endl;
    std::cout << "u = " << u << std::endl;
    std::cout << "x = " << x << std::endl;
    std::cout << "y = " << y << std::endl;

    std::cout << "x + y = " << x + y << std::endl;
    std::cout << "x + 5 * y = " << x + 5 * y << std::endl;
    std::cout << "x - y = " << x - y << std::endl;
    std::cout << "-r = " << -r << std::endl;
    std::cout << "(u,x) = " << (u,x) << std::endl;
    std::cout << "(x,y) = " << (x,y) << std::endl;
    std::cout << "(x,2*u) = " << (x,2*u) << std::endl;
    std::cout << "(x,x) = " << (x,x) << std::endl;
    std::cout << "3*x = " << 3*x << std::endl;
    std::cout << "x*3 = " << 3*x << std::endl;

    std::cout << "x == y :" << std::boolalpha << " " << (x == y) << std::endl;
    std::cout << "x != y :" << std::boolalpha << " " << (x != y) << std::endl;
    std::cout << "x == (u,u) * x * 0.125 :" << " " << std::boolalpha << (x == (u,u) * 0.125 * x) << std::endl;

    test::activation_function( "Activation_Function" );
    return 0;
}