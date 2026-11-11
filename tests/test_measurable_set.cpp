/***************************************************************************
 *            test_measurable_set.cpp
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Geometry. If not, see <https://www.gnu.org/licenses/>.
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

#include "ariadne-testing.hpp"

using namespace std;
using namespace Ariadne;

template<class X> inline OutputStream& operator<<(OutputStream& os, Sequence<X> const& seq) {
    SequenceWriter writer(4u); return os << writer(seq);
}

class TestMeasurableSet : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;
    void set_up() { Dyadic::set_default_writer(DecimalWriter()); }

Void test_lower_measurable_set_measurement() {
    using Intervals=UnionOfIntervals<Dyadic>;
    const Intervals intervals({{0,0.5_dy},{1,2.5_dy}});
    ValidatedLowerMeasurableSet<Real> wrapped(intervals);

    // The public handle must delegate both whole-set and restricted measures.
    const auto total=wrapped.measure();
    const auto restricted=wrapped.measure_in(ExactIntervalType(0.25_x,2.25_x));
    ARIADNE_TESTING_EXPECT_EQUAL(total.get(dp).raw(), FloatDP(2,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(restricted.get(dp).raw(), FloatDP(1.5_dy,dp));

    std::ostringstream written;
    written << wrapped;
    ARIADNE_TESTING_PROVE(!written.str().empty());

    // Error bounds are clamped to [0, measure(set)] before subtraction.
    using Model=LowerMeasurableSetModel<Intervals,Dyadic>;
    const Model partial(intervals,Dyadic(0.5_dy));
    const Model excess(intervals,Dyadic(3));
    const Model negative(intervals,Dyadic(-1));
    ARIADNE_TESTING_EXPECT_EQUAL(measure(partial), PositiveDyadic(1.5_dy));
    ARIADNE_TESTING_EXPECT_EQUAL(measure(excess), PositiveDyadic(0u));
    ARIADNE_TESTING_EXPECT_EQUAL(measure(negative), PositiveDyadic(2u));
}



Void test_exponent_range()
{
    Dyadic::set_default_writer(DecimalWriter());
    ARIADNE_TESTING_EXPECT_EQUAL(exp2(Integer(-3)).exponent(), exp2(-3).exponent());
    // These fit in host long but exceed Int: rejection must come from exp2's
    // precondition, rather than the earlier checked Integer-to-long conversion.
    const Integer largest_exponent(std::numeric_limits<Int>::max());
    const Integer smallest_exponent(std::numeric_limits<Int>::min());
    ARIADNE_TESTING_EXPECT_EQUAL(exp2(largest_exponent).exponent(), std::numeric_limits<Int>::max());
    ARIADNE_TESTING_EXPECT_EQUAL(exp2(smallest_exponent).exponent(), std::numeric_limits<Int>::min());
    ARIADNE_TESTING_EXPECT_EXCEPTION(exp2(largest_exponent+1));
    ARIADNE_TESTING_EXPECT_EXCEPTION(exp2(smallest_exponent-1));
    ARIADNE_TESTING_EXPECT_EXCEPTION(exp2(Integer("18446744073709551616")));
}


Void test_scalar_set()
{
    {
        [[maybe_unused]] UnionOfIntervals<Dyadic> ivls1({{1,2},{1.5_dy,2.5_dy},{0,0.5_dy}});
        [[maybe_unused]] UnionOfIntervals<Dyadic> ivls2({{0.25_dy,2.25_dy}});
        ARIADNE_TESTING_EXPECT_EQUAL(ivls1.measure(), 2);
        ARIADNE_TESTING_EXPECT_EQUAL(intersection(ivls1,ivls2).measure(), 1.5_dy);
        ARIADNE_TESTING_EXPECT_EQUAL(join(ivls1,ivls2).measure(), 2.5_dy);

        [[maybe_unused]] UnionOfIntervals<FloatDP> ivls1dp({{1.0_dy,2.0_dy},{1.5_dy,2.5_dy},{0.0_dy,0.5_dy}},double_precision);
        [[maybe_unused]] UnionOfIntervals<FloatMP> ivls1mp({{1.0_dy,2.0_dy},{1.5_dy,2.5_dy},{0.0_dy,0.5_dy}},MultiplePrecision(128));
    }

    {
        [[maybe_unused]] LowerSequence<UnionOfIntervals<Dyadic>> lms([](Natural n){return UnionOfIntervals<Dyadic>({{exp2(-n),1.0_dy+exp2(-(n+1u))}});});
        [[maybe_unused]] IncreasingSequence<PositiveDyadic> ms(lms.measures());
    }

    {
        [[maybe_unused]] UnionOfIntervals<Dyadic> ivls({{1,2},{1.5_dy,2.5_dy},{0,0.5_dy}});
        [[maybe_unused]] ValidatedOpenSet<Real> ops = wrap_open(ivls);


        PositiveDyadic err(0u);
        LowerMeasurableSetModel lmsm(ivls,err);

        static_assert(IsLowerMeasurableSet<UnionOfIntervals<Dyadic>,ValidatedTag,Real>::value);
        static_assert(IsLowerMeasurableSet<LowerMeasurableSetModel<UnionOfIntervals<Dyadic>,Dyadic>,ValidatedTag,Real>::value);

        ValidatedLowerMeasurableSet<Real> ilms(ivls);

    }
}


};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_measurable_set");
    ARIADNE_TESTING_ADD_TEST(TestMeasurableSet, test_scalar_set);
    ARIADNE_TESTING_ADD_TEST(TestMeasurableSet, test_lower_measurable_set_measurement);
    ARIADNE_TESTING_ADD_TEST(TestMeasurableSet, test_exponent_range);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
