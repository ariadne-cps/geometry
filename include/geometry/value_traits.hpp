// Common scalar/vector/matrix value traits shared by geometric sets and functions.
// Geometry owns these declarations so it does not depend on Function.
#ifndef ARIADNE_GEOMETRY_VALUE_TRAITS_HPP
#define ARIADNE_GEOMETRY_VALUE_TRAITS_HPP

#include "algebra/linear_algebra.decl.hpp"
#include "utility/metaprogramming.hpp"
#include "utility/typedefs.hpp"

namespace Ariadne {

template<template<class>class T> struct LinearAlgebraTraits;
template<> struct LinearAlgebraTraits<Scalar> {
    template<class X> using Type=Scalar<X>;
    typedef Scalar<Real> Kind;
    typedef SizeOne SizeType;
    typedef IndexZero IndexType;
};
template<> struct LinearAlgebraTraits<Vector> {
    template<class X> using Type=Vector<X>;
    typedef Vector<Real> Kind;
    typedef Ariadne::SizeType SizeType;
    typedef Ariadne::SizeType IndexType;
};
template<> struct LinearAlgebraTraits<Matrix> {
    template<class X> using Type=Matrix<X>;
    typedef Matrix<Real> Kind;
    typedef Pair<Ariadne::SizeType,Ariadne::SizeType> SizeType;
    typedef Pair<Ariadne::SizeType,Ariadne::SizeType> IndexType;
};
using ScalarTraits = LinearAlgebraTraits<Scalar>;
using VectorTraits = LinearAlgebraTraits<Vector>;
using MatrixTraits = LinearAlgebraTraits<Matrix>;

template<class R> struct ValueTraits;
template<> struct ValueTraits<Scalar<Real>> : LinearAlgebraTraits<Scalar> { };
template<template<class>class T> struct ValueTraits<T<Real>> : LinearAlgebraTraits<T> { };

using DimensionOne = SizeOne;

} // namespace Ariadne
#endif // ARIADNE_GEOMETRY_VALUE_TRAITS_HPP
