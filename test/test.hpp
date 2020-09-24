#ifndef TEST_HPP_H_
#define TEST_HPP_H_

#include <catch/catch.hpp>

#ifdef ENABLE_MOCKING
#include <trompeloeil/trompeloeil.hpp>

// Ensure we only extern if we're not in the main file
#ifndef CATCH_CONFIG_MAIN
extern template struct trompeloeil::reporter<trompeloeil::specialized>;
#endif // CATCH_CONFIG_MAIN
#endif // ENABLE_MOCKING

#endif // TEST_HPP_H_
