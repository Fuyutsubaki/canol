
# include <Siv3D.hpp> // OpenSiv3D v0.2.7

#include "coffin/util.hpp"
#include "view.hpp"

namespace {
	struct Dat {
		s3d::String screen_name;
		s3d::String desc;
		s3d::String pic_id;
	};

	std::array< Dat, app::BuildingMax> conf = {
		Dat{ U"やる", UR"(
とりあえずやってみる
知恵が知恵を呼び知恵を作る
しかし、人ひとりが現実的な時間にできることは
あまりにも少ない……
		)" ,U"punch" },
		Dat{ U"自動水やり機", UR"(
何をやるにもまず水が必要
適切な水分管理により頭の回転がよくなる
		)" ,U"water" },
		Dat{ U"照明", UR"(
何をやるにもまず日光が必要
水と同じ理由で頭がよくなる
		)" ,U"light" },
		Dat{ U"サーモン", UR"(
DHAがどうたらで頭がよくなる
あと鮭はおいしい。好物。誰かの
彼女も鮭好きっ子なのだろうか
)" ,U"sushi" },

		Dat{ U"VRするやつ", UR"(
え？VRってパンツ覗くためのやつじゃないんですか？？？
物事を様々な角度から見ることを可能にし、
隠された知恵を発見する
【勝手広告】「星の欠片の物語、ひとかけら版」好評配信中

		)" ,U"hmd" },
		Dat{ U"ANOS", UR"(
時かける系鉱物
時間の巻き戻しを行うことで、知恵を得る際発生する様々な
可能性を削減し（少なくとも第三者の目から見る分には）
[知恵/時間]を効率化する
)"  ,U"anos" }
	};
}

namespace app {


	s3d::String tienum_goodviwer(tie_float n) {
		// ex: 123 → 1.230E+3;
		if (n>10000) {
			auto d = (int)std::log10(n);
			return U"{0:.3f}E+{1}"_fmt(n / std::pow(10, d), d);
		}
		else {
			return U"{:.1f}"_fmt(n);
		}
	}

	void GameView::update() {
		for (auto &build : builing_list_) {
			build.update();
		}

		info_->visible = false;
		for (int i = 0; i < app::BuildingMax; ++i) {
			auto & build = builing_list_[i];
			if (build.rect_.mouseOver()) {
				info_->visible = true;
				info_->screen_name = conf[i].screen_name;
				info_->description_ = conf[i].desc;
				info_->textrure = conf[i].pic_id;
				auto boxh = info_->rect_.h;
				int y = std::clamp(s3d::Cursor::Pos().y - boxh / 2, 0, s3d::Window::Height() - boxh);
				int x = build.rect_.x - info_->rect_.w;
				info_->rect_.pos = { x,y };
			}
		}
	}

}
