/*
    3D Physics Simulations - Context: draw and update function refs holder
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

#ifndef SIMULATION_CONTEXT_HPP
#define SIMULATION_CONTEXT_HPP

class Context {
public:
	virtual void loop_run(float delta_t) = 0;
	// Called when switching to this context
	virtual void start() {};
};

// Holds the running context, set/get the running context
class Context_holder {
public:
	Context_holder(const Context_holder&) = delete;
	Context_holder(Context_holder&&) = delete;
	Context_holder& operator =(const Context_holder&) = delete;
	Context_holder& operator =(Context_holder&&) = delete;

	// Context manipulation
	void set_context(Context* ctx) noexcept { ctx->start(); current = ctx; }
	Context* get_context() const noexcept { return current; }

	// Global Contexts (set in main function)
	Context* menu = nullptr;

	// Display width and height, updated by a callback
	int display_width = 800;
	int display_height = 600;

	// Unique static instance
	static Context_holder& get();

private:
	Context_holder() = default;

	Context* current = nullptr;

	static Context_holder instance;
};

#endif //SIMULATION_CONTEXT_HPP
