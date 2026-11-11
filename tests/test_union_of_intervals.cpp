/***************************************************************************
 *            test_union_of_intervals.cpp
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Ariadne Geometry. If not, see <https://www.gnu.org/licenses/>.
 */

#include "geometry/union_of_intervals.hpp"
#include "geometry/set.hpp"
#include "ariadne-testing.hpp"
#include <sstream>
using namespace Ariadne;

class TestUnionOfIntervals : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

Void test_interval_union() {
    using U=UnionOfIntervals<Dyadic>;
    U empty({});
    ARIADNE_TESTING_EXPECT_EQUAL(empty.size(), 0u);
    ARIADNE_TESTING_EXPECT_EQUAL(empty.measure(), 0);
    ARIADNE_TESTING_EXPECT_EXCEPTION(empty.bounding_box());
    U left({{0,1},{3,4}});
    U right({{2,3.5_dy}});
    ARIADNE_TESTING_EXPECT_EQUAL(intersection(left,right).measure(), 0.5_dy);
    ARIADNE_TESTING_EXPECT_EQUAL(intersection(right,left).measure(), 0.5_dy);
    ARIADNE_TESTING_EXPECT_EQUAL(intersection(empty,left).size(), 0u);
    ARIADNE_TESTING_EXPECT_EQUAL(intersection(left,empty).size(), 0u);
    ARIADNE_TESTING_PROVE(!intersect(empty,left));
    ARIADNE_TESTING_PROVE(!intersect(left,empty));
    ARIADNE_TESTING_PROVE(intersect(left,right));
    ARIADNE_TESTING_PROVE(intersect(right,left));
    ARIADNE_TESTING_PROVE(!intersect(U({{0,1}}),U({{2,3}})));
    ARIADNE_TESTING_PROVE(!intersect(U({{2,3}}),U({{0,1}})));
    ARIADNE_TESTING_PROVE(subset(empty,left));
    ARIADNE_TESTING_PROVE(!subset(left,empty));
    ARIADNE_TESTING_PROVE(subset(left,left));
    ARIADNE_TESTING_PROVE(subset(U({{3,3.5_dy}}),left));
    ARIADNE_TESTING_PROVE(!subset(U({{-1,0.5_dy}}),left));
    ARIADNE_TESTING_PROVE(!subset(U({{0.5_dy,1.5_dy}}),left));
    ARIADNE_TESTING_EXPECT_EQUAL(left.bounding_box(), DyadicInterval(0,4));
    ARIADNE_TESTING_EXPECT_EQUAL(measure(left), 2);
    ARIADNE_TESTING_EXPECT_EQUAL(left.dimension(), SizeOne());
    ARIADNE_TESTING_PROVE(left.separated(IntervalDomainType(1.5_dy,2.5_dy)));
    ARIADNE_TESTING_PROVE(left.overlaps(IntervalDomainType(0.5_dy,2.5_dy)));
    ARIADNE_TESTING_PROVE(left.covers(IntervalDomainType(0.25_dy,0.75_dy)));
    ARIADNE_TESTING_PROVE(left.inside(IntervalDomainType(-1,5)));
    ARIADNE_TESTING_EXPECT_EQUAL(intersection(left,IntervalDomainType(0.5_dy,3.5_dy)).measure(), 1);
    U sorted({{3,4},{0,2},{0,1},{0,3},{5,6}});
    ARIADNE_TESTING_EXPECT_EQUAL(sorted.size(), 2u);
    ARIADNE_TESTING_EXPECT_EQUAL(sorted.measure(), 5);
    ARIADNE_TESTING_EXPECT_EQUAL(sorted.begin()->lower_bound(), 0);
    const U& const_sorted=sorted;
    ARIADNE_TESTING_EXPECT_EQUAL(const_sorted.begin()->upper_bound(), 4);
    ARIADNE_TESTING_EXPECT_EQUAL(interior(left,Dyadic(0.25_dy)).measure(), 1);
    ARIADNE_TESTING_EXPECT_EQUAL(interior(left,Dyadic(1)).size(), 0u);
    LowerUpperBoundLess less;
    ARIADNE_TESTING_PROVE(less(DyadicInterval(0,1),DyadicInterval(1,2)));
    ARIADNE_TESTING_PROVE(less(DyadicInterval(0,1),DyadicInterval(0,2)));
    ARIADNE_TESTING_PROVE(!less(DyadicInterval(0,2),DyadicInterval(0,1)));
    ARIADNE_TESTING_PROVE(!less(DyadicInterval(1,2),DyadicInterval(0,2)));
    std::ostringstream repr;
    repr<<left;
    ARIADNE_TESTING_PROVE(!repr.str().empty());
}

