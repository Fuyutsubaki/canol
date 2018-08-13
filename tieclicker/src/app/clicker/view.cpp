
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
		Dat{ U"���", UR"(
�Ƃ肠��������Ă݂�
�m�b���m�b���Ăђm�b�����
�������A�l�ЂƂ肪�����I�Ȏ��Ԃɂł��邱�Ƃ�
���܂�ɂ����Ȃ��c�c
		)" ,U"punch" },
		Dat{ U"���������@", UR"(
�������ɂ��܂������K�v
�K�؂Ȑ����Ǘ��ɂ�蓪�̉�]���悭�Ȃ�
		)" ,U"water" },
		Dat{ U"�Ɩ�", UR"(
�������ɂ��܂��������K�v
���Ɠ������R�œ����悭�Ȃ�
		)" ,U"light" },
		Dat{ U"�T�[����", UR"(
DHA���ǂ�����œ����悭�Ȃ�
���ƍ��͂��������B�D���B�N����
�ޏ������D�����q�Ȃ̂��낤��
)" ,U"sushi" },

		Dat{ U"VR������", UR"(
���HVR���ăp���c�`�����߂̂����Ȃ���ł����H�H�H
������l�X�Ȋp�x���猩�邱�Ƃ��\�ɂ��A
�B���ꂽ�m�b�𔭌�����
�y����L���z�u���̌��Ђ̕���A�ЂƂ�����Łv�D�]�z�M��

		)" ,U"hmd" },
		Dat{ U"ANOS", UR"(
��������n�z��
���Ԃ̊����߂����s�����ƂŁA�m�b�𓾂�۔�������l�X��
�\�����팸���i���Ȃ��Ƃ���O�҂̖ڂ��猩�镪�ɂ́j
[�m�b/����]������������
)"  ,U"anos" }
	};
}

namespace app {


	s3d::String tienum_goodviwer(tie_float n) {
		// ex: 123 �� 1.230E+3;
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
