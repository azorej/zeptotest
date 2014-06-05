#include "menu_window.hpp"

#include "application.hpp"
#include "button.hpp"
#include "game_window.hpp"

menu_window_t::menu_window_t()
{
	auto button = add_child(new button_t("new_game.png", {0.f, 0.f}, {25.f, 15.f}));
	button->set_on_click([](){
		application_t::singleton().set_main_window(new game_window_t());
	});
}
