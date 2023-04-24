#pragma once

#include <boost/ut.hpp>

#ifndef ENV_TEST
#define ENV_TEST
#endif

#include "../../include/logger.h"

namespace test = boost::ut;

#include <iostream>


namespace ft {
template<class TReporter>
struct runner : test::runner<TReporter> {

    template<class... Ts>
    auto on(test::events::test<Ts...> test) {
        std::cout << ">> " << test.type << "::" << test.name << '\n';
        test::runner<TReporter>::on(test);
        std::cout << "------------------------" << '\n';
    }

    using test::runner<TReporter>::on;
};
}  // namespace ft

template<class... Ts>
inline auto test::cfg<test::override, Ts...> = ft::runner<test::reporter<>>{};
