
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

	model->as_click().subscribe([&](int n) {
		view.set_bank(n);
	});

	view.get_channel().subscribe([&](int) {
		model->click();
	});

	while (System::Update())
	{
		while (scheduler.has_event())
		{
			scheduler.update();
		}
		
		view.update();
		view.draw();
	}
}