Void test_floating_interval_union() {
    UnionOfIntervals<FloatDP> floating({{3,4},{0,2},{1,3}},dp);
    ARIADNE_TESTING_EXPECT_EQUAL(floating.size(), 1u);
    ARIADNE_TESTING_EXPECT_EQUAL(floating.measure().upper().raw(), FloatDP(4,dp));
    ARIADNE_TESTING_PROVE(floating.overlaps(IntervalDomainType(1,2)));
    ARIADNE_TESTING_EXPECT_EQUAL(floating.create(IntervalDomainType(1,2)).measure().upper().raw(), FloatDP(1,dp));
    UnionOfIntervals<FloatDP> empty({},dp);
    ARIADNE_TESTING_EXPECT_EQUAL(empty.measure().upper().raw(), FloatDP(0,dp));
    ARIADNE_TESTING_EXPECT_EQUAL(empty.create(IntervalDomainType(1,2)).size(), 1u);
    UnionOfIntervals<FloatMP> multiprecision({{0,2}},MultiplePrecision(128));
    ARIADNE_TESTING_EXPECT_EQUAL(multiprecision.create(IntervalDomainType(1,2)).size(), 1u);
    UnionOfIntervals<FloatMP> empty_mp({},MultiplePrecision(128));
    ARIADNE_TESTING_EXPECT_EQUAL(empty_mp.create(IntervalDomainType(1,2)).size(), 1u);
    ARIADNE_TESTING_EXPECT_EQUAL(empty_mp.measure().upper().raw(), FloatMP(0,FloatMP::get_default_precision()));
    ARIADNE_TESTING_EXPECT_EXCEPTION(empty.bounding_box());
    LowerUpperBoundLess less;
    ARIADNE_TESTING_PROVE(less(ExactIntervalType(0,1),ExactIntervalType(1,2)));
    ARIADNE_TESTING_PROVE(less(ExactIntervalType(0,1),ExactIntervalType(0,2)));
    ARIADNE_TESTING_PROVE(!less(ExactIntervalType(0,2),ExactIntervalType(0,1)));
    ARIADNE_TESTING_PROVE(!less(ExactIntervalType(1,2),ExactIntervalType(0,2)));
    // Raw floating comparisons use short-circuit Boolean conditions;
    // exercise every outcome in this instantiation as well.
    auto make=[](InitializerList<DyadicInterval> intervals) { return UnionOfIntervals<FloatDP>(intervals,dp); };
    auto first=make({{0,1},{3,4}});
    auto second=make({{2,3.5_dy}});
    auto no_intervals=make({});
    ARIADNE_TESTING_EXPECT_EQUAL(first.dimension(), SizeOne());
    ARIADNE_TESTING_EXPECT_EQUAL(first.bounding_box(), ExactIntervalType(0,4));
    ARIADNE_TESTING_PROVE(first.inside(IntervalDomainType(-1,5)));
    ARIADNE_TESTING_PROVE(first.covers(IntervalDomainType(0.25_dy,0.75_dy)));
    ARIADNE_TESTING_PROVE(first.separated(IntervalDomainType(1.5_dy,2.5_dy)));
    ARIADNE_TESTING_PROVE(intersect(first,second));
    ARIADNE_TESTING_PROVE(intersect(second,first));
    ARIADNE_TESTING_PROVE(!intersect(first,no_intervals));
    ARIADNE_TESTING_PROVE(!intersect(no_intervals,first));
    ARIADNE_TESTING_PROVE(!intersect(make({{0,1}}),make({{2,3}})));
    ARIADNE_TESTING_PROVE(!intersect(make({{2,3}}),make({{0,1}})));
    ARIADNE_TESTING_PROVE(subset(no_intervals,first));
    ARIADNE_TESTING_PROVE(!subset(first,no_intervals));
    ARIADNE_TESTING_PROVE(subset(first,first));
    ARIADNE_TESTING_PROVE(subset(make({{3,3.5_dy}}),first));
    ARIADNE_TESTING_PROVE(!subset(make({{-1,0.5_dy}}),first));
    ARIADNE_TESTING_PROVE(!subset(make({{0.5_dy,1.5_dy}}),first));
    auto survivor=make({{0,1},{0.5_dy,2},{3,4}});
    ARIADNE_TESTING_EXPECT_EQUAL(survivor.size(), 2u);
    ARIADNE_TESTING_EXPECT_EQUAL(survivor.measure().upper().raw(), FloatDP(3,dp));
    std::ostringstream floating_repr;
    floating_repr<<first;
    ARIADNE_TESTING_PROVE(!floating_repr.str().empty());
    UnionOfIntervals<Dyadic> exact({{0,1}});
    UnionOfIntervals<Real> converted(exact);
    ARIADNE_TESTING_EXPECT_EQUAL(converted.size(), 1u);
}
Void test_multiprecision_interval_union_ordering_and_compaction() {
    const MultiplePrecision precision(128);
    auto make=[&](InitializerList<DyadicInterval> intervals) {
        return UnionOfIntervals<FloatMP>(intervals,precision);
    };

    // Compare different lower bounds and both orders of equal lower bounds.
    auto shorter=make({{0,1}});
    auto longer=make({{0,2}});
    auto later=make({{3,4}});
    LowerUpperBoundLess less;
    ARIADNE_TESTING_PROVE(less(*shorter.begin(),*later.begin()));
    ARIADNE_TESTING_PROVE(!less(*later.begin(),*shorter.begin()));
    ARIADNE_TESTING_PROVE(less(*shorter.begin(),*longer.begin()));
    ARIADNE_TESTING_PROVE(!less(*longer.begin(),*shorter.begin()));
    ARIADNE_TESTING_PROVE(!less(*shorter.begin(),*shorter.begin()));

    // The first two intervals merge and the disjoint last interval moves.
    auto normalized=make({{3,4},{0.5_dy,2},{0,1}});
    ARIADNE_TESTING_EXPECT_EQUAL(normalized.size(), 2u);
    ARIADNE_TESTING_EXPECT_EQUAL(normalized.measure().upper().raw(), FloatMP(3,precision));
    ARIADNE_TESTING_EXPECT_EQUAL(normalized.begin()->lower_bound(), shorter.begin()->lower_bound());
    ARIADNE_TESTING_EXPECT_EQUAL(normalized.bounding_box().upper_bound(), later.begin()->upper_bound());
}

};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_union_of_intervals");
    ARIADNE_TESTING_ADD_TEST(TestUnionOfIntervals, test_interval_union);
    ARIADNE_TESTING_ADD_TEST(TestUnionOfIntervals, test_floating_interval_union);
    ARIADNE_TESTING_ADD_TEST(TestUnionOfIntervals, test_multiprecision_interval_union_ordering_and_compaction);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
