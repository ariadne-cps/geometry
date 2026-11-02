/***************************************************************************
 *            test_point.cpp
 *
 *  This file is part of Ariadne.
 *
 *  Ariadne is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 ****************************************************************************/

#include "geometry/point.hpp"
#include "geometry/point.tpl.hpp"
#include <cmath>
#include <memory>
#include <sstream>
#include "utility/string.hpp"
#include "ariadne-testing.hpp"

using namespace Ariadne;

// These explicit specializations provide the stable human-readable names
// used by diagnostics and other type-aware clients.
class TestPoint : public Ariadne::Testing::ContextFixture {
  public:
    using ContextFixture::ContextFixture;

Void test_point_class_names() {
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Point<Real>>(), String("Point<Real>"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Point<FloatDP>>(), String("Point<FloatDP>"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Point<FloatDPBounds>>(), String("Point<FloatDPBounds>"));
    ARIADNE_TESTING_EXPECT_EQUAL(class_name<Point<FloatDPApproximation>>(), String("Point<FloatDPApproximation>"));
}

Void test_point_geometry() {
    Point<FloatDPApproximation> pt({1.0_x,2.0_x},dp);
    Point<FloatDPApproximation> zero;
    ARIADNE_TESTING_EXPECT_EQUAL(zero.dimension(), 0u);
    Point<FloatDP> generated(2u,[](SizeType i){return FloatDP(static_cast<Int>(i),dp);});
    Point<Real> generated_real(2u,[](SizeType i){return Real(static_cast<Int>(i));});
    // Exercise the public initializer-list constructor with Real coordinates.
    Point<Real> listed({Real(1),Real(2)});
    ARIADNE_TESTING_EXPECT_EQUAL(listed.dimension(), 2u);
    ARIADNE_TESTING_PROVE(decide(listed[0]==Real(1)));
    ARIADNE_TESTING_PROVE(decide(listed[1]==Real(2)));
    ARIADNE_TESTING_EXPECT_EQUAL(generated[1], FloatDP(1,dp));
    ARIADNE_TESTING_PROVE(decide(generated_real[1]==1));
    auto bounds=pt.bounding_box();
    ARIADNE_TESTING_EXPECT_EQUAL(bounds.dimension(), 2u);
    ARIADNE_TESTING_INSPECT("bounds", bounds);
    ARIADNE_TESTING_PROVE(definitely(subset(ExactBoxType{{1.0_x,1.0_x},{2.0_x,2.0_x}},cast_exact_box(bounds))));
    ARIADNE_TESTING_EXPECT_EQUAL(zero.bounding_box().dimension(), 0u);
    std::unique_ptr<Point<FloatDPApproximation>> clone(pt.clone());
    ARIADNE_TESTING_PROVE(decide(*clone==pt));
    ARIADNE_TESTING_PROVE(decide(clone->centre()==pt.vector()));
    std::ostringstream repr;
    pt._write(repr);
    ARIADNE_TESTING_PROVE(!repr.str().empty());
    auto joined=product(pt,pt);
    ARIADNE_TESTING_EXPECT_EQUAL(joined.dimension(), 4u);
    auto origin=Point<FloatDPApproximation>::origin(2u,dp);
    ARIADNE_TESTING_PROVE(std::abs(distance(pt,origin).raw().get_d()-std::sqrt(5.0))<1e-12);
    Point<FloatDPApproximation> line({3.0_x},dp);
    Point<FloatDPApproximation> line_origin({0.0_x},dp);
    ARIADNE_TESTING_EXPECT_EQUAL(distance(line,line_origin).raw().get_d(), 3.0);
    ARIADNE_TESTING_EXPECT_EXCEPTION(distance(pt,line));
    ARIADNE_TESTING_EXPECT_EXCEPTION(distance(zero,zero));
}

};

int main(int argc, char** argv) {
    ARIADNE_TESTING_SUITE("test_point");
    ARIADNE_TESTING_ADD_TEST(TestPoint, test_point_geometry);
    ARIADNE_TESTING_ADD_TEST(TestPoint, test_point_class_names);
    return ARIADNE_TESTING_RUN_TESTS_WITH_ARGS(argc, argv);
}
