/***************************************************************************
 *            test_set_adapters.cpp
 *
 *  This file is part of Ariadne, distributed under the GNU GPL version 3
 *  or (at your option) any later version.
 ****************************************************************************/
#include "geometry/set_wrapper.hpp"
#include "geometry/union_of_intervals.hpp"
#include "geometry/interval_set.hpp"
#include "macro/testing.hpp"
#include <memory>
#include <sstream>
#include <type_traits>
using namespace Ariadne;

static_assert(std::is_same_v<RegularLocatedSetInterface<ValidatedTag,RealVector>::BasicSetType,ExactBoxType>);
static_assert(std::is_same_v<RegularLocatedSetInterface<ValidatedTag,Real>::BasicSetType,ExactIntervalType>);

template<class C> Void check_set_adapter() {
    ExactBoxType shape{{0.0_x,1.0_x}}, inner{{0.25_x,0.75_x}}, outside{{2.0_x,3.0_x}}, enclosing{{-1.0_x,2.0_x}};
    using W=WrapperType<ExactBoxType,C>;
    W wrapper(shape);
    C handle(shape);
    C cloned=wrap<C>(wrapper);
    ARIADNE_TEST_EQUALS(handle.dimension(),1u);
    ARIADNE_TEST_EQUALS(cloned.dimension(),1u);
    if constexpr(requires { handle.overlaps(inner); }) {
        ARIADNE_TEST_ASSERT(definitely(handle.overlaps(inner)));
        ARIADNE_TEST_ASSERT(!definitely(handle.overlaps(outside)));
    }
    if constexpr(requires { handle.covers(inner); }) {
        ARIADNE_TEST_ASSERT(definitely(handle.covers(inner)));
        ARIADNE_TEST_ASSERT(!definitely(handle.covers(outside)));
    }
    if constexpr(requires { handle.separated(outside); }) {
        ARIADNE_TEST_ASSERT(definitely(handle.separated(outside)));
        ARIADNE_TEST_ASSERT(!definitely(handle.separated(inner)));
    }
    if constexpr(requires { handle.inside(enclosing); }) {
        ARIADNE_TEST_ASSERT(definitely(handle.inside(enclosing)));
        ARIADNE_TEST_ASSERT(!definitely(handle.inside(outside)));
        ARIADNE_TEST_ASSERT(same(cast_exact_box(handle.bounding_box()),shape));
    }
    std::ostringstream expected,actual,direct;
    expected<<shape; actual<<handle; direct<<wrapper;
    ARIADNE_TEST_EQUALS(actual.str(),expected.str());
    ARIADNE_TEST_EQUALS(direct.str(),expected.str());
}

Void test_set_adapters() {
    ARIADNE_TEST_CALL((check_set_adapter<OpenSet<ValidatedTag,RealVector>>()));
    ARIADNE_TEST_CALL((check_set_adapter<ClosedSet<ValidatedTag,RealVector>>()));
    ARIADNE_TEST_CALL((check_set_adapter<OvertSet<ValidatedTag,RealVector>>()));
    ARIADNE_TEST_CALL((check_set_adapter<CompactSet<ValidatedTag,RealVector>>()));
    ARIADNE_TEST_CALL((check_set_adapter<RegularSet<ValidatedTag,RealVector>>()));
    ARIADNE_TEST_CALL((check_set_adapter<LocatedSet<ValidatedTag,RealVector>>()));
    ARIADNE_TEST_CALL((check_set_adapter<RegularLocatedSet<ValidatedTag,RealVector>>()));
    EuclideanSpace space(3u);
    ARIADNE_TEST_EQUALS(space.dimension(),3u);
    UnionOfIntervals<FloatDP> intervals({{0,1}},dp);
    RegularSet<ValidatedTag,Real> regular(intervals);
    ARIADNE_TEST_ASSERT(definitely(regular.covers(ExactIntervalType(0.25_x,0.75_x))));
    ARIADNE_TEST_ASSERT(definitely(regular.separated(ExactIntervalType(2,3))));
    auto open=wrap_open(intervals);
    ARIADNE_TEST_EQUALS(open.dimension(),1u);
    ARIADNE_TEST_ASSERT(definitely(open.overlaps(ExactIntervalType(0.25_x,0.75_x))));
    ARIADNE_TEST_ASSERT(definitely(open.covers(ExactIntervalType(0.25_x,0.75_x))));
    std::unique_ptr<SetInterfaceBase<Real>> cloned(open.reference().clone());
    ARIADNE_TEST_EQUALS(cloned->dimension(),1u);
    IntervalSet<FloatDPUpperBound> interval(UpperIntervalType(0,1));
    CompactSet<ValidatedTag,Real> compact(interval);
    ARIADNE_TEST_EQUALS(compact.dimension(),1u);
    ARIADNE_TEST_ASSERT(definitely(compact.inside(ExactIntervalType(-1,2))));
    ARIADNE_TEST_ASSERT(definitely(compact.separated(ExactIntervalType(2,3))));
    ARIADNE_TEST_ASSERT(same(cast_exact_interval(compact.bounding_box()),ExactIntervalType(0,1)));
    std::unique_ptr<SetInterfaceBase<Real>> compact_clone(compact.reference().clone());
    ARIADNE_TEST_EQUALS(compact_clone->dimension(),1u);
    std::ostringstream text; text<<compact;
    ARIADNE_TEST_ASSERT(!text.str().empty());
}
Int main() {
    ARIADNE_TEST_CALL(test_set_adapters());
    return ARIADNE_TEST_FAILURES;
}
