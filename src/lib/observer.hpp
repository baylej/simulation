/*
    3D Physics Simulations - Observer: observer pattern
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

#ifndef LIB_OBSERVER_HPP
#define LIB_OBSERVER_HPP

#include <vector>

namespace Lib {

// An observer template for function func to be called when a single instance W is modified, to notify many instance of O
template<typename O, typename W, void (* func)(O& observer, const W& what)>
class Observer {
public:
	explicit Observer(const W& what) : what(what) {};
	~Observer() = default;
	Observer(const Observer&) = delete;
	Observer(Observer&&) = delete;
	Observer& operator=(const Observer&) = delete;
	Observer& operator=(Observer&&) = delete;

	void add(O& observer)
	{
		observers.push_back(observer);
	}

	void notify() const
	{
		for (auto observer: observers) {
			func(observer, what);
		}
	}

private:
	std::vector<O&> observers;
	const W& what;
};

// An observer template for heterogeneous types of observers
template<typename W, void (* func)(void* observer, const W& what)>
class Observer_heterogeneous {
public:
	explicit Observer_heterogeneous(const W& what) : what(what) {};
	~Observer_heterogeneous() = default;
	Observer_heterogeneous(const Observer_heterogeneous&) = delete;
	Observer_heterogeneous(Observer_heterogeneous&&) = delete;
	Observer_heterogeneous& operator=(const Observer_heterogeneous&) = delete;
	Observer_heterogeneous& operator=(Observer_heterogeneous&&) = delete;

	void add(void* observer, typeof(func) function)
	{
		observers.push_back({observer, function});
	}

	void notify() const
	{
		for (std::pair<void*, typeof func> observer:
		observers) {
			observer.second(observer, what);
		}
	}

private:
	std::vector<std::pair<void*, typeof(func)>> observers;
	const W& what;
};

} // namespace Lib

#endif //LIB_OBSERVER_HPP
