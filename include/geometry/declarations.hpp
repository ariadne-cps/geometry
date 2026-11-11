/***************************************************************************
 *            geometry/declarations.hpp
 *
 *  Copyright  2011-26  Pieter Collins
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


/** @file geometry/declarations.hpp
 *  @brief Common geometric forward declarations and type aliases.
 */

#ifndef ARIADNE_GEOMETRY_DECLARATIONS_HPP
#define ARIADNE_GEOMETRY_DECLARATIONS_HPP

#include <iosfwd>

#include "utility/metaprogramming.hpp"
#include "utility/typedefs.hpp"

#include "foundation/paradigm.hpp"
#include "foundation/logical.decl.hpp"

#include "interval/interval.decl.hpp"
#include "geometry/box.decl.hpp"

namespace Ariadne {


template<class X> class Point;

} // namespace Ariadne

#endif /* ARIADNE_GEOMETRY_DECLARATIONS_HPP */
