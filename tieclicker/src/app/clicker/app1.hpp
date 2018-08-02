#pragma once

#include<vector>
#include<memory>
#include<variant>
#include "coffin/util.hpp"

namespace app {
	using tie_int = unsigned long long;
	constexpr std::size_t BuildingMax = 6;
	struct Building {
		enum { debug1, debug2, debug3 };
		s3d::String name;
		std::size_t num;
	};


	struct Upgrade {

	};

	struct Data{
		tie_int bank;
		s3d::Array<Building> building{ BuildingMax };
		Upgrade upgrade;

		tie_int calc_tps() const{
			return building.map([](Building const&item) {return item.num; }).sum();
		}

		tie_int calc_tpc() const {
			return 1;
		}
	};


	class GameModel {
		std::shared_ptr<Data> dat_;

		cfn::basic_cnannel<int> chChangedBank_;
		cfn::basic_cnannel<std::tuple<std::size_t, Building>> chChangedBuilding_;
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
			dat_->building[N].num += 1;
			chChangedBuilding_.on_next({ N, dat_->building[N] });
		}

		void buy_upgrade() {
			if (dat_->bank > 100) {
				;
			}
		}

		cfn::basic_cnannel<int>& onChangedBank() {
			return chChangedBank_;
		}

		cfn::basic_cnannel<std::tuple<std::size_t, Building>>& onChangedBuilding() {
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



	class BuildingView
		:public BoxObject
	{
	public:
		BuildingView(s3d::Point lt)
			:BoxObject({ lt,{200,80} })
		{		}

		s3d::String name;
		std::size_t num;
		std::size_t next_price;
		s3d::Font font{ 50 };
		void draw_impl() {
			font(num).draw();
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
			font(val_).draw();
			main_tie.draw();
			for (auto &build : builing_list_) {
				build.draw();
			}
		}

		void set_bank(tie_int val) {
			val_ = val;
		}

		void set_building(std::size_t N, Building const& msg) {
			builing_list_[N].num = msg.num;
		}

		cfn::basic_cnannel<std::monostate>& onClick() {
			return main_tie.onClick();
		}

		cfn::basic_cnannel<std::monostate>& onClickBuild(std::size_t N) {
			return builing_list_[N].onClick();
		}

	private:
		tie_int val_;
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