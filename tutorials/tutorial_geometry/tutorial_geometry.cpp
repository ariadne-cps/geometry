// Geometry fundamentals: Point, Box, abstract sets and interval unions.
// Deliberately uses no solvers, constrained images, grids or paving algorithms.
#include <iostream>
#include <string>

#include "geometry/point.hpp"
#include "geometry/box.hpp"
#include "geometry/set.hpp"
#include "geometry/set_wrapper.hpp"
#include "geometry/union_of_intervals.hpp"

using namespace Ariadne;

template<class T>
void show(const std::string& label, const T& value) {
    std::cout << label << ": " << value << '\n';
}

void section(const char* name) {
    std::cout << "\n== " << name << " ==\n";
}

int main() {
    section("Points and coordinate spaces");
    EuclideanSpace space(2);
    FloatDPPoint sample({ExactDouble(0.5), ExactDouble(0.25)}, dp);
    show("space dimension", space.dimension());
    show("sample point", sample);
    show("point dimension", sample.dimension());
    show("point bounding box", sample.bounding_box());

    section("Boxes and validated geometric predicates");
    ExactBoxType workspace({{-2.0_x, 2.0_x}, {-1.5_x, 1.5_x}});
    ExactBoxType observation({{-0.75_x, 1.25_x}, {-0.5_x, 1.0_x}});
    ExactBoxType shifted({{1.0_x, 3.0_x}, {-0.25_x, 0.5_x}});
    show("workspace", workspace);
    show("dimension", workspace.dimension());
    show("centre", workspace.centre());
    show("midpoint", workspace.midpoint());
    show("widths", workspace.widths());
    show("radii", workspace.radii());
    show("measure", workspace.measure());
    show("contains sample", workspace.contains(sample));
    show("covers observation", workspace.covers(observation));
    show("observation inside workspace", observation.inside(workspace));
    show("workspace overlaps shifted", workspace.overlaps(shifted));

    section("Operations on boxes");
    show("intersection", intersection(workspace, observation));
    show("bounding hull", hull(workspace, shifted));
    show("Cartesian product", product(workspace, observation));
    auto halves = observation.split();
    show("lower half", halves.first);
    show("upper half", halves.second);
    show("unit box in 2D", ExactBoxType::unit_box(2));

    section("Set interfaces and type-erased handles");
    // An ordinary Box implements the semantic predicates of several
    // abstract sets; no concrete solver-based set is needed here.
    LocatedSet<ValidatedTag, RealVector> located(workspace);
    OpenSet<ValidatedTag, RealVector> open(workspace);
    show("located set dimension", located.dimension());
    show("located set bounding box", located.bounding_box());
    show("open set covers observation", open.covers(observation));

    section("Finite unions of scalar intervals");
    UnionOfIntervals<Dyadic> intervals({{0,1}, {2,3}});
    UnionOfIntervals<Dyadic> mask({{0.5_dy,2.5_dy}});
    show("interval union", intervals);
    show("interval count", intervals.size());
    show("bounding interval", intervals.bounding_box());
    show("union measure", intervals.measure());
    show("intersected intervals", intersection(intervals,mask));
    show("joined intervals", join(intervals,mask));
    show("intersection measure", intersection(intervals,mask).measure());

    std::cout << "\nGeometry deliberately ends at fundamental representations.\n"
                 "See tutorial_set for constrained sets and curves, and "
                 "tutorial_paving for discrete approximations.\n";
    return 0;
}
