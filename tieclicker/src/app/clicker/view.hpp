#pragma once
#include "app1.hpp"

namespace app {
	s3d::String tienum_goodviwer(tie_float n) {
		// ex: 123 → 1.230E+3;
		if (n>10000) {
			auto d = (int)std::log10(n);
			s3d::String tmp = s3d::Format(s3d::DecimalPlace(3), (n / std::pow(10, d - 3)) / 1000.0);
			return s3d::Format(tmp, s3d::String(5 - tmp.size(), '0'), U"E+", d);
			// return s3d::Format(s3d::DecimalPlace(3), (n / std::pow(10, d - 3)) / 1000.0, U"E+", d);// 1.000 みたいなとき文字長変わってカクカクする
		}
		else {
			return s3d::Format(s3d::DecimalPlace(1), n);
		}

	}
	class BoxObject {
	public:
		virtual ~BoxObject() {}
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
				Transformer2D local{ s3d::Mat3x2::Identity().translated(rect_.tl()),true };
				draw_impl();
			}
		}
		s3d::Rect rect_;
	protected:
		s3d::Rect local_rect() const{
			return s3d::Rect{ {0,0}, rect_.size };
		}
		virtual void draw_impl() {};

	private:

		cfn::basic_cnannel<std::monostate> click_;

	};

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

	class Info 
		:public BoxObject 
	{
	public:
		Info()
			:BoxObject({ {0,0},{ 280,100 } })
		{		}
		s3d::Texture bg_ = s3d::TextureAsset(U"anos");
		s3d::String description_ = U"え、VRってパンツ覗くための道具じゃないんですか？";
		s3d::Font font{ 10 };
		bool visible = 0;
	private:
		void draw_impl() override{
			if (visible) {
				local_rect().draw(s3d::Palette::Black);
				s3d::TextureAsset(U"anos").resized(100, 100).draw();
				font(description_).draw();
			}

		}
	};

	class GameView {
	public:
		GameView() {		}

		void init() {
			s3d::TextureAsset::Register(U"anos", U"asset/texture/anos.png");
		}

		void update() {
			for (auto &build : builing_list_) {
				build.update();
			}

			info_->visible = false;
			for (auto &build : builing_list_) {
				if (build.rect_.mouseOver()) {
					info_->visible = true;
					auto boxh = info_->rect_.h;
					int y = std::clamp(s3d::Cursor::Pos().y - boxh / 2, 0 , s3d::Window::Height() - boxh);
					int x = build.rect_.x - info_->rect_.w;
					info_->rect_.pos = {x,y};
				}
			}

		}
		void draw();

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

		std::shared_ptr<Info> info_ = std::make_shared<Info>();
	};
}