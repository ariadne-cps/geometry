/***************************************************************************
 *            test_measurable_set.cpp
 *
 *  Copyright  2020  Pieter Collins
 *
 ****************************************************************************/

/*
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Ariadne is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <stdexcept>
#include <limits>
#include <fenv.h>
#include "geometry/set.hpp"
#include "geometry/set_wrapper.hpp"
#include "geometry/measurable_set.hpp"

#include "macro/testing.hpp"

using namespace std;
using namespace Ariadne;

template<class X> inline OutputStream& operator<<(OutputStream& os, Sequence<X> const& seq) {
    SequenceWriter writer(4u); return os << writer(seq);
}

Void test_lower_measurable_set_measurement() {
    using Intervals=UnionOfIntervals<Dyadic>;
    const Intervals intervals({{0,0.5_dy},{1,2.5_dy}});
    ValidatedLowerMeasurableSet<Real> wrapped(intervals);

    // The public handle must delegate both whole-set and restricted measures.
    const auto total=wrapped.measure();
    const auto restricted=wrapped.measure_in(ExactIntervalType(0.25_x,2.25_x));
    ARIADNE_TEST_EQUALS(total.get(dp).raw(),FloatDP(2,dp));
    ARIADNE_TEST_EQUALS(restricted.get(dp).raw(),FloatDP(1.5_dy,dp));

    std::ostringstream written;
    written << wrapped;
    ARIADNE_TEST_ASSERT(!written.str().empty());

    // Error bounds are clamped to [0, measure(set)] before subtraction.
    using Model=LowerMeasurableSetModel<Intervals,Dyadic>;
    const Model partial(intervals,Dyadic(0.5_dy));
    const Model excess(intervals,Dyadic(3));
    const Model negative(intervals,Dyadic(-1));
    ARIADNE_TEST_EQUALS(measure(partial),PositiveDyadic(1.5_dy));
    ARIADNE_TEST_EQUALS(measure(excess),PositiveDyadic(0u));
    ARIADNE_TEST_EQUALS(measure(negative),PositiveDyadic(2u));
}

class TestMeasurableSet
{
  public:
    Void test();
  private:
    Void test_concept();
    Void test_scalar_set();
};

Void TestMeasurableSet::test()
{
    Dyadic::set_default_writer(DecimalWriter());
    ARIADNE_TEST_CALL(test_scalar_set());
    ARIADNE_TEST_CALL(test_lower_measurable_set_measurement());
    ARIADNE_TEST_EQUALS(exp2(Integer(-3)).exponent(),exp2(-3).exponent());
    // These fit in host long but exceed Int: rejection must come from exp2's
    // precondition, rather than the earlier checked Integer-to-long conversion.
    const Integer largest_exponent(std::numeric_limits<Int>::max());
    const Integer smallest_exponent(std::numeric_limits<Int>::min());
    ARIADNE_TEST_EQUALS(exp2(largest_exponent).exponent(),std::numeric_limits<Int>::max());
    ARIADNE_TEST_EQUALS(exp2(smallest_exponent).exponent(),std::numeric_limits<Int>::min());
    ARIADNE_TEST_FAIL(exp2(largest_exponent+1));
    ARIADNE_TEST_FAIL(exp2(smallest_exponent-1));
    ARIADNE_TEST_FAIL(exp2(Integer("18446744073709551616")));
}

Void TestMeasurableSet::test_concept()
{
}

Void TestMeasurableSet::test_scalar_set()
{
    {
        ARIADNE_TEST_CONSTRUCT(UnionOfIntervals<Dyadic>,ivls1,({{1,2},{1.5_dy,2.5_dy},{0,0.5_dy}}));
        ARIADNE_TEST_CONSTRUCT(UnionOfIntervals<Dyadic>,ivls2,({{0.25_dy,2.25_dy}}));
        ARIADNE_TEST_EQUALS(ivls1.measure(),2);
        ARIADNE_TEST_EQUALS(intersection(ivls1,ivls2).measure(),1.5_dy);
        ARIADNE_TEST_EQUALS(join(ivls1,ivls2).measure(),2.5_dy);

        ARIADNE_TEST_CONSTRUCT(UnionOfIntervals<FloatDP>,ivls1dp,({{1.0_dy,2.0_dy},{1.5_dy,2.5_dy},{0.0_dy,0.5_dy}},double_precision));
        ARIADNE_TEST_CONSTRUCT(UnionOfIntervals<FloatMP>,ivls1mp,({{1.0_dy,2.0_dy},{1.5_dy,2.5_dy},{0.0_dy,0.5_dy}},MultiplePrecision(128)));
    }

    {
        ARIADNE_TEST_CONSTRUCT(LowerSequence<UnionOfIntervals<Dyadic>>,lms,([](Natural n){return UnionOfIntervals<Dyadic>({{exp2(-n),1.0_dy+exp2(-(n+1u))}});}));
        ARIADNE_TEST_CONSTRUCT(IncreasingSequence<PositiveDyadic>,ms,(lms.measures()));
    }

    {
        ARIADNE_TEST_CONSTRUCT(UnionOfIntervals<Dyadic>,ivls,({{1,2},{1.5_dy,2.5_dy},{0,0.5_dy}}));
        ARIADNE_TEST_ASSIGN_CONSTRUCT(ValidatedOpenSet<Real>,ops,wrap_open(ivls));


        PositiveDyadic err(0u);
        LowerMeasurableSetModel lmsm(ivls,err);

        static_assert(IsLowerMeasurableSet<UnionOfIntervals<Dyadic>,ValidatedTag,Real>::value);
        static_assert(IsLowerMeasurableSet<LowerMeasurableSetModel<UnionOfIntervals<Dyadic>,Dyadic>,ValidatedTag,Real>::value);

        ValidatedLowerMeasurableSet<Real> ilms(ivls);

    }
}


Int main() {
    TestMeasurableSet().test();

    return ARIADNE_TEST_FAILURES;
}

