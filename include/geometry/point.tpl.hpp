/***************************************************************************
 *            geometry/point.tpl.hpp
 *
 *  Copyright  2008-20  Alberto Casagrande, Pieter Collins
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

/** @file geometry/point.tpl.hpp
 *  @brief Template definitions of point operations.
 */

#include <cstdarg>
#include <sstream>
#include <string>
#include <vector>

#include "geometry/box.hpp"
#include "geometry/point.hpp"
#include "utility/stlio.hpp"

namespace Ariadne {

template<class X>
Point<X>::Point(InitializerList<X> lst)
    : Vector<X>(lst) {
}

template<class X>
Point<X>* Point<X>::clone() const {
    return new Point<X>(*this);
}

template<class X>
BoundingBoxType Point<X>::bounding_box() const {
    typedef decltype(declval<X>() + declval<FloatDPError>()) U;
    FloatDPError eps(FloatDP::eps(dp));
    ExactBoxType r(this->dimension());
    UpperIntervalType v;
    for (SizeType i = 0; i != this->dimension(); ++i) {
        const X& pti = (*this)[i];
        r[i] = cast_exact_interval(Interval<U>(pti - eps, pti + eps));
    }
    return r;
}

} // namespace Ariadne
