// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPL-3.0-only OR Embedded Virtual Machine Commercial License

#include "instance_list.hpp"
#include <catch2/catch.hpp>

using namespace embutil;

TEST_CASE("Create dynamic instance list", "[utility/instance_list]")
{
	DynamicInstanceList<int> l;

	CHECK(0 == l.size());
	CHECK(0 < l.capacity());
}

TEST_CASE("Add/remove single without key to dynamic instance list", "[utility/instance_list]")
{
	DynamicInstanceList<int> l;
	int x = 32;

	l.add(&x);

	for(auto& t : l.rawStorage())
	{
		CHECK(t.value == &x);
		CHECK(*(t.value) == x);
	}

	CHECK(1 == l.size());

	l.remove(&x);

	CHECK(0 == l.size());
}

TEST_CASE("Add multiple, remove single without key from dynamic instance list",
		  "[utility/instance_list]")
{
	DynamicInstanceList<int> l;
	int x = 32, y = 28, z = 18;

	l.add(&x);
	l.add(&y);
	l.add(&z);

	CHECK(3 == l.size());

	l.remove(&x);

	for(auto& t : l.rawStorage())
	{
		CHECK(t.value != &x);
		CHECK(*(t.value) != x);
	}

	CHECK(2 == l.size());
}

TEST_CASE("Add multiple by name, remove single by ptr from dynamic instance list",
		  "[utility/instance_list]")
{
	DynamicInstanceList<int> l;
	int x = 32, z = 18;

	l.add("x", &x);
	l.add("y", &x);
	l.add("z", &z);

	CHECK(x == *l["y"].value());
	CHECK(z == *l.find("z").value());
	CHECK(3 == l.size());

	l.remove("y", &x);

	CHECK(2 == l.size());
	CHECK(x == *l["x"].value());
}

TEST_CASE("Create static instance list", "[utility/instance_list]")
{
	StaticInstanceList<int, 32> l;

	CHECK(0 == l.size());
	CHECK(32 == l.capacity());
}

TEST_CASE("Add/remove single without key to static instance list", "[utility/instance_list]")
{
	StaticInstanceList<int> l;
	int x = 32;

	l.add(&x);

	for(auto& t : l.rawStorage())
	{
		CHECK(t.value == &x);
		CHECK(*(t.value) == x);
	}

	CHECK(1 == l.size());

	l.remove(&x);

	CHECK(0 == l.size());
}

TEST_CASE("Add multiple, remove single without key from static instance list",
		  "[utility/instance_list]")
{
	StaticInstanceList<int> l;
	int x = 32, y = 28, z = 18;

	l.add(&x);
	l.add(&y);
	l.add(&z);

	CHECK(3 == l.size());

	l.remove(&x);

	for(auto& t : l.rawStorage())
	{
		CHECK(t.value != &x);
		CHECK(*(t.value) != x);
	}

	CHECK(2 == l.size());
}

TEST_CASE("Add multiple by name, remove single by ptr from static instance list",
		  "[utility/instance_list]")
{
	StaticInstanceList<int> l;
	int x = 32, z = 18;

	l.add("x", &x);
	l.add("y", &x);
	l.add("z", &z);

	CHECK(x == *l["y"].value());
	CHECK(z == *l.find("z").value());
	CHECK(3 == l.size());

	l.remove("y", &x);

	CHECK(2 == l.size());
	CHECK(x == *l["x"].value());
}
