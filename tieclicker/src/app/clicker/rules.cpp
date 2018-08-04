#include "rules.hpp"
namespace app{
	struct Datset
	{
		double price;
		double tps;
	};
	std::array<Datset, BuildingMax> conf = {
		Datset{10.,1},
		Datset{1000.,2<<26},
		Datset{ 100000. ,42},
		Datset{ 10000000. ,10000},
		Datset{ 1000000000. ,1500000},
		Datset{ 100000000000. ,1000000000}
	};

	double base_price(std::size_t n, std::size_t id) {
		std::array<double, BuildingMax> base_price = { 10.,1000.,100000.,10000000.,1000000000.,100000000000. };
		return conf[id].price * std::pow(1.12, n);
	}

	double base_tps(std::size_t id) {
		return conf[id].tps;
	}
}

