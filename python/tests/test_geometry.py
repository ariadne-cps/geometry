from pyariadne import *


def test_geometry():
    dp = DoublePrecision()
    x = FloatDP(1, dp)
    box = FloatDPExactBox([(-x, x), (-x, x)])

    assert Box[FloatDP] == FloatDPExactBox
    assert box.dimension() == 2

    left, right = box.split(0)
    assert left.dimension() == 2
    assert right.dimension() == 2

def test_point_without_rendering_base():
    dp = DoublePrecision()
    point = Point[FloatDP]([FloatDP(1, dp), FloatDP(2, dp)])
    assert point.dimension() == 2
    # Kernel must not require or inherit any class from the Drawing layer.
    assert all("Drawable" not in base.__name__ for base in type(point).__mro__)


def test_geometry_tutorial_primitives():
    assert EuclideanSpace(3).dimension() == 3
    pieces = DyadicUnionOfIntervals([
        DyadicInterval(Dyadic(0), Dyadic(1)),
        DyadicInterval(Dyadic(2), Dyadic(3)),
    ])
    mask = DyadicUnionOfIntervals([
        DyadicInterval(Dyadic(exact(0.5)), Dyadic(exact(2.5)))
    ])
    assert pieces.dimension() == 1
    assert pieces.size() == 2
    assert intersection(pieces, mask).size() == 2
    assert join(pieces, mask).size() == 1
