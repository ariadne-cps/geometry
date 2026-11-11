/***************************************************************************
 *            geometry/point.hpp
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

/** @file geometry/point.hpp
 *  @brief Points in Euclidean space.
 */

#ifndef ARIADNE_POINT_HPP
#define ARIADNE_POINT_HPP

#include "algebra/vector.hpp"
#include "geometry/box.decl.hpp"
#include "numeric/numeric.hpp"

namespace Ariadne {

template<class X>
class Point;

//!@{
//! \relates Point
//! \name Type synonyms
using DyadicPoint = Point<Dyadic>;     //!< <p/>
using RationalPoint = Point<Rational>; //!< <p/>
using RealPoint = Point<Real>;         //!< <p/>

template<class F>
using ExactPoint = Point<F>;
template<class F>
using ValidatedPoint = Point<Bounds<F>>;
template<class F>
using ApproximatePoint = Point<Approximation<F>>;

using FloatDPPoint = Point<FloatDP>;
using FloatDPBoundsPoint = Point<FloatDPBounds>;
using FloatDPApproximationPoint = Point<FloatDPApproximation>;

typedef ExactPoint<FloatDP> ExactPointType;

//!@}

//! A point in Euclidean space.
template<class X>
class Point : public Vector<X> {
    /** @brief Construct default coordinate from the supplied arguments. */
    static X _make_default_coordinate();

  public:
    typedef X RealType;
    //! A point in zero dimensions
    explicit Point() : Vector<RealType>() {}
    //! The origin in \a n dimensions.
    template<class... PRS>
        requires Constructible<X, Nat, PRS...>
    explicit Point(SizeType n, PRS... prs) : Vector<RealType>(n, X(0u, prs...)) {}
    /** @brief Construct a \c Point from v. */
    Point(const Vector<RealType>& v) : Vector<RealType>(v) {}
    /** @brief Copy-construct the object from the supplied instance. */
    template<class T>
        requires Convertible<T, X>
    Point(const Point<T>& pt) : Vector<RealType>(pt.vector()) {}
    /** @brief Construct a \c Point from pt, prs. */
    template<class Y, class... PRS>
        requires Constructible<X, Y, PRS...>
    Point(const Point<Y>& pt, PRS... prs) : Vector<RealType>(pt.vector(), prs...) {}
    //! Construct from an initializer list of floating-point values.
    template<class T>
        requires Convertible<T, X>
    Point(SizeType n, const T& t) : Vector<RealType>(n, RealType(t)) {}
    //! Construct from an initializer list of floating-point values.
    explicit Point(InitializerList<X> lst);
    //! Construct from a size and an element generator
    template<class G>
        requires InvocableReturning<X, G, SizeType>
    Point(SizeType n, const G& g) : Vector<X>(n, g, _make_default_coordinate()) {}
    //! Construct from a size, an element generator, and characteristics of the default element
    template<class G, class... PRS>
        requires InvocableReturning<X, G, SizeType> and Constructible<X, PRS...>
    Point(SizeType n, const G& g, PRS... prs) : Vector<X>(n, g, prs...) {}
    //! Construct from an initializer list of floating-point values.
    template<class... PRS>
        requires Constructible<X, ExactDouble, PRS...>
    explicit Point(InitializerList<ExactDouble> lst, PRS... prs);
    //! The origin in \a n dimensions.
    template<class... PRS>
        requires Constructible<X, Int, PRS...>
    static Point origin(SizeType n, PRS... prs) { return Point(n, RealType(0, prs...)); }
    //! A dynamically-allocated copy.
    Point<X>* clone() const;
    //! The dimension of the point.
    DimensionType dimension() const { return this->size(); }
    //! An explicit cast to a float vector. Useful to prevent ambiguous function overloads.
    const Vector<RealType>& vector() const { return *this; }

    /** @brief Return the centre of the enclosure. */
    Vector<RealType> centre() const { return *this; }

    /** @brief Return the Cartesian product. */
    friend Point<X> product(const Point<X>& pt1, const Point<X>& pt2) {
        return Point<X>(join(pt1.vector(), pt2.vector()));
    }

    //! Write to an output stream.
    OutputStream& _write(OutputStream& os) const {
        return os << static_cast<const Vector<RealType>&>(*this);
    }

    /** @brief Return a box enclosing the set. */
    FloatDPUpperBox bounding_box() const;
};

/** @brief Return the distance between the operands. */
template<class X>
inline X distance(const Point<X>& pt1, const Point<X>& pt2) {
    ARIADNE_PRECONDITION(pt1.dimension() == pt2.dimension());
    ARIADNE_PRECONDITION(pt1.dimension() > 0);
    X result = sqr(pt1.at(0) - pt2.at(0));
    auto v1 = pt1.vector();
    auto v2 = pt2.vector();
    for (SizeType i = 1; i < pt1.dimension(); ++i) {
        result += sqr(pt1.at(i) - pt2.at(i));
    }
    return sqrt(result);
}

/** @brief Construct a \c Point with the specified characteristics. */
template<class X>
Point(Vector<X>) -> Point<X>;

template<class X>
X Point<X>::_make_default_coordinate() {
    static_assert(DefaultConstructible<X> or Constructible<X, DP>);
    if constexpr (DefaultConstructible<X>) {
        return X();
    } else if constexpr (Constructible<X, DP>) {
        return X(dp);
    }
}

template<class X>
template<class... PRS>
    requires Constructible<X, ExactDouble, PRS...>
Point<X>::Point(InitializerList<ExactDouble> lst, PRS... prs) : Vector<X>(UniformArray<X>(lst, prs...)) {}

} // namespace Ariadne

#endif // ARIADNE_POINT_HPP
