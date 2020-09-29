/*
    3D Physics Simulations - simerror: class exception thrown in this project
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

#ifndef SIMULATION_SIMERROR_HPP
#define SIMULATION_SIMERROR_HPP

#include <stdexcept>

class Sim_error: public std::runtime_error {
public:
	explicit Sim_error(const std::string& what): std::runtime_error(what) {};

	Sim_error(const runtime_error&) = delete;
	Sim_error(const runtime_error&&) = delete;
};

#endif //SIMULATION_SIMERROR_HPP
