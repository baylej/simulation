/*
    3D Physics Simulations - Menu context
    Copyright (C) 2020  Jonathan Bayle

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once

#ifndef SIMULATION_MENU_HPP
#define SIMULATION_MENU_HPP

#include <memory>
#include "../context.hpp"
#include "../renderer/renderer.hpp"

namespace Engine::Contexts {

class Menu : public Context {
public:
	Menu();
	~Menu() final = default;
	void loop_run(float delta_t) final;

private:
	bool show_help = false;
	std::unique_ptr<Renderer::Renderer> renderer;
	std::unique_ptr<Context> new_ctx;
};

} // namespace Engine::Contexts

#endif //SIMULATION_MENU_HPP
