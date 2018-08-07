

#include "coffin/util.hpp"
#include "view.hpp"

namespace app {
	void GameView::draw() {
		font(tienum_goodviwer(val_)).draw();
		font(U"per second:+", tienum_goodviwer(tps_)).draw(0, 100);
		for (auto &build : builing_list_) {
			build.draw();
		}
		info_->draw();
	}

}
