#pragma once
// ============================================================================
// ВАРИАНТ 4: Dynamic Pricing Core.
// Расчёт оптимальной цены с учётом спроса, остатка и цены конкурента.
// ============================================================================

#include <cmath>
#include <string>

#include "../../json.hpp"

namespace TaskVariant {

    struct Input {
        double base_price;
        double competitor_price;
        int current_stock;
        double demand_surge_factor;
        double min_allowable_margin;
    };

    struct Output {
        double optimized_price;
    };

    inline std::string validate(const Input& in) {
        if (!std::isfinite(in.base_price) || in.base_price <= 0.0)
            return "base_price must be > 0.0";
        if (!std::isfinite(in.competitor_price) ||
            in.competitor_price < in.base_price * 0.2)
            return "competitor_price must be >= base_price * 0.2";
        if (in.current_stock <= 0)
            return "current_stock must be > 0";
        if (!std::isfinite(in.demand_surge_factor) ||
            in.demand_surge_factor < 0.5 || in.demand_surge_factor > 5.0)
            return "demand_surge_factor must be in [0.5, 5.0]";
        if (!std::isfinite(in.min_allowable_margin) ||
            in.min_allowable_margin < 0.0 || in.min_allowable_margin > 90.0)
            return "min_allowable_margin must be in [0.0, 90.0]";
        return {};
    }

    inline Output calculate(const Input& in) {
        Output out{};
        const double scarcity_multiplier = (in.current_stock < 10) ? 1.25 : 1.0;

        out.optimized_price =
            in.base_price * in.demand_surge_factor * scarcity_multiplier;

        const double competitor_floor = in.competitor_price * 0.95;
        if (out.optimized_price < competitor_floor)
            out.optimized_price = competitor_floor;

        const double margin_floor =
            in.base_price * (1.0 + in.min_allowable_margin / 100.0);
        if (out.optimized_price < margin_floor)
            out.optimized_price = margin_floor;

        return out;
    }

    inline Input parse(const std::string& body) {
        Input in{};

        auto get_number = [&](const std::string& key) -> double {
            const auto key_pos = body.find('"' + key + '"');
            if (key_pos == std::string::npos) return 0.0;
            const auto colon_pos = body.find(':', key_pos);
            if (colon_pos == std::string::npos) return 0.0;

            try {
                std::size_t consumed = 0;
                const double value = std::stod(body.substr(colon_pos + 1), &consumed);
                if (consumed == 0 || !std::isfinite(value)) return 0.0;
                return value;
            } catch (...) {
                return 0.0;
            }
        };

        in.base_price = get_number("base_price");
        in.competitor_price = get_number("competitor_price");
        in.current_stock = static_cast<int>(get_number("current_stock"));
        in.demand_surge_factor = get_number("demand_surge_factor");
        in.min_allowable_margin = get_number("min_allowable_margin");
        return in;
    }

    inline std::string to_json(const Output& out) {
        JObj object;
        object.num("optimized_price", out.optimized_price);
        return object.str();
    }

} // namespace TaskVariant
