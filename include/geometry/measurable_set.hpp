/***************************************************************************
 *            geometry/measurable_set.hpp
 *
 *  Copyright  2020  Pieter Collins
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

/** @file geometry/measurable_set.hpp
 *  @brief Lower-measurable geometric sets and measures.
 */

#ifndef ARIADNE_MEASURABLE_SET_HPP
#define ARIADNE_MEASURABLE_SET_HPP

#include "geometry/set_interface.hpp"
#include "interval/interval.hpp"
#include "numeric/dyadic.hpp"
#include "numeric/integer.hpp"
#include "numeric/number.hpp"
#include "numeric/sequence.hpp"
#include "numeric/twoexp.hpp"
#include "utility/handle.hpp"

namespace Ariadne {

/** @brief Return two raised to the given power. */
inline TwoExp exp2(Integer z) {
    Int n = z.get_si();
    ARIADNE_PRECONDITION(n == z);
    return exp2(n);
}

/** @brief Sequence providing increasing lower information. */
template<class S>
class LowerSequence : public ConvergentSequence<S> {
  public:
    /** @brief Construct a \c LowerSequence from seq. */
    explicit LowerSequence(const Sequence<S>& seq) : ConvergentSequence<S>(seq) {}
    /** @brief Construct a \c LowerSequence from fn. */
    explicit LowerSequence(std::function<S(Natural)> fn) : ConvergentSequence<S>(fn) {}
    /** @brief Return the sequence of measure approximations. */
    decltype(auto) measures() const {
        using X = typename S::MeasureType;
        return IncreasingSequence<X>(Sequence<X>([this](Natural n) { return cast_positive(max(0u, (*this)[n].measure() - exp2(-n))); }));
    }
};

using Void = void;

template<class P, class T>
class OpenSet;

template<class P, class T>
class LowerMeasurableSet;
template<class T>
using EffectiveLowerMeasurableSet = LowerMeasurableSet<EffectiveTag, T>;
template<class T>
using ValidatedLowerMeasurableSet = LowerMeasurableSet<ValidatedTag, T>;

/** @brief Compile-time predicate testing whether a type is lower measurable set. */
template<class S, class P, class T>
struct IsLowerMeasurableSet {
    using LY = PositiveLowerNumber<P>;
    using BS = typename SetTraits<T>::BasicSetType;
    /** @brief Evaluate the compile-time or runtime predicate for the supplied arguments. */
    template<class SS, class = decltype(measure(intersection(declval<SS>(), declval<BS>())))>
    static std::true_type test(int);
    /** @brief Evaluate the compile-time or runtime predicate for the supplied arguments. */
    template<class SS>
    static std::false_type test(...);
    static const bool value = decltype(test<S>(1))::value;
};

template<class S, class P, class T>
concept ALowerMeasurableSet = IsLowerMeasurableSet<S, P, T>::value;

/** @brief Polymorphic interface for lower measurable set. */
template<class P, class T>
class LowerMeasurableSetInterface {
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    /** @brief Destroy the object and release its owned resources. */
    virtual ~LowerMeasurableSetInterface() = default;
    /** @brief Return the measure of the set within the supplied bounding set. */
    virtual PositiveLowerNumber<P> _measure_in(const BasicSetType&) const = 0;
    /** @brief Return the measure of the set. */
    virtual PositiveLowerNumber<P> _measure() const = 0;
    //    virtual LowerMeasurableSetInterface<P,T>* _intersection(OpenSet<P,T> const&) const = 0;
    /** @brief Write the value to the output stream. */
    virtual OutputStream& _write(OutputStream& os) const = 0;
};

/** @brief Polymorphic set whose measure is available from below. */
template<class P, class T>
class LowerMeasurableSet : public Handle<const LowerMeasurableSetInterface<P, T>> {
  public:
    using typename Handle<const LowerMeasurableSetInterface<P, T>>::Interface;
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    using Handle<Interface>::Handle;
    /** @brief Construct a \c LowerMeasurableSet from s. */
    template<ALowerMeasurableSet<P, T> S>
    LowerMeasurableSet(const S& s);
    /** @brief Return the measure of the set within the supplied bounding set. */
    PositiveLowerNumber<P> measure_in(const BasicSetType& bs) const { return this->reference()._measure_in(bs); }
    /** @brief Return the measure of the set. */
    PositiveLowerNumber<P> measure() const { return this->reference()._measure(); }
    /** @brief Return the intersection of the operands. */
    friend LowerMeasurableSet<P, T> intersection(LowerMeasurableSet<P, T>, OpenSet<P, T>);
    /** @brief Write the value to the output stream. */
    friend OutputStream& operator<<(OutputStream& os, const LowerMeasurableSet<P, T>& lms) { return lms.reference()._write(os); }
};

/** @brief Adapter exposing lower measurable set. */
template<class S, class P, class T>
class LowerMeasurableSetWrapper;

/** @brief Adapter exposing lower measurable set. */
template<class S, class T>
class LowerMeasurableSetWrapper<S, ValidatedTag, T>
    : public virtual LowerMeasurableSet<ValidatedTag, T>::Interface {
    using P = ValidatedTag;
    /** @brief Return the underlying base object. */
    const S& base() const { return this->_s; }

  private:
    S _s;

  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    /** @brief Construct a \c LowerMeasurableSetWrapper from set. */
    LowerMeasurableSetWrapper(const S& set) : _s(set) {}
    /** @brief Return the measure of the set within the supplied bounding set. */
    virtual PositiveLowerNumber<P> _measure_in(const BasicSetType& bs) const final override {
        return PositiveLowerNumber<P>(ValidatedLowerNumber(measure(intersection(this->base(), bs))));
    }
    /** @brief Return the measure of the set. */
    PositiveLowerNumber<P> _measure() const final override {
        return PositiveLowerNumber<P>(measure(this->base()));
    }
    /** @brief Write the value to the output stream. */
    OutputStream& _write(OutputStream& os) const final override {
        return os << this->base();
    }
};

/** @brief Wrap the set in the lower-measurable-set interface. */
template<class S>
decltype(auto) wrap_lower_measurable(const S& s) {
    using P = ValidatedTag;
    using T = Real;
    return LowerMeasurableSet<P, T>(std::make_shared<LowerMeasurableSetWrapper<S, P, T>>(s));
}

template<class P, class T>
template<ALowerMeasurableSet<P, T> S>
LowerMeasurableSet<P, T>::LowerMeasurableSet(const S& s)
    : LowerMeasurableSet(std::make_shared<LowerMeasurableSetWrapper<S, P, T>>(s)) {
}

/** @brief Concrete model of a set with lower-computable measure. */
template<class S, class E>
class LowerMeasurableSetModel {
    S _set;
    E _error;

