/***************************************************************************
 *            geometry/interval_set.hpp
 *
 *  Scalar interval adapter for geometric set interfaces.
 *
 *  This file is part of Ariadne Geometry and is distributed under the
 *  GNU General Public License version 3 or (at your option) any later version.
 *  Copyright  2026  Luca Geretti
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne Geometry.
 *
 *  Ariadne Geometry is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne Geometry is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Geometry.  If not, see <https://www.gnu.org/licenses/>.
 */

/** @file geometry/interval_set.hpp
 *  @brief Adapter exposing scalar intervals through geometric set interfaces.
 */

#ifndef ARIADNE_GEOMETRY_INTERVAL_SET_HPP
#define ARIADNE_GEOMETRY_INTERVAL_SET_HPP

#include <utility>

#include "interval/interval.hpp"

namespace Ariadne {

template<class U>
class IntervalSetBase : public Interval<U> {
  protected:
    IntervalSetBase(Interval<U>&& ivl) : Interval<U>(ivl) {}
    template<class UU>
    decltype(auto) _inside(const Interval<UU>& ivl) const { return inside(*this, ivl); }
    template<class UU>
    decltype(auto) _separated(const Interval<UU>& ivl) const { return separated(*this, ivl); }
};

template<class U>
class IntervalSet : public IntervalSetBase<U> {
  public:
    IntervalSet(Interval<U>&& ivl) : IntervalSetBase<U>(std::move(ivl)) {}
    const Interval<U>& bounding_box() const { return *this; }
    template<class UU>
    decltype(auto) inside(const Interval<UU>& ivl) const { return this->_inside(ivl); }
    template<class UU>
    decltype(auto) separated(const Interval<UU>& ivl) const { return this->_separated(ivl); }
};

} // namespace Ariadne

#endif // ARIADNE_GEOMETRY_INTERVAL_SET_HPP
