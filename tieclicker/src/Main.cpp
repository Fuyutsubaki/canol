
# include <Siv3D.hpp> // OpenSiv3D v0.2.7
#include "app/clicker/app1.hpp"


void Main()
{
	Graphics::SetBackground(ColorF(0.8, 0.9, 1.0));

	const Font font(50);

	cfn::mainloop_scheduler scheduler;
	auto dat = std::make_shared<app::Data>();

	auto model = std::make_shared<app::GameModel>(dat);

	app::GameView view;

	model->onChangedBank().subscribe([&](auto n) {
		view.set_bank(n);
	});

	model->onChangedBuilding().subscribe([&](auto const&t) {
		auto[n,build] = t;
		view.set_building(n, build);
	});

	view.onClick().subscribe([&](auto) {
		model->click();
	});

	for (std::size_t i = 0; i < app::BuildingMax; ++i) {
		view.onClickBuild(i).subscribe([=](auto) {
			model->buy_building(i);
		});
	}

	while (System::Update())
	{
		while (scheduler.has_event())
		{
			scheduler.update();
		}
		model->tik();  
		view.update();
		view.draw();
	}
}
