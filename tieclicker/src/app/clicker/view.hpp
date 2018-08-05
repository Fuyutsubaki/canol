#pragma once
#include "app1.hpp"

namespace app {
	s3d::String tienum_goodviwer(tie_float n) {
		constexpr std::size_t splitline = 4;

		// ex: 123 → 1.230E+3;
		if (n>10000) {
			auto d = (int)std::log10(n);
			s3d::String tmp = s3d::Format(s3d::DecimalPlace(3), (n / std::pow(10, d - 3)) / 1000.0);
			return s3d::Format(tmp, s3d::String(5 - tmp.size(), '0'), U"E+", d);
			// return s3d::Format(s3d::DecimalPlace(3), (n / std::pow(10, d - 3)) / 1000.0, U"E+", d);// 1.000 みたいなとき文字長変わってカクカクする
		}
		else {
			return s3d::Format(int(n));
		}

	}


	class BuildingView
		:public BoxObject
	{
	public:
		BuildingView(s3d::Point lt)
			:BoxObject({ lt,{ 200,80 } })
		{		}

		s3d::String name;
		std::size_t num;
		tie_float next_price;
		s3d::Font font{ 50 };
		s3d::Font font_mini{ 15 };
		void draw_impl() {
			font(num).draw();
			font_mini(tienum_goodviwer(next_price)).draw(0, 50);
		}
	};

	class GameView {
	public:
		GameView() {		}
		void update() {
			for (auto &build : builing_list_) {
				build.update();
			}
		}
		void draw() {
			font(tienum_goodviwer(val_)).draw();
			font(U"per second:+", tienum_goodviwer(tps_)).draw(0, 100);
			for (auto &build : builing_list_) {
				build.draw();
			}
		}

		void set_bank(tie_float val) {
			val_ = val;
		}
		void set_tps(tie_float val) {
			tps_ = val;
		}

		void set_building(std::size_t N, msgBuyBuilding const& msg) {
			builing_list_[N].num = msg.num;
			builing_list_[N].next_price = msg.next_price;
		}

		cfn::basic_cnannel<std::monostate>& onClickBuild(std::size_t N) {
			return builing_list_[N].onClick();
		}

	private:
		tie_float val_;
		tie_float tps_;
		s3d::Font font{ 50 };

		std::array<BuildingView, BuildingMax> builing_list_ = {
			s3d::Point{ 480,0 },
			s3d::Point{ 480,80 } ,
			s3d::Point{ 480,80 * 2 } ,
			s3d::Point{ 480,80 * 3 } ,
			s3d::Point{ 480,80 * 4 },
			s3d::Point{ 480,80 * 5 }
		};

	};
}