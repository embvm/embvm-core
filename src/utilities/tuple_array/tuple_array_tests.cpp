#include "tuple_array.hpp"
#include <catch2/catch.hpp>

using namespace embutil;

static auto generate_tuple(void)
{
	auto my_tuple = std::make_tuple(int{10}, int{100});

	return std::tuple_cat(my_tuple, std::make_tuple(int{104848}));
}

TEST_CASE("Check tuple to array conversion", "[util/tuple_array]")
{
	using array_t = std::array<int, 3>;
	array_t reference{200, 100, static_cast<int>('z')};
	array_t val =
		tuple_to_array<int>(std::tuple<unsigned long long, unsigned, char>{200, 100, 'z'});

	for(size_t i = 0; i < val.size(); i++)
	{
		CHECK(reference.at(i) == val.at(i));
	}
}

TEST_CASE("Check tuple to array conversion with a concat tuple", "[util/tuple_array]")
{
	using array_t = std::array<int, 3>;
	array_t reference{10, 100, 104848};

	auto val = tuple_to_array<int>(generate_tuple());

	for(size_t i = 0; i < val.size(); i++)
	{
		CHECK(reference.at(i) == val.at(i));
	}
}