  public:
    typedef typename S::BasicSetType BasicSetType;
    typedef decltype(cast_positive(measure(declval<S>()) - declval<E>())) MeasureType;

    /** @brief Construct a \c LowerMeasurableSetModel from s, e. */
    explicit LowerMeasurableSetModel(S s, E e) : _set(s), _error(min(max(e, nul(e)), measure(s))) {}
    /** @brief Construct a \c LowerMeasurableSetModel from s, pres. */
    template<class... PRES>
        requires Constructible<E, Nat, PRES...>
    explicit LowerMeasurableSetModel(S s, PRES... pres) : _set(s), _error(0u, pres...) {}
    /** @brief Return the measure of the set. */
    friend MeasureType measure(const LowerMeasurableSetModel<S, E>& lms) {
        return lms._measure();
    }
    /** @brief Return the intersection of the operands. */
    friend LowerMeasurableSetModel<S, E> intersection(const LowerMeasurableSetModel<S, E>& lms, const BasicSetType& bs) {
        return LowerMeasurableSetModel(intersection(lms._set, bs), lms._error);
    }
    /** @brief Return the intersection of the operands. */
    friend LowerMeasurableSetModel<S, E> intersection(const LowerMeasurableSetModel<S, E>& lms1, const LowerMeasurableSetModel<S, E>& lms2) {
        return LowerMeasurableSetModel(intersection(lms1._set, lms2._set), min(lms1._error, lms2._error));
    }
    /** @brief Join the operands in their natural order. */
    friend LowerMeasurableSetModel<S, E> join(const LowerMeasurableSetModel<S, E>& lms1, const LowerMeasurableSetModel<S, E>& lms2) {
        return LowerMeasurableSetModel(join(lms1._set, lms2._set), lms1._error + lms2._error);
    }
    /** @brief Write the value to the output stream. */
    friend OutputStream& operator<<(OutputStream& os, const LowerMeasurableSetModel<S, E>& lms) {
        return os << lms._set << "+/-" << lms._error;
    }

  private:
    /** @brief Return the measure of the set. */
    MeasureType _measure() const {
        // return MeasureType(cast_positive(max(PositiveFloatDPBounds(FloatDPBounds(2,2,dp))-this->_error,0u))); }
        return cast_positive(max(measure(this->_set) - this->_error, 0u));
    }
};

} // namespace Ariadne

#include "union_of_intervals.hpp"

namespace Ariadne {
template class LowerMeasurableSetModel<UnionOfIntervals<Dyadic>, Dyadic>;
template class LowerMeasurableSetModel<UnionOfIntervals<FloatDP>, Error<FloatDP>>;
} // namespace Ariadne

#endif
