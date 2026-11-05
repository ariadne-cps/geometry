/***************************************************************************
 *            test_set_adapters.cpp
 *
 *  This file is part of Ariadne, distributed under the GNU GPL version 3
 *  or (at your option) any later version.
 ****************************************************************************/
#include "geometry/set_wrapper.hpp"
#include "geometry/union_of_intervals.hpp"
#include "geometry/interval_set.hpp"
#include "ariadne-testing.hpp"
#include <memory>
#include <sstream>
#include <type_traits>
using namespace Ariadne;

static_assert(std::is_same_v<RegularLocatedSetInterface<ValidatedTag,RealVector>::BasicSetType,ExactBoxType>);
static_assert(std::is_same_v<RegularLocatedSetInterface<ValidatedTag,Real>::BasicSetType,ExactIntervalType>);

class TestSetAdapters : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

template<class C> Void check_set_adapter() {
    ExactBoxType shape{{0.0_x,1.0_x}}, inner{{0.25_x,0.75_x}}, outside{{2.0_x,3.0_x}}, enclosing{{-1.0_x,2.0_x}};
    using W=SetWrapperType<ExactBoxType,C>;
    W wrapper(shape);
    C handle(shape);
    C cloned=wrap<C>(wrapper);
    ARIADNE_TESTING_EXPECT_EQUAL(handle.dimension(), 1u);
    ARIADNE_TESTING_EXPECT_EQUAL(cloned.dimension(), 1u);
    if constexpr(requires { handle.overlaps(inner); }) {
        ARIADNE_TESTING_PROVE(definitely(handle.overlaps(inner)));
        ARIADNE_TESTING_PROVE(!definitely(handle.overlaps(outside)));
    }
    if constexpr(requires { handle.covers(inner); }) {
        ARIADNE_TESTING_PROVE(definitely(handle.covers(inner)));
        ARIADNE_TESTING_PROVE(!definitely(handle.covers(outside)));
    }
    if constexpr(requires { handle.separated(outside); }) {
        ARIADNE_TESTING_PROVE(definitely(handle.separated(outside)));
        ARIADNE_TESTING_PROVE(!definitely(handle.separated(inner)));
    }
    if constexpr(requires { handle.inside(enclosing); }) {
        ARIADNE_TESTING_PROVE(definitely(handle.inside(enclosing)));
        ARIADNE_TESTING_PROVE(!definitely(handle.inside(outside)));
        ARIADNE_TESTING_PROVE(same(cast_exact_box(handle.bounding_box()),shape));
    }
    std::ostringstream expected,actual,direct;
    expected<<shape; actual<<handle; direct<<wrapper;
    ARIADNE_TESTING_EXPECT_EQUAL(actual.str(), expected.str());
    ARIADNE_TESTING_EXPECT_EQUAL(direct.str(), expected.str());
}

    void test_open_set_adapter() { check_set_adapter<OpenSet<ValidatedTag,RealVector>>(); }
    void test_closed_set_adapter() { check_set_adapter<ClosedSet<ValidatedTag,RealVector>>(); }
    void test_overt_set_adapter() { check_set_adapter<OvertSet<ValidatedTag,RealVector>>(); }
    void test_compact_set_adapter() { check_set_adapter<CompactSet<ValidatedTag,RealVector>>(); }
    void test_regular_set_adapter() { check_set_adapter<RegularSet<ValidatedTag,RealVector>>(); }
    void test_located_set_adapter() { check_set_adapter<LocatedSet<ValidatedTag,RealVector>>(); }
    void test_regular_located_set_adapter() { check_set_adapter<RegularLocatedSet<ValidatedTag,RealVector>>(); }

// These short identifiers are part of the predicate adapters' diagnostic API.
Void test_predicate_operation_codes() {
    ARIADNE_TESTING_EXPECT_EQUAL(std::string(Overlap{}.code()), std::string("overlap"));
    ARIADNE_TESTING_EXPECT_EQUAL(std::string(Separated{}.code()), std::string("separated"));
    ARIADNE_TESTING_EXPECT_EQUAL(std::string(Inside{}.code()), std::string("inside"));
    ARIADNE_TESTING_EXPECT_EQUAL(std::string(Covers{}.code()), std::string("covers"));
}

Void test_set_adapters() {







    EuclideanSpace space(3u);
    ARIADNE_TESTING_EXPECT_EQUAL(space.dimension(), 3u);
    UnionOfIntervals<FloatDP> intervals({{0,1}},dp);
    RegularSet<ValidatedTag,Real> regular(intervals);
    ARIADNE_TESTING_PROVE(definitely(regular.covers(ExactIntervalType(0.25_x,0.75_x))));
    ARIADNE_TESTING_PROVE(definitely(regular.separated(ExactIntervalType(2,3))));
    auto open=wrap_open(intervals);
    ARIADNE_TESTING_EXPECT_EQUAL(open.dimension(), 1u);
    ARIADNE_TESTING_PROVE(definitely(open.overlaps(ExactIntervalType(0.25_x,0.75_x))));
    ARIADNE_TESTING_PROVE(definitely(open.covers(ExactIntervalType(0.25_x,0.75_x))));
    std::unique_ptr<SetInterfaceBase<Real>> cloned(open.reference().clone());
    ARIADNE_TESTING_EXPECT_EQUAL(cloned->dimension(), 1u);
    IntervalSet<FloatDPUpperBound> interval(UpperIntervalType(0,1));
    CompactSet<ValidatedTag,Real> compact(interval);
    ARIADNE_TESTING_EXPECT_EQUAL(compact.dimension(), 1u);
    ARIADNE_TESTING_PROVE(definitely(compact.inside(ExactIntervalType(-1,2))));
    ARIADNE_TESTING_PROVE(definitely(compact.separated(ExactIntervalType(2,3))));
    ARIADNE_TESTING_PROVE(same(cast_exact_interval(compact.bounding_box()),ExactIntervalType(0,1)));
    std::unique_ptr<SetInterfaceBase<Real>> compact_clone(compact.reference().clone());
    ARIADNE_TESTING_EXPECT_EQUAL(compact_clone->dimension(), 1u);
    std::ostringstream text; text<<compact;
    ARIADNE_TESTING_PROVE(!text.str().empty());
}
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_set_adapters");
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_set_adapters);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_predicate_operation_codes);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_open_set_adapter);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_closed_set_adapter);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_overt_set_adapter);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_compact_set_adapter);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_regular_set_adapter);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_located_set_adapter);
    ARIADNE_TESTING_ADD_TEST(TestSetAdapters, test_regular_located_set_adapter);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
