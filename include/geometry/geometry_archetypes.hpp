/***************************************************************************
 *            geometry/geometry_archetypes.hpp
 *
 *  Copyright  2023  Pieter Collins
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

/** @file geometry/geometry_archetypes.hpp
 *  @brief Archetypes for geometry set interfaces and concepts.
 */

#ifndef ARIADNE_GEOMETRY_ARCHETYPES_HPP
#define ARIADNE_GEOMETRY_ARCHETYPES_HPP

#include <iosfwd>

#include "foundation/logical.decl.hpp"

namespace Ariadne {

/** @brief Reference type specifying the requirements of regular located set. */
template<class P, class T>
class RegularLocatedSetArchetype {
  public:
    typedef typename SetTraits<T>::DimensionType DimensionType;
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    typedef typename SetTraits<T>::BoundingSetType BoundingSetType;

    /** @brief Return the dimension. */
    auto dimension() const -> DimensionType;
    /** @brief Return a box enclosing the set. */
    auto bounding_box() const -> BoundingSetType;
    /** @brief Test whether the first set covers the second. */
    auto covers(BasicSetType) const -> LowerLogicalType<P>;
    /** @brief Test whether the sets are separated. */
    auto separated(BasicSetType) const -> LowerLogicalType<P>;
    /** @brief Test whether the sets overlap. */
    auto overlaps(BasicSetType) const -> LowerLogicalType<P>;
    /** @brief Test whether the first set lies inside the second. */
    auto inside(BasicSetType) const -> LowerLogicalType<P>;

    /** @brief Write the value to the output stream. */
    friend OutputStream& operator<<(OutputStream& os, const RegularLocatedSetArchetype<P, T>& set);
};

} // namespace Ariadne

#endif // ARIADNE_GEOMETRY_ARCHETYPES_HPP
