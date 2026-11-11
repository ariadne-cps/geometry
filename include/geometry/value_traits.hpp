/***************************************************************************
 *            geometry/value_traits.hpp
 *
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

// Common scalar/vector/matrix value traits shared by geometric sets and functions.
// Geometry owns these declarations so it does not depend on Function.
/** @file geometry/value_traits.hpp
 *  @brief Scalar, vector, and matrix value traits for geometric objects.
 */

#ifndef ARIADNE_GEOMETRY_VALUE_TRAITS_HPP
#define ARIADNE_GEOMETRY_VALUE_TRAITS_HPP

#include "algebra/linear_algebra.decl.hpp"
#include "utility/metaprogramming.hpp"
#include "utility/typedefs.hpp"

namespace Ariadne {

/** @brief Compile-time traits for linear algebra. */
template<template<class> class T>
struct LinearAlgebraTraits;
/** @brief Compile-time traits for linear algebra. */
template<>
struct LinearAlgebraTraits<Scalar> {
    template<class X>
    using Type = Scalar<X>;
    typedef Scalar<Real> Kind;
    typedef SizeOne SizeType;
    typedef IndexZero IndexType;
};
/** @brief Compile-time traits for linear algebra. */
template<>
struct LinearAlgebraTraits<Vector> {
    template<class X>
    using Type = Vector<X>;
    typedef Vector<Real> Kind;
    typedef Ariadne::SizeType SizeType;
    typedef Ariadne::SizeType IndexType;
};
/** @brief Compile-time traits for linear algebra. */
template<>
struct LinearAlgebraTraits<Matrix> {
    template<class X>
    using Type = Matrix<X>;
    typedef Matrix<Real> Kind;
    typedef Pair<Ariadne::SizeType, Ariadne::SizeType> SizeType;
    typedef Pair<Ariadne::SizeType, Ariadne::SizeType> IndexType;
};
using ScalarTraits = LinearAlgebraTraits<Scalar>;
using VectorTraits = LinearAlgebraTraits<Vector>;
using MatrixTraits = LinearAlgebraTraits<Matrix>;

/** @brief Compile-time traits for value. */
template<class R>
struct ValueTraits;
/** @brief Compile-time traits for value. */
template<>
struct ValueTraits<Scalar<Real>> : LinearAlgebraTraits<Scalar> {};
/** @brief Compile-time traits for value. */
template<template<class> class T>
struct ValueTraits<T<Real>> : LinearAlgebraTraits<T> {};

using DimensionOne = SizeOne;

} // namespace Ariadne
#endif // ARIADNE_GEOMETRY_VALUE_TRAITS_HPP
