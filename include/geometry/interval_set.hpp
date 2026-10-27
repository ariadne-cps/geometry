/***************************************************************************
 *            geometry/interval_set.hpp
 *
 *  Scalar interval adapter for geometric set interfaces.
 *
 *  This file is part of Ariadne and is distributed under the
 *  GNU General Public License version 3 or (at your option) any later version.
 ****************************************************************************/

#ifndef ARIADNE_GEOMETRY_INTERVAL_SET_HPP
#define ARIADNE_GEOMETRY_INTERVAL_SET_HPP

#include <utility>

#include "interval/interval.hpp"

namespace Ariadne {

template<class U> class IntervalSetBase : public Interval<U> {
  protected:
    IntervalSetBase(Interval<U>&& ivl) : Interval<U>(ivl) { }
    template<class UU> decltype(auto) _inside(Interval<UU> const& ivl) const { return inside(*this,ivl); }
    template<class UU> decltype(auto) _separated(Interval<UU> const& ivl) const { return separated(*this,ivl); }
};

template<class U> class IntervalSet : public IntervalSetBase<U> {
  public:
    IntervalSet(Interval<U>&& ivl) : IntervalSetBase<U>(std::move(ivl)) { }
    Interval<U> const& bounding_box() const { return *this; }
    template<class UU> decltype(auto) inside(Interval<UU> const& ivl) const { return this->_inside(ivl); }
    template<class UU> decltype(auto) separated(Interval<UU> const& ivl) const { return this->_separated(ivl); }
};



} // namespace Ariadne

#endif // ARIADNE_GEOMETRY_INTERVAL_SET_HPP
