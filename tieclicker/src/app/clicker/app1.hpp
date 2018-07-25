#pragma once

#include<vector>
#include<memory>
#include "coffin/util.hpp"

namespace app {
	using tie_int = unsigned long long;

	struct Building {

	};
	struct Upgrade {

	};

	struct Data{
		tie_int bank;
		Building building;
		Upgrade upgrade;

		tie_int calc_tps() const{
			return 1;
		}

		tie_int calc_tpc() const {
			return 1;
		}
	};


	class GameModel {
		std::shared_ptr<Data> dat_;

		cfn::basic_cnannel<int> click_;
	public:
		GameModel(std::shared_ptr<Data> const &dat)
			:dat_(dat)
		{


		}

		void click(){
			dat_->bank += dat_->calc_tpc();
			click_.on_next(dat_->bank);

		}
		void tik(){
			dat_->bank += dat_->calc_tps();
		}

		void buy_building() {
			if (dat_->bank > 100) {

			}
		}

		void buy_upgrade() {
			if (dat_->bank > 100) {
				;
			}
		}

		cfn::basic_cnannel<int>& as_click() {
			return click_;
		}
	};

	class GameView {
	public:
		GameView() {		}
		void update(){
			if (button.leftClicked()) {
				click.on_next(1);
			}
		}
		void draw(){
			font(val_).draw();
			button.drawFrame();
		}

		void set_bank(tie_int val) {
			val_ = val;
		}
		cfn::basic_cnannel<int>& get_channel() {
			return click;
		}

	private:
		tie_int val_;
		s3d::Font font{ 50 };

		s3d::Rect button{ 120,120,30,30 };

		cfn::basic_cnannel<int> click;
	};

}
