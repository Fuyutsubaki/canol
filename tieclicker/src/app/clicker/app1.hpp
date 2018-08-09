#pragma once

#include<vector>
#include<memory>
#include<variant>
#include"rules.hpp"
#include "coffin/util.hpp"


namespace app {
	using tie_float = double;

	struct Building {
		s3d::String name;
		std::size_t num;
	};


	struct Upgrade {

	};

	struct Data{
		tie_float bank;
		s3d::Array<Building> building{ BuildingMax };
		Upgrade upgrade;

		tie_float calc_tps() const{
			double sum = 0.1;
			for (size_t i = 0; i < building.size();i++)
			{
				sum += building[i].num * base_tps(i);
			}
			return sum;
		}
	};

	struct msgBuyBuilding{
		std::size_t num;
		double next_price;
	};

	class GameModel {
		std::shared_ptr<Data> dat_;

		cfn::basic_cnannel<tie_float> chChangedBank_;
		cfn::basic_cnannel<tie_float> chChangedTps_;
		cfn::basic_cnannel<std::tuple<std::size_t, msgBuyBuilding>> chChangedBuilding_;
	public:
		GameModel(std::shared_ptr<Data> const &dat)
			:dat_(dat)
		{		}

		// subscribeŒã‚É‚æ‚Ô‚±‚Æ
		// ReactiveProperty(–¢ŠJ”­) ‚É‹z‚í‚¹‚½‚¢
		void chInit() {
			for (std::size_t i = 0; i < dat_->building.size(); ++i) {
				auto &build = dat_->building[i];
				chChangedBuilding_.on_next({ i,{ build.num, base_price(build.num, i) } });
			}
			auto tps = dat_->calc_tps();
			chChangedTps_.on_next(tps);
			chChangedBank_.on_next(dat_->bank);
		}

		void tik(double sec){
			auto tps = dat_->calc_tps(); 
			chChangedTps_.on_next(tps);
			dat_->bank += tps * sec;
			chChangedBank_.on_next(dat_->bank);
		}

		void buy_building(std::size_t N) {
			auto &build = dat_->building[N];
			auto price = base_price(build.num, N);
			if (dat_->bank >= price) {
				dat_->bank -= price;
				build.num += 1;
				chChangedBuilding_.on_next({ N, { build.num, base_price(build.num, N) } });

			}
		}

		cfn::basic_cnannel<tie_float>& onChangedBank() {
			return chChangedBank_;
		}

		cfn::basic_cnannel<std::tuple<std::size_t, msgBuyBuilding>>& onChangedBuilding() {
			return chChangedBuilding_;
		}

		cfn::basic_cnannel<tie_float>& onChangedTps() {
			return chChangedTps_;
		}
	};





}