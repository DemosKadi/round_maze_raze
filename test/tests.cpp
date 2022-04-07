#include "../lib/position.h"
#include "../lib/controller_model.h"
#include <catch2/catch.hpp>

/*
unsigned int Factorial(unsigned int number)// NOLINT(misc-no-recursion)
{
  return number <= 1 ? number : Factorial(number - 1) * number;
}

TEST_CASE("Factorials are computed", "[factorial]")
{
  REQUIRE(Factorial(1) == 1);
  REQUIRE(Factorial(2) == 2);
  REQUIRE(Factorial(3) == 6);
  REQUIRE(Factorial(10) == 3628800);
}
 */

TEST_CASE("Testing Position distance", "[position][distance]")
{
  constexpr Position center{};

  // REQUIRE(center.distance({ 0, 0 }) == 0.0);
  REQUIRE(center.distance({ 1, 0 }) == 1.0);
  REQUIRE(center.distance({ 0, 1 }) == 1.0);
  REQUIRE(center.distance({ 3, 4 }) == 5.0);// NOLINT Magic Number
  REQUIRE(center.distance({ -1, 0 }) == 1.0);
  REQUIRE(center.distance({ 0, -1 }) == 1.0);
  REQUIRE(center.distance({ -3, -4 }) == 5.0);// NOLINT Magic Number
}

TEST_CASE("Testing Position radian", "[position][radian]")
{
  constexpr Position center{};

  // REQUIRE(center.radian({ 0, 0 }) == 0.0);
  REQUIRE(center.radian({ 1, 0 }) == Approx(3.141592));// NOLINT Magic Number
  REQUIRE(center.radian({ 0, 1 }) == Approx(-1.570796));// NOLINT Magic Number
  REQUIRE(center.radian({ 3, 4 }) == Approx(-2.214297));// NOLINT Magic Number
  REQUIRE(center.radian({ -1, 0 }) == 0.0);
  REQUIRE(center.radian({ 0, -1 }) == Approx(1.570796));// NOLINT Magic Number
  REQUIRE(center.radian({ -3, -4 }) == Approx(0.927295));// NOLINT Magic Number
}

TEST_CASE("Tilting Model distance", "[tilting_model][distance]")
{
  ControllerModel model{ { 0, 0 }, { 0, 0 }, 3 };

  REQUIRE(model.distance() == 0.0);

  model.mouse_position = { 0, 3 };
  REQUIRE(model.distance() == 3.0);// NOLINT Magic Number

  model.mouse_position = { 0, 4 };
  REQUIRE(model.distance() == 4.0);// NOLINT Magic Number

  model.mouse_position = { 3, 4 };
  REQUIRE(model.distance() == 5.0);// NOLINT Magic Number
}

TEST_CASE("Tilting Model limited_distance", "[tilting_model][limited_distance]")
{
  ControllerModel model{ { 0, 0 }, { 0, 0 }, 3 };

  REQUIRE(model.limited_distance() == 0.0);

  model.mouse_position = { 0, 3 };
  REQUIRE(model.limited_distance() == 3.0);// NOLINT Magic Number

  model.mouse_position = { 0, 4 };
  REQUIRE(model.limited_distance() == 3.0);// NOLINT Magic Number

  model.mouse_position = { 3, 4 };
  REQUIRE(model.limited_distance() == 3.0);// NOLINT Magic Number
}

TEST_CASE("Tilting Model radian", "[tilting_model][radian]")
{
  ControllerModel model{ { 0, 0 }, { 0, 0 }, 3 };

  REQUIRE(model.radian() == 0.0);

  model.mouse_position = { 1, 0 };
  REQUIRE(model.radian() == Approx(3.141592));// NOLINT Magic Number

  model.mouse_position = { 3, 4 };
  REQUIRE(model.radian() == Approx(-2.214297));// NOLINT Magic Number
}

TEST_CASE("Tilting Model limited_mouse_position", "[tilting_model][limited_mouse_position]")
{
  ControllerModel model{ { 0, 0 }, { 0, 0 }, 3 };

  REQUIRE(model.limited_mouse_position() == Position{ 0, 0 });

  model.mouse_position = { 3, 4 };
  REQUIRE(model.limited_mouse_position() == Position{ 2, 2 });

  model.mouse_position = { 1, 4 };
  REQUIRE(model.limited_mouse_position() == Position{ 1, 3 });
}
