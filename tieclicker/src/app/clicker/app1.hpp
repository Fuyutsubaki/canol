#pragma once

#include<vector>
#include<memory>
#include<variant>
#include"def.hpp"
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
			double sum = 0;
			for (size_t i = 0; i < building.size();i++)
			{
				sum += building[i].num * base_tps(i);
			}
			return sum;
		}

		tie_float calc_tpc() const {
			return 1;
		}
	};

	struct msgBuyBuilding{
		std::size_t num;
		double next_price;
	};

	class GameModel {
		std::shared_ptr<Data> dat_;

		cfn::basic_cnannel<tie_float> chChangedBank_;
		cfn::basic_cnannel<std::tuple<std::size_t, msgBuyBuilding>> chChangedBuilding_;
	public:
		GameModel(std::shared_ptr<Data> const &dat)
			:dat_(dat)
		{		}

		void click(){
			dat_->bank += dat_->calc_tpc();
			chChangedBank_.on_next(dat_->bank);

		}
		void tik(){
			dat_->bank += dat_->calc_tps();
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
	};

	class BoxObject {
	public:
		virtual ~BoxObject(){}
		BoxObject(s3d::Rect rect)
			: rect_(rect)
		{}

		void update() {
			if (rect_.leftClicked()) {
				click_.on_next({});
			}
			{
				Transformer2D local{ s3d::Mat3x2::Identity().translated(rect_.tl()),true };

				// update_impl();
			}
		}

		cfn::basic_cnannel<std::monostate>& onClick() {
			return click_;
		}

		void draw() {
			rect_.drawFrame();
			{
				Transformer2D local{ s3d::Mat3x2::Identity().translated(rect_.tl()) };
				draw_impl();
			}
		}

		virtual void draw_impl() {};
	private:
		s3d::Rect rect_;
		cfn::basic_cnannel<std::monostate> click_;

	};

	s3d::String tienum_goodviwer(tie_float n) {
		constexpr std::size_t splitline = 4;
			
			// ex: 123 → 1.230E+3;
		if (n>10000) {
			auto d = (int)std::log10(n);
			s3d::String tmp = s3d::Format(s3d::DecimalPlace(3), (n / std::pow(10, d - 3)) / 1000.0);
			return s3d::Format(tmp, s3d::String( 5 - tmp.size(),'0' ), U"E+", d);
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
			:BoxObject({ lt,{200,80} })
		{		}

		s3d::String name;
		std::size_t num;
		tie_float next_price;
		s3d::Font font{ 50 };
		s3d::Font font_mini{ 15 };
		void draw_impl() {
			font(num).draw();
			font_mini(tienum_goodviwer(next_price)).draw(0,50);
		}
	};

	class GameView {
	public:
		GameView() {		}
		void update(){
			main_tie.update();
			for (auto &build : builing_list_) {
				build.update();
			}
		}
		void draw(){
			font(tienum_goodviwer(val_)).draw();
			main_tie.draw();
			for (auto &build : builing_list_) {
				build.draw();
			}
		}

		void set_bank(tie_float val) {
			val_ = val;
		}

		void set_building(std::size_t N, msgBuyBuilding const& msg) {
			builing_list_[N].num = msg.num;
			builing_list_[N].next_price = msg.next_price;
		}

		cfn::basic_cnannel<std::monostate>& onClick() {
			return main_tie.onClick();
		}

		cfn::basic_cnannel<std::monostate>& onClickBuild(std::size_t N) {
			return builing_list_[N].onClick();
		}

	private:
		tie_float val_;
		s3d::Font font{ 50 };
		BoxObject main_tie{ { 120,120,30,30 }};
		std::array<BuildingView, BuildingMax> builing_list_ = {
			s3d::Point{480,0},
			 s3d::Point{ 480,80 } ,
			 s3d::Point{ 480,80 * 2 } ,
			 s3d::Point{ 480,80 * 3 } ,
			 s3d::Point{ 480,80 * 4 },
			 s3d::Point{ 480,80 * 5 }
		};

	};

}