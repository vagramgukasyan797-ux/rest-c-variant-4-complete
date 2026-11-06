#include <cassert>
#include <cmath>

#include "../src/modules/individual/variant_mine.hpp"

int main() {
    using namespace TaskVariant;

    const Input control = parse(
        R"({"base_price":100.0,"competitor_price":110.0,"current_stock":5,
            "demand_surge_factor":1.2,"min_allowable_margin":10.0})");
    assert(validate(control).empty());
    assert(std::abs(calculate(control).optimized_price - 150.0) < 1e-12);

    const Input stock_boundary = parse(
        R"({"base_price":100,"competitor_price":20,"current_stock":10,
            "demand_surge_factor":0.5,"min_allowable_margin":0})");
    assert(validate(stock_boundary).empty());
    assert(std::abs(calculate(stock_boundary).optimized_price - 100.0) < 1e-12);

    const Input margin_boundary = parse(
        R"({"base_price":100,"competitor_price":110,"current_stock":20,
            "demand_surge_factor":0.5,"min_allowable_margin":90})");
    assert(validate(margin_boundary).empty());
    assert(std::abs(calculate(margin_boundary).optimized_price - 190.0) < 1e-12);

    const Input zero_stock = parse(
        R"({"base_price":100,"competitor_price":110,"current_stock":0,
            "demand_surge_factor":1,"min_allowable_margin":0})");
    assert(!validate(zero_stock).empty());

    const Input bad_price = parse(
        R"({"base_price":0,"competitor_price":110,"current_stock":5,
            "demand_surge_factor":1,"min_allowable_margin":0})");
    assert(!validate(bad_price).empty());

    const Input bad_demand = parse(
        R"({"base_price":100,"competitor_price":110,"current_stock":5,
            "demand_surge_factor":5.1,"min_allowable_margin":0})");
    assert(!validate(bad_demand).empty());
}
