/***************************************************************************
 *            test_geometry.cpp
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

#include "geometry/geometry.hpp"
#include "ariadne-testing.hpp"
using namespace Ariadne;
using Located=LocatedSetInterface<EffectiveTag,RealVector>;
using Regular=RegularSetInterface<EffectiveTag,RealVector>;

class TestGeometry : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

Void test_geometric_relations() {
    const FloatDP tolerance(0.015625_x,dp);
    ExactBoxSetType source(ExactBoxType{{0.0_x,1.0_x}});
    ExactBoxSetType enclosing(ExactBoxType{{-1.0_x,2.0_x}});
    ExactBoxSetType disjoint_set(ExactBoxType{{2.0_x,3.0_x}});
    ExactBoxSetType touching(ExactBoxType{{1.0_x,2.0_x}});
    const Located& located=source;
    const Regular& enclosing_regular=enclosing;
    const Regular& disjoint_regular=disjoint_set;
    const Regular& touching_regular=touching;
    ARIADNE_TESTING_PROVE(definitely(overlap(located,enclosing_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(inside(located,enclosing_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(!separated(located,enclosing_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(!overlap(located,disjoint_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(!inside(located,disjoint_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(separated(located,disjoint_regular,tolerance)));
    ARIADNE_TESTING_PROVE(is_indeterminate(overlap(located,touching_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(!inside(located,touching_regular,tolerance)));
    ARIADNE_TESTING_PROVE(is_indeterminate(separated(located,touching_regular,tolerance)));
    ExactBoxSetType empty(ExactBoxType(1u));
    const Located& empty_located=empty;
    ARIADNE_TESTING_PROVE(definitely(!overlap(empty_located,enclosing_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(inside(empty_located,enclosing_regular,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(separated(empty_located,enclosing_regular,tolerance)));
    ExactBoxType away{{4.0_x,5.0_x}};
    ARIADNE_TESTING_PROVE(definitely(!overlap(located,enclosing_regular,away,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(inside(located,enclosing_regular,away,tolerance)));
    ARIADNE_TESTING_PROVE(definitely(separated(located,enclosing_regular,away,tolerance)));

    ExactBoxType query{{0.0_x,1.0_x}};
    const ExactBoxType supports[]={ExactBoxType{{0.125_x,0.25_x}},ExactBoxType{{0.75_x,0.875_x}},ExactBoxType{{0.25_x,0.75_x}}};
    const ExactBoxType targets[]={ExactBoxType{{0.0625_x,0.3125_x}},ExactBoxType{{0.6875_x,0.9375_x}},ExactBoxType{{0.125_x,0.875_x}}};
    for(SizeType i=0u;i!=3u;++i) {
        ARIADNE_TESTING_INSPECT("supports[i]", supports[i]);
        ExactBoxSetType small_source(supports[i]);
        ExactBoxSetType small_target(targets[i]);
        const Located& small_located=small_source;
        const Regular& small_regular=small_target;
        ARIADNE_TESTING_PROVE(definitely(overlap(small_located,small_regular,query,tolerance)));
        ARIADNE_TESTING_PROVE(definitely(inside(small_located,small_regular,query,tolerance)));
        ARIADNE_TESTING_PROVE(definitely(!separated(small_located,small_regular,query,tolerance)));
    }
    // The query touches the source only on its boundary, so it supplies
    // no witness in the query's interior, even though the target covers it.
    ExactBoxSetType boundary_source(ExactBoxType{{0.0_x,0.25_x}});
    ExactBoxSetType boundary_target(ExactBoxType{{0.125_x,0.5_x}});
    const Located& boundary_located=boundary_source;
    const Regular& boundary_regular=boundary_target;
    ExactBoxType boundary_query{{0.25_x,0.375_x}};
    ARIADNE_TESTING_PROVE(is_indeterminate(overlap(boundary_located,boundary_regular,boundary_query,tolerance)));
    ARIADNE_TESTING_PROVE(is_indeterminate(separated(boundary_located,boundary_regular,boundary_query,tolerance)));
    ARIADNE_TESTING_PROVE(is_indeterminate(inside(boundary_located,disjoint_regular,boundary_query,tolerance)));
}
};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_geometry");
    ARIADNE_TESTING_ADD_TEST(TestGeometry, test_geometric_relations);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
