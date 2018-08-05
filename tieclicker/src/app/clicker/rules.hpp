#pragma once
#include<vector>
#include<array>

namespace app {
	constexpr std::size_t BuildingMax = 6;
	double base_price(std::size_t n, std::size_t id);
	double base_tps(std::size_t id);
}