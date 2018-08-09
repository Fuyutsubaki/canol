#pragma once
#include "app1.hpp"

namespace app {
	s3d::String tienum_goodviwer(tie_float n);
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

		void draw_impl() {
			local_rect().drawFrame();
			s3d::FontAsset(U"f50")(num).draw();
			s3d::FontAsset(U"f10")(U"•K—v‚È’mŒb:", tienum_goodviwer(next_price)).draw(0, 50);
		}
	};

	class Info 
		:public BoxObject 
	{
	public:
		Info()
			:BoxObject({ {0,0},{ 280,100 } })
		{		}
		s3d::String screen_name;
		s3d::String description_ ;
		s3d::String textrure;
		bool visible = 0;
	private:
		void draw_impl() override{
			if (visible) {
				//s3d::TextureAsset(textrure).resized(100, 100).draw();
				auto img = s3d::TextureAsset(textrure);
				img.scaled(100.0 / img.height()).draw();
				local_rect().draw(s3d::ColorF{ 0,0,0,0.6 });
				auto rect1 = s3d::FontAsset(U"f15")(screen_name).draw();
				s3d::FontAsset(U"f10")(description_).draw(rect1.bl());
			}

		}
	};

	class GameView {
	public:
		GameView() {		}

		void init() {
			s3d::TextureAsset::Register(U"bg", U"asset/texture/bg.png");
			s3d::TextureAsset::Register(U"punch", U"asset/texture/punch.png");
			s3d::TextureAsset::Register(U"water", U"asset/texture/water.png");
			s3d::TextureAsset::Register(U"light", U"asset/texture/light.png");
			s3d::TextureAsset::Register(U"anos", U"asset/texture/anos.png");
			s3d::TextureAsset::Register(U"sushi", U"asset/texture/sushi.png");
			s3d::TextureAsset::Register(U"hmd", U"asset/texture/hmd.png");
			s3d::FontAsset::Register(U"f80", 80);
			s3d::FontAsset::Register(U"f50", 50);
			s3d::FontAsset::Register(U"f15", 15);
			s3d::FontAsset::Register(U"f10", 10);
		}

		void update();
		void draw() {
			s3d::TextureAsset(U"bg").draw();
			s3d::FontAsset(U"f50")(tienum_goodviwer(val_)).draw();
			s3d::FontAsset(U"f15")(U"per second:+", tienum_goodviwer(tps_)).draw(0, 100);
			for (auto &build : builing_list_) {
				build.draw();
			}
			info_->draw();
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