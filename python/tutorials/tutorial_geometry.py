#!/usr/bin/env python3
"""Fundamental geometry; counterpart of tutorials/tutorial_geometry (C++)."""
from pyariadne import *


def show(label, value):
    print(f"{label}: {value}")


def section(name):
    print(f"\n== {name} ==")


def tutorial_geometry():
    dp = DoublePrecision()
    section("Points and coordinate spaces")
    space = EuclideanSpace(2)
    point = FloatDPPoint([FloatDP(exact(0.5), dp), FloatDP(exact(0.25), dp)])
    show("space dimension", space.dimension())
    show("point", point)
    show("point dimension", point.dimension())
    show("point enclosure", point.bounding_box())

    section("Boxes and geometric predicates")
    workspace = FloatDPExactBox([
        {exact(-2): exact(2)}, {exact(-1.5): exact(1.5)}
    ])
    observation = FloatDPExactBox([
        {exact(-0.75): exact(1.25)}, {exact(-0.5): exact(1)}
    ])
    shifted = FloatDPExactBox([
        {exact(1): exact(3)}, {exact(-0.25): exact(0.5)}
    ])
    for label, value in (
        ("workspace", workspace),
        ("dimension", workspace.dimension()),
        ("centre", workspace.centre()),
        ("midpoint", workspace.midpoint()),
        ("widths", workspace.widths()),
        ("radii", workspace.radii()),
        ("area", workspace.measure()),
        ("contains point", workspace.contains(point)),
        ("covers observation", workspace.covers(observation)),
        ("observation inside workspace", observation.inside(workspace)),
        ("overlaps shifted", workspace.overlaps(shifted)),
    ):
        show(label, value)

    section("Enclosure algebra")
    show("intersection", intersection(workspace, observation))
    show("hull", hull(workspace, shifted))
    show("Cartesian product", product(workspace, observation))
    left, right = observation.split()
    show("left half", left)
    show("right half", right)

    section("Unions of disjoint intervals")
    intervals = DyadicUnionOfIntervals([
        DyadicInterval(Dyadic(0), Dyadic(1)),
        DyadicInterval(Dyadic(2), Dyadic(3)),
    ])
    mask = DyadicUnionOfIntervals([
        DyadicInterval(Dyadic(exact(0.5)), Dyadic(exact(2.5))),
    ])
    show("interval union", intervals)
    show("number of intervals", intervals.size())
    show("bounding interval", intervals.bounding_box())
    show("measure", intervals.measure())
    show("intersection", intersection(intervals, mask))
    show("join", join(intervals, mask))

    print("\nSee tutorial_set for concrete constrained sets and curves, "
          "and tutorial_paving for grids.")


if __name__ == "__main__":
    tutorial_geometry()
