/***************************************************************************
 *            geometry_submodule.cpp
 *
 *  Copyright  2008-20  Pieter Collins
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

#include "pybind11.hpp"
#include "algebra-utilities.hpp"
#include "numeric_submodule.hpp"
#include "interval-utilities.hpp"
#include "geometry/geometry.hpp"
#include "geometry/point.hpp"

#include "geometry/box.hpp"
#include "geometry/union_of_intervals.hpp"




namespace Ariadne {


template<class F> OutputStream& operator<<(OutputStream& os, const PythonRepresentation<Bounds<F>>& x);

template<> struct PythonTemplateName<Point> { static std::string get() { return "Point"; } };
template<> struct PythonTemplateName<Box> { static std::string get() { return "Box"; } };

template<class X> struct PythonClassName<Point<X>> {
    std::string get() const { return python_template_class_name<X>("Point"); } };
template<class X> struct PythonClassName<Box<X>> {
    std::string get() const { return python_template_class_name<X>("Box"); } };

template<class UB> struct PythonClassName<Box<Interval<UB>>> {
    std::string get() const { return python_class_name<UB>()+"Box"; } };
template<> struct PythonClassName<Box<Interval<FloatDP>>> {
    std::string get() const { return "FloatDPExactBox"; } };
template<> struct PythonClassName<Box<Interval<FloatDPUpperBound>>> {
    std::string get() const { return "FloatDPUpperBox"; } };
template<> struct PythonClassName<Box<Interval<FloatDPLowerBound>>> {
    std::string get() const { return "FloatDPLowerBox"; } };
template<> struct PythonClassName<Box<Interval<FloatDPApproximation>>> {
    std::string get() const { return "FloatDPApproximateBox"; } };


template<class UB> OutputStream& operator<<(OutputStream& os, const PythonRepresentation<Box<UB>>& repr) {
    Box<UB> const& bx=repr.reference();
    os << python_class_name<Box<UB>>() << "([";
    for(SizeType i=0; i!=bx.dimension(); ++i) {
        if(i!=0) { os << ','; }
        os << python_literal(bx[i]);
    }
    os << "])";
    return os;
}

template<class P, class T> class OpenSetWrapper;
template<class P, class T> class ClosedSetWrapper;
template<class P, class T> class OvertSetWrapper;
template<class P, class T> class BoundedSetWrapper;
template<class P, class T> class CompactSetWrapper;
template<class P, class T> class RegularSetWrapper;
template<class P, class T> class LocatedSetWrapper;

template<class T> class OpenSetWrapper<EffectiveTag,T>
  : public pybind11::wrapper<OpenSetInterface<EffectiveTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    OpenSetInterface<EffectiveTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    LowerKleenean covers(const BasicSetType& r) const { return this->get_override("covers")(r); }
    LowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};

template<class T> class ClosedSetWrapper<EffectiveTag,T>
  : public pybind11::wrapper<ClosedSetInterface<EffectiveTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    ClosedSetInterface<EffectiveTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    LowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};


template<class T> class OvertSetWrapper<EffectiveTag,T>
  : public pybind11::wrapper<OvertSetInterface<EffectiveTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    OvertSetInterface<EffectiveTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    LowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};


template<class T> class BoundedSetWrapper<EffectiveTag,T>
  : public pybind11::wrapper<BoundedSetInterface<EffectiveTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    typedef typename SetTraits<T>::BoundingSetType BoundingSetType;
    BoundedSetInterface<EffectiveTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    LowerKleenean inside(const BasicSetType& r) const { return this->get_override("inside")(r); }
    BoundingSetType bounding_box() const { return this->get_override("bounding_box")(); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};

template<class T> class CompactSetWrapper<EffectiveTag,T>
  : public pybind11::wrapper<CompactSetInterface<EffectiveTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    typedef typename SetTraits<T>::BoundingSetType BoundingSetType;
    CompactSetInterface<EffectiveTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    LowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    LowerKleenean inside(const BasicSetType& r) const { return this->get_override("inside")(r); }
    LowerKleenean is_bounded() const { return this->get_override("is_bounded")(); }
    BoundingSetType bounding_box() const { return this->get_override("bounding_box")(); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};

template<class T> class RegularSetWrapper<EffectiveTag,T>
  : public pybind11::wrapper<RegularSetInterface<EffectiveTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    RegularSetInterface<EffectiveTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    LowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    LowerKleenean covers(const BasicSetType& r) const { return this->get_override("covers")(r); }
    LowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};

template<class T> class LocatedSetWrapper<EffectiveTag,T>
  : public pybind11::wrapper<LocatedSetInterface<EffectiveTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    typedef typename SetTraits<T>::BoundingSetType BoundingSetType;
    LocatedSetInterface<EffectiveTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    LowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    LowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    LowerKleenean inside(const BasicSetType& r) const { return this->get_override("inside")(r); }
    LowerKleenean is_bounded() const { return this->get_override("is_bounded")(); }
    BoundingSetType bounding_box() const { return this->get_override("bounding_box")(); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};




template<class T> class OpenSetWrapper<ValidatedTag,T>
  : public pybind11::wrapper<OpenSetInterface<ValidatedTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    OpenSetInterface<ValidatedTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    ValidatedLowerKleenean covers(const BasicSetType& r) const { return this->get_override("covers")(r); }
    ValidatedLowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};

template<class T> class ClosedSetWrapper<ValidatedTag,T>
  : public pybind11::wrapper<ClosedSetInterface<ValidatedTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    ClosedSetInterface<ValidatedTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    ValidatedLowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};


template<class T> class OvertSetWrapper<ValidatedTag,T>
  : public pybind11::wrapper<OvertSetInterface<ValidatedTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    OvertSetInterface<ValidatedTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    ValidatedLowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};


template<class T> class BoundedSetWrapper<ValidatedTag,T>
  : public pybind11::wrapper<BoundedSetInterface<ValidatedTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    typedef typename SetTraits<T>::BoundingSetType BoundingSetType;
    BoundedSetInterface<ValidatedTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    ValidatedLowerKleenean inside(const BasicSetType& r) const { return this->get_override("inside")(r); }
    BoundingSetType bounding_box() const { return this->get_override("bounding_box")(); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};


template<class T> class CompactSetWrapper<ValidatedTag,T>
  : public pybind11::wrapper<CompactSetInterface<ValidatedTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    typedef typename SetTraits<T>::BoundingSetType BoundingSetType;
    CompactSetInterface<ValidatedTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    ValidatedLowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    ValidatedLowerKleenean inside(const BasicSetType& r) const { return this->get_override("inside")(r); }
    ValidatedLowerKleenean is_bounded() const { return this->get_override("is_bounded")(); }
    BoundingSetType bounding_box() const { return this->get_override("bounding_box")(); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};

template<class T> class RegularSetWrapper<ValidatedTag,T>
  : public pybind11::wrapper<RegularSetInterface<ValidatedTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    RegularSetInterface<ValidatedTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    ValidatedLowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    ValidatedLowerKleenean covers(const BasicSetType& r) const { return this->get_override("covers")(r); }
    ValidatedLowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};

template<class T> class LocatedSetWrapper<ValidatedTag,T>
  : public pybind11::wrapper<LocatedSetInterface<ValidatedTag,T>>
{
  public:
    typedef typename SetTraits<T>::BasicSetType BasicSetType;
    typedef typename SetTraits<T>::BoundingSetType BoundingSetType;
    LocatedSetInterface<ValidatedTag,T>* clone() const { return this->get_override("clone")(); }
    SizeType dimension() const { return this->get_override("dimension")(); }
    ValidatedLowerKleenean overlaps(const BasicSetType& r) const { return this->get_override("overlaps")(r); }
    ValidatedLowerKleenean separated(const BasicSetType& r) const { return this->get_override("separated")(r); }
    ValidatedLowerKleenean inside(const BasicSetType& r) const { return this->get_override("inside")(r); }
    ValidatedLowerKleenean is_bounded() const { return this->get_override("is_bounded")(); }
    BoundingSetType bounding_box() const { return this->get_override("bounding_box")(); }
    OutputStream& _write(OutputStream& os) const { return this->get_override("_write")(os); }
};


} // namespace Ariadne


using namespace Ariadne;

template<class BX> BX box_from_list(pybind11::list lst) {
    typedef typename BX::IntervalType IVL;
    Array<IVL> ary( lst.size(), [&lst](SizeType i){return pybind11::cast<IVL>(lst[i]);} );
    return BX(ary);
}



template<class PT> PT point_from_python(pybind11::list pylst) {
    typedef typename PT::ValueType X;
    std::vector<X> lst=pybind11::cast<std::vector<X>>(pylst);
    Array<X> ary(lst.begin(),lst.end());
    return PT(Vector<X>(ary));
}

template<class PT> Void export_point(pybind11::module& module, std::string name=python_class_name<PT>())
{
    typedef typename PT::ValueType X;
    pybind11::class_<PT> point_class(module,name.c_str());
    point_class.def(pybind11::init(&point_from_python<PT>));
    point_class.def(pybind11::init<PT>());
    if constexpr (DefaultConstructible<X>) {
        point_class.def(pybind11::init<Nat>());
    }
    if constexpr (HasPrecisionType<X>) {
        typedef typename X::PrecisionType PR;
        point_class.def(pybind11::init<Nat,PR>());
    }
    point_class.def("__getitem__", &__getitem__<PT,Int,X>);
    point_class.def("dimension", &PT::dimension);
    point_class.def("centre", &PT::centre);
    point_class.def("bounding_box", &PT::bounding_box);
    point_class.def("__str__", &__cstr__<PT>);
}

Void export_points(pybind11::module& module) {
    export_point<RealPoint>(module);
    export_point<FloatDPPoint>(module);
    export_point<FloatDPBoundsPoint>(module);
    export_point<FloatDPApproximationPoint>(module);

    template_<Point> point_template(module);
    point_template.instantiate<Real>();
    point_template.instantiate<FloatDP>();
    point_template.instantiate<FloatDPBounds>();
    point_template.instantiate<FloatDPApproximation>();
}

Void export_geometry_vectors(pybind11::module& module) {
    using RadiusType = Positive<FloatDPBounds>;
    using RadiusVector = Vector<RadiusType>;

    pybind11::class_<RadiusVector> radius_vector_class(module,"FloatDPPositiveBoundsVector");
    radius_vector_class.def("size", &RadiusVector::size);
    radius_vector_class.def("__len__", &RadiusVector::size);
    radius_vector_class.def("__getitem__", [](RadiusVector const& v, SizeType i) {
        if(i>=v.size()) { throw pybind11::index_error(); }
        return v[i];
    });
    radius_vector_class.def("__str__", &__cstr__<RadiusVector>);
}

template<class UB> using BoxWithUpperBound=Box<Interval<UB>>;

template<class BX> Void export_box(pybind11::module& module, std::string name=python_class_name<BX>())
{
    using IVL=typename BX::IntervalType;

    using BoxType = BX;
    using IntervalType = IVL;

    typedef typename BoxType::MidpointType MidpointType;

    typedef decltype(contains(declval<BoxType>(),declval<MidpointType>())) ContainsType;
    typedef decltype(disjoint(declval<BX>(),declval<BX>())) DisjointType;
    typedef decltype(subset(declval<BX>(),declval<BX>())) SubsetType;
    typedef decltype(separated(declval<BX>(),declval<BX>())) SeparatedType;
    typedef decltype(overlap(declval<BX>(),declval<BX>())) OverlapType;
    typedef decltype(covers(declval<BX>(),declval<BX>())) CoversType;
    typedef decltype(inside(declval<BX>(),declval<BX>())) InsideType;

    //NOTE: Boxes do not inherit SetInterface<T>s or  in C++ API
    //pybind11::class_<BasicSetType,pybind11::bases<CompactSetInterface<T>,OpenSetInterface<T>>>
    pybind11::class_<BoxType> box_class(module,name.c_str());
    box_class.def(pybind11::init<BoxType>());
    box_class.def(pybind11::init<DimensionType>());

    define_conversion<BoxDomainType>(box_class);
    define_conversion<DyadicBox>(box_class);
    define_conversion<DecimalBox>(box_class);
    define_conversion<RationalBox>(box_class);
    define_conversion<RealBox>(box_class);

    if constexpr (HasPrecisionType<IntervalType>) {
        typedef PrecisionType<IntervalType> PrecisionType;
        if constexpr (Constructible<BoxType,RealBox,PrecisionType>) {
            box_class.def(pybind11::init<RealBox,PrecisionType>());
        } else if constexpr (Constructible<BoxType,DyadicBox,PrecisionType>) {
            box_class.def(pybind11::init<DyadicBox,PrecisionType>());
        }

        typedef typename IntervalType::UpperBoundType FloatType;
        export_conversions<BoxWithUpperBound,FloatType>(box_class);
    }

    box_class.def(pybind11::init<Array<IntervalType>>());
    box_class.def(pybind11::init(&box_from_list<BoxType>));
    pybind11::implicitly_convertible<pybind11::list,BoxType>();

    if constexpr (HasEquality<BX,BX>) {
        box_class.def("__eq__",  __eq__<BX,BX , Return<EqualityType<BX,BX>> >);
        box_class.def("__ne__",  __ne__<BX,BX , Return<InequalityType<BX,BX>> >);
    }

    box_class.def("dimension", (DimensionType(BX::*)()const) &BX::dimension);
    box_class.def("__getitem__", &__getitem__<BX,Int>);
    box_class.def("centre", (typename BX::CentreType(BX::*)()const) &BX::centre);
    box_class.def("midpoint", (typename BX::MidpointType(BX::*)()const) &BX::midpoint);
    box_class.def("radius", (typename BX::RadiusType(BX::*)()const) &BX::radius);
    box_class.def("radii", (Vector<typename BX::RadiusType>(BX::*)()const) &BX::radii);
    box_class.def("widths", (Vector<typename BX::RadiusType>(BX::*)()const) &BX::widths);
    box_class.def("semiperimeter", (typename BX::RadiusType(BX::*)()const) &BX::semiperimeter);
    box_class.def("bounding_box", (BX(BX::*)()const) &BX::bounding_box);
    box_class.def("measure", (typename BX::MeasureType(BX::*)()const) &BX::measure);
    box_class.def("volume", (typename BX::MeasureType(BX::*)()const) &BX::volume);
    box_class.def("separated", (SeparatedType(BX::*)(const BX&)const) &BX::separated);
    box_class.def("overlaps", (OverlapType(BX::*)(const BX&)const) &BX::overlaps);
    box_class.def("covers", (CoversType(BX::*)(const BX&)const) &BX::covers);
    box_class.def("inside", (InsideType(BX::*)(const BX&)const) &BX::inside);
    box_class.def("is_empty", (SeparatedType(BX::*)()const) &BX::is_empty);
    box_class.def("split", (Pair<BX,BX>(BX::*)()const) &BX::split);
    box_class.def("split", (Pair<BX,BX>(BX::*)(SizeType)const) &BX::split);
    box_class.def("__str__",&__cstr__<BX>);
    box_class.def("__repr__",&__repr__<BX>);

    module.def("centre", (typename BX::CentreType(*)(BX const&)) &centre);
    module.def("midpoint", (typename BX::MidpointType(*)(BX const&)) &midpoint);
    module.def("radius", (typename BX::RadiusType(*)(BX const&)) &radius);
    module.def("radii", (Vector<typename BX::RadiusType>(*)(BX const&)) &radii);
    module.def("widths", (Vector<typename BX::RadiusType>(*)(BX const&)) &widths);
    module.def("measure", (typename BX::MeasureType(*)(BX const&)) &measure);
    module.def("volume", (typename BX::MeasureType(*)(BX const&)) &volume);

    box_class.def("contains", (ContainsType(BX::*)(MidpointType const&)const) &BX::contains);
    module.def("contains", (ContainsType(*)(BX const&,MidpointType const&)) &contains);
    if constexpr (HasPrecisionType<typename IntervalType::UpperBoundType>) {
        typedef PrecisionType<typename IntervalType::UpperBoundType> PrecisionType;
        if constexpr (Same<typename IntervalType::UpperBoundType,FloatUpperBound<PrecisionType>> and false) {
            box_class.def("contains", (ContainsType(*)(BoxType const&, Vector<FloatBounds<PrecisionType>> const&)) &contains);
            module.def("contains", (ContainsType(*)(BoxType const&, Vector<FloatBounds<PrecisionType>> const&)) &contains);
        }
    }

    module.def("disjoint", (DisjointType(*)(BX const&,BX const&)) &disjoint);
    if constexpr (Same<typename IVL::UpperBoundType, typename IVL::LowerBoundType>) {
        module.def("subset", (SubsetType(*)(const BX&,const BX&)) &subset);
    } else {
        typedef Box<Interval<typename IVL::LowerBoundType>> LBX;
        using SubsetOfLowerType = decltype(subset(declval<BX>(),declval<LBX>()));
        module.def("subset", (SubsetOfLowerType(*)(const BX&,const LBX&)) &subset);
    }

    module.def("product", (BX(*)(const BX&,const IVL&)) &product);
    module.def("product", (BX(*)(const BX&,const BX&)) &product);
    module.def("hull", (BX(*)(const BX&,const BX&)) &hull);
    module.def("intersection", (BX(*)(const BX&,const BX&)) &intersection);
    module.def("split", (Pair<BX,BX>(*)(BX const&)) &split);

    if constexpr (Same<BX,BoxDomainType>) {
        module.attr("BoxDomainType")=box_class;
    } else if constexpr (Same<BX,BoxValidatedRangeType>) {
        module.attr("BoxValidatedRangeType")=box_class;
    } else if constexpr (Same<BX,BoxApproximateRangeType>) {
        module.attr("BoxApproximateRangeType")=box_class;
    }

    module.def("cast_exact",(BoxDomainType(*)(BoxApproximateRangeType const&)) &cast_exact_box);

}


template<class BX> Void export_simple_box(pybind11::module& module, std::string name=python_class_name<BX>())
{
    using BoxType = BX;
    pybind11::class_<BoxType> box_class(module,name.c_str());
    box_class.def(pybind11::init(&box_from_list<BoxType>));
    if constexpr (Constructible<BX,DyadicBox>) {
        box_class.def(pybind11::init<DyadicBox>());
    }
    if constexpr (Constructible<BX,DecimalBox>) {
        box_class.def(pybind11::init<DecimalBox>());
    }
    if constexpr (Constructible<BX,RationalBox>) {
        box_class.def(pybind11::init<RationalBox>());
    }
    if constexpr (Constructible<BoxType,BoxDomainType>) {
        box_class.def(pybind11::init<BoxDomainType>());
        pybind11::implicitly_convertible<BoxDomainType,BoxType>();
    }
    box_class.def("dimension", (DimensionType(BX::*)()const) &BX::dimension);
    box_class.def("__getitem__", &__getitem__<BX,Int>);
    box_class.def("__str__",&__cstr__<BoxType>);
    box_class.def("__repr__",&__repr__<BoxType>);
}

Void export_boxes(pybind11::module& module) {
    export_box<RealBox>(module);
    export_simple_box<RationalBox>(module);
    export_simple_box<DecimalBox>(module);
    export_simple_box<DyadicBox>(module);
//    export_box<ExactBoxType>(module,"ExactBoxType");
//    export_box<UpperBoxType>(module,"UpperBoxType");
//    export_box<ApproximateBoxType>(module,"ApproximateBoxType");
    pybind11::implicitly_convertible<DyadicBox,DecimalBox>();
    pybind11::implicitly_convertible<DyadicBox,RationalBox>();
    pybind11::implicitly_convertible<DyadicBox,RealBox>();
    pybind11::implicitly_convertible<DecimalBox,RationalBox>();
    pybind11::implicitly_convertible<DecimalBox,RealBox>();
    pybind11::implicitly_convertible<RationalBox,RealBox>();

    export_box<FloatDPExactBox>(module);
    export_box<FloatDPUpperBox>(module);
    export_box<FloatDPLowerBox>(module);
    export_box<FloatDPApproximateBox>(module);
//    export_box<FloatMPUpperBox>(module);

    pybind11::implicitly_convertible<FloatDPExactBox,FloatDPUpperBox>();
    pybind11::implicitly_convertible<FloatDPExactBox,FloatDPLowerBox>();
    pybind11::implicitly_convertible<FloatDPExactBox,FloatDPApproximateBox>();
    pybind11::implicitly_convertible<FloatDPUpperBox,FloatDPApproximateBox>();
    pybind11::implicitly_convertible<FloatDPLowerBox,FloatDPApproximateBox>();

    module.def("widen", (FloatDPUpperBox(*)(FloatDPExactBox const&, FloatDP eps)) &widen);

    module.def("cast_singleton", (Vector<FloatDPBounds>(*)(Box<Interval<FloatDPUpperBound>> const&)) &cast_singleton);
    module.def("cast_singleton", (Vector<FloatMPBounds>(*)(Box<Interval<FloatMPUpperBound>> const&)) &cast_singleton);

    template_<Box> box_template(module);
    // TODO: Change templates so that
    box_template.as_instantiate<DyadicBox,Dyadic>();
    box_template.as_instantiate<DecimalBox,Decimal>();
    box_template.as_instantiate<RationalBox,Rational>();
    box_template.as_instantiate<RealBox,Real>();
    box_template.as_instantiate<FloatDPExactBox,FloatDP>();
    box_template.as_instantiate<FloatDPUpperBox,FloatDPUpperBound>();
    box_template.as_instantiate<FloatDPLowerBox,FloatDPLowerBound>();
    box_template.as_instantiate<FloatDPApproximateBox,FloatDPApproximation>();

}


void export_geometry_primitives(pybind11::module& module) {
    // The abstract Set interfaces remain owned by geometry, even though their
    // concrete set implementations are bound in set_submodule.cpp.
    pybind11::class_<EuclideanSpace> space_class(module, "EuclideanSpace");
    space_class.def(pybind11::init<SizeType>());
    space_class.def("dimension", &EuclideanSpace::dimension);

    using IntervalUnion = UnionOfIntervals<Dyadic>;
    pybind11::class_<IntervalUnion> union_class(module, "DyadicUnionOfIntervals");
    union_class.def(pybind11::init([](const std::vector<DyadicInterval>& intervals) {
        List<DyadicInterval> values;
        for (const auto& interval : intervals) { values.append(interval); }
        return IntervalUnion(values);
    }));
    // A scalar interval union has dimension one, but its C++ return type
    // SizeOne has no Python type caster. Expose the same value as SizeType.
    union_class.def("dimension", [](const IntervalUnion&) -> SizeType { return 1u; });
    union_class.def("size", &IntervalUnion::size);
    union_class.def("__len__", &IntervalUnion::size);
    union_class.def("bounding_box", &IntervalUnion::bounding_box);
    union_class.def("measure", &IntervalUnion::measure);
    union_class.def("overlaps", &IntervalUnion::overlaps);
    union_class.def("covers", &IntervalUnion::covers);
    union_class.def("inside", &IntervalUnion::inside);
    union_class.def("separated", &IntervalUnion::separated);
    union_class.def("__str__", &__cstr__<IntervalUnion>);
    module.def("intersection", [](const IntervalUnion& lhs, const IntervalUnion& rhs) {
        return intersection(lhs, rhs);
    });
    module.def("join", [](const IntervalUnion& lhs, const IntervalUnion& rhs) {
        return join(lhs, rhs);
    });
}

void geometry_submodule(pybind11::module& module) {
    export_geometry_primitives(module);
    export_points(module);
    export_geometry_vectors(module);
    export_boxes(module);
}
