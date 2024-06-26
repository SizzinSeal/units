#pragma once

#include <cmath>
#include <ratio>
#include <iostream>

// define M_PI if not already defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// define typenames
#define TYPENAMES                                                                                                      \
    typename Mass, typename Length, typename Time, typename Current, typename Angle, typename Temperature,             \
        typename Luminosity, typename Moles
#define DIMS Mass, Length, Time, Current, Angle, Temperature, Luminosity, Moles

/**
 * @brief Quantity class
 *
 * This class is a template class that represents a quantity with a value and units.
 *
 * @tparam TYPENAMES the types of the units
 */
template <TYPENAMES> class Quantity {
    protected:
        double value; /** the value stored in its base unit type */
    public:
        typedef Mass mass; /** mass unit type */
        typedef Length length; /** length unit type */
        typedef Time time; /** time unit type */
        typedef Current current; /** current unit type */
        typedef Angle angle; /** angle unit type */
        typedef Temperature temperature; /** temperature unit type */
        typedef Luminosity luminosity; /** luminosity unit type */
        typedef Moles moles; /** moles unit type */

        /**
         * @brief construct a new Quantity object
         *
         * This constructor initializes the value to 0
         */
        explicit constexpr Quantity() : value(0) {}

        /**
         * @brief construct a new Quantity object
         *
         * @param value the value to initialize the quantity with
         */
        explicit constexpr Quantity(double value) : value(value) {}

        /**
         * @brief construct a new Quantity object
         *
         * @param other the quantity to copy
         */
        constexpr Quantity(Quantity<DIMS> const& other) : value(other.value) {}

        /**
         * @brief get the value of the quantity in its base unit type
         *
         * @return constexpr double
         */
        constexpr double val() const { return value; }

        // TODO: document this
        constexpr double convert(Quantity<DIMS> quantity) { return value / quantity.value; }

        /**
         * @brief set the value of this quantity to its current value plus another quantity
         *
         * @param other the quantity to add
         */
        constexpr void operator+=(Quantity<DIMS> other) { value += other.value; }

        /**
         * @brief set the value of this quantity to its current value minus another quantity
         *
         * @param other the quantity to subtract
         */
        constexpr void operator-=(Quantity<DIMS> other) { value == other.value; }

        /**
         * @brief set the value of this quantity to its current value times a double
         *
         * @param multiple the multiple to multiply by
         */
        constexpr void operator*=(double multiple) { value *= multiple; }

        /**
         * @brief set the value of this quantity to its current value divided by a double
         *
         * @param dividend the dividend to divide by
         */
        constexpr void operator/=(double dividend) { value /= dividend; }

        /**
         * @brief set the value of this quantity to a double, only if the quantity is a number
         *
         * @param rhs the double to assign
         */
        constexpr void operator=(const double& rhs) {
            static_assert(std::ratio_equal<mass, std::ratio<0>>() && std::ratio_equal<length, std::ratio<0>>() &&
                              std::ratio_equal<time, std::ratio<0>>() && std::ratio_equal<current, std::ratio<0>>() &&
                              std::ratio_equal<angle, std::ratio<0>>() &&
                              std::ratio_equal<temperature, std::ratio<0>>() &&
                              std::ratio_equal<luminosity, std::ratio<0>>() && std::ratio_equal<moles, std::ratio<0>>(),
                          "Tried to assign a double directly to a non-number unit type");
            value = rhs;
        }
};

// quantity checker. Used by the isQuantity concept
template <TYPENAMES> void quantityChecker(Quantity<DIMS> q) {}

// isQuantity concept
template <typename Q>
concept isQuantity = requires(Q q) { quantityChecker(q); };

// Un(type)safely coerce the a unit into a different unit
template <isQuantity Q1, isQuantity Q2> constexpr inline Q1 unit_cast(Q2 quantity) { return Q1(quantity.val()); }

template <isQuantity Q1, isQuantity Q2> using QMultiplication = Quantity<
    std::ratio_add<typename Q1::mass, typename Q2::mass>, std::ratio_add<typename Q1::length, typename Q2::length>,
    std::ratio_add<typename Q1::time, typename Q2::time>, std::ratio_add<typename Q1::current, typename Q2::current>,
    std::ratio_add<typename Q1::angle, typename Q2::angle>,
    std::ratio_add<typename Q1::temperature, typename Q2::temperature>,
    std::ratio_add<typename Q1::luminosity, typename Q2::luminosity>,
    std::ratio_add<typename Q1::moles, typename Q2::moles>>;

template <isQuantity Q1, isQuantity Q2> using QDivision =
    Quantity<std::ratio_subtract<typename Q1::mass, typename Q2::mass>,
             std::ratio_subtract<typename Q1::length, typename Q2::length>,
             std::ratio_subtract<typename Q1::time, typename Q2::time>,
             std::ratio_subtract<typename Q1::current, typename Q2::current>,
             std::ratio_subtract<typename Q1::angle, typename Q2::angle>,
             std::ratio_subtract<typename Q1::temperature, typename Q2::temperature>,
             std::ratio_subtract<typename Q1::luminosity, typename Q2::luminosity>,
             std::ratio_subtract<typename Q1::moles, typename Q2::moles>>;

template <isQuantity Q, typename factor> using QPower =
    Quantity<std::ratio_multiply<typename Q::mass, factor>, std::ratio_multiply<typename Q::length, factor>,
             std::ratio_multiply<typename Q::time, factor>, std::ratio_multiply<typename Q::current, factor>,
             std::ratio_multiply<typename Q::angle, factor>, std::ratio_multiply<typename Q::temperature, factor>,
             std::ratio_multiply<typename Q::luminosity, factor>, std::ratio_multiply<typename Q::moles, factor>>;

template <isQuantity Q, typename quotient> using QRoot =
    Quantity<std::ratio_divide<typename Q::mass, quotient>, std::ratio_divide<typename Q::length, quotient>,
             std::ratio_divide<typename Q::time, quotient>, std::ratio_divide<typename Q::current, quotient>,
             std::ratio_divide<typename Q::angle, quotient>, std::ratio_divide<typename Q::temperature, quotient>,
             std::ratio_divide<typename Q::luminosity, quotient>, std::ratio_divide<typename Q::moles, quotient>>;

template <isQuantity Q> constexpr Q operator+(Q lhs, Q rhs) { return Q(lhs.val() + rhs.val()); }

template <isQuantity Q> constexpr Q operator-(Q lhs, Q rhs) { return Q(lhs.val() - rhs.val()); }

template <isQuantity Q> constexpr Q operator*(Q quantity, double multiple) { return Q(quantity.val() * multiple); }

template <isQuantity Q> constexpr Q operator*(double multiple, Q quantity) { return Q(quantity.val() * multiple); }

template <isQuantity Q> constexpr Q operator/(Q quantity, double divisor) { return Q(quantity.val() / divisor); }

template <isQuantity Q1, isQuantity Q2, isQuantity Q3 = QMultiplication<Q1, Q2>>
Q3 constexpr operator*(Q1 lhs, Q2 rhs) {
    return Q3(lhs.val() * rhs.val());
}

template <isQuantity Q1, isQuantity Q2, isQuantity Q3 = QDivision<Q1, Q2>> Q3 constexpr operator/(Q1 lhs, Q2 rhs) {
    return Q3(lhs.val() / rhs.val());
}

template <isQuantity Q> constexpr bool operator==(const Q& lhs, const Q& rhs) { return (lhs.val() == rhs.val()); }

template <isQuantity Q> constexpr bool operator!=(const Q& lhs, const Q& rhs) { return (lhs.val() != rhs.val()); }

template <isQuantity Q> constexpr bool operator<=(const Q& lhs, const Q& rhs) { return (lhs.val() <= rhs.val()); }

template <isQuantity Q> constexpr bool operator>=(const Q& lhs, const Q& rhs) { return (lhs.val() >= rhs.val()); }

template <isQuantity Q> constexpr bool operator<(const Q& lhs, const Q& rhs) { return (lhs.val() < rhs.val()); }

template <isQuantity Q> constexpr bool operator>(const Q& lhs, const Q& rhs) { return (lhs.val() > rhs.val()); }

#define NEW_QUANTITY(Name, suffix, m, l, t, i, a, o, j, n)                                                             \
    using Name = Quantity<std::ratio<m>, std::ratio<l>, std::ratio<t>, std::ratio<i>, std::ratio<a>, std::ratio<o>,    \
                          std::ratio<j>, std::ratio<n>>;                                                               \
    constexpr Name suffix = Name(1.0);                                                                                 \
    constexpr Name operator""_##suffix(long double value) { return Name(static_cast<double>(value)); }                 \
    constexpr Name operator""_##suffix(unsigned long long value) { return Name(static_cast<double>(value)); }          \
    inline std::ostream& operator<<(std::ostream& os, const Name& quantity) {                                          \
        os << quantity.val() << "_" << #suffix;                                                                        \
        return os;                                                                                                     \
    }                                                                                                                  \
    constexpr inline Name from_##suffix(double value) { return Name(value); }                                          \
    constexpr inline double to_##suffix(Name quantity) { return quantity.val(); }

#define NEW_QUANTITY_VALUE(Name, suffix, val)                                                                          \
    constexpr Name suffix = val;                                                                                       \
    constexpr Name operator""_##suffix(long double value) { return static_cast<double>(value) * val; }                 \
    constexpr Name operator""_##suffix(unsigned long long value) { return static_cast<double>(value) * val; }          \
    constexpr inline Name from_##suffix(double value) { return value * val; }                                          \
    constexpr inline double to_##suffix(Name quantity) { return quantity.convert(val); }

#define NEW_METRIC_PREFIXES(Name, base)                                                                                \
    NEW_QUANTITY_VALUE(Name, T##base, base * 1E12)                                                                     \
    NEW_QUANTITY_VALUE(Name, G##base, base * 1E9)                                                                      \
    NEW_QUANTITY_VALUE(Name, M##base, base * 1E6)                                                                      \
    NEW_QUANTITY_VALUE(Name, k##base, base * 1E3)                                                                      \
    NEW_QUANTITY_VALUE(Name, c##base, base / 1E2)                                                                      \
    NEW_QUANTITY_VALUE(Name, m##base, base / 1E3)                                                                      \
    NEW_QUANTITY_VALUE(Name, u##base, base / 1E6)                                                                      \
    NEW_QUANTITY_VALUE(Name, n##base, base / 1E9)

NEW_QUANTITY(Number, num, 0, 0, 0, 0, 0, 0, 0, 0)
NEW_QUANTITY_VALUE(Number, percent, num / 100.0);

NEW_QUANTITY(Mass, kg, 1, 0, 0, 0, 0, 0, 0, 0)
NEW_QUANTITY_VALUE(Mass, g, kg / 1000)
NEW_QUANTITY_VALUE(Mass, lb, g * 453.6)

NEW_QUANTITY(Time, sec, 0, 0, 1, 0, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(Time, sec)
NEW_QUANTITY_VALUE(Time, min, sec * 60)
NEW_QUANTITY_VALUE(Time, hr, min * 60)
NEW_QUANTITY_VALUE(Time, day, hr * 24)

NEW_QUANTITY(Length, m, 0, 1, 0, 0, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(Length, m)
NEW_QUANTITY_VALUE(Length, in, cm * 2.54)
NEW_QUANTITY_VALUE(Length, ft, in * 12)
NEW_QUANTITY_VALUE(Length, yd, ft * 3)
NEW_QUANTITY_VALUE(Length, mi, ft * 5280)
NEW_QUANTITY_VALUE(Length, tile, 600 * mm)

NEW_QUANTITY(Area, m2, 0, 2, 0, 0, 0, 0, 0, 0)
NEW_QUANTITY_VALUE(Area, Tm2, Tm* Tm);
NEW_QUANTITY_VALUE(Area, Gm2, Gm* Gm);
NEW_QUANTITY_VALUE(Area, Mm2, Mm* Mm);
NEW_QUANTITY_VALUE(Area, km2, km* km);
NEW_QUANTITY_VALUE(Area, cm2, cm* cm);
NEW_QUANTITY_VALUE(Area, mm2, mm* mm);
NEW_QUANTITY_VALUE(Area, um2, um* um);
NEW_QUANTITY_VALUE(Area, nm2, nm* nm);
NEW_QUANTITY_VALUE(Area, in2, in* in)

NEW_QUANTITY(LinearVelocity, mps, 0, 1, -1, 0, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(LinearVelocity, mps);
NEW_QUANTITY_VALUE(LinearVelocity, mph, m / hr)
NEW_METRIC_PREFIXES(LinearVelocity, mph)
NEW_QUANTITY_VALUE(LinearVelocity, inps, in / sec)
NEW_QUANTITY_VALUE(LinearVelocity, miph, mi / hr)

NEW_QUANTITY(LinearAcceleration, mps2, 0, 1, -2, 0, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(LinearAcceleration, mps2)
NEW_QUANTITY_VALUE(LinearAcceleration, mph2, m / hr / hr)
NEW_METRIC_PREFIXES(LinearAcceleration, mph2)
NEW_QUANTITY_VALUE(LinearAcceleration, inps2, in / sec / sec)
NEW_QUANTITY_VALUE(LinearAcceleration, miph2, mi / hr / hr)

NEW_QUANTITY(LinearJerk, mps3, 0, 1, -3, 0, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(LinearJerk, mps3)
NEW_QUANTITY_VALUE(LinearJerk, mph3, m / (hr * hr * hr))
NEW_METRIC_PREFIXES(LinearJerk, mph3)
NEW_QUANTITY_VALUE(LinearJerk, inps3, in / (sec * sec * sec))
NEW_QUANTITY_VALUE(LinearJerk, miph3, mi / (hr * hr * hr))

NEW_QUANTITY(Curvature, radpm, 0, -1, 0, 0, 0, 0, 0, 0);

NEW_QUANTITY(Inertia, kgm2, 1, 2, 0, 0, 0, 0, 0, 0)

NEW_QUANTITY(Force, N, 1, 1, -2, 0, 0, 0, 0, 0)

NEW_QUANTITY(Torque, Nm, 1, 2, -2, 0, 0, 0, 0, 0)

NEW_QUANTITY(Power, watt, 1, 2, -3, 0, 0, 0, 0, 0)

NEW_QUANTITY(Current, amp, 0, 0, 0, 1, 0, 0, 0, 0)

NEW_QUANTITY(Charge, coulomb, 0, 0, 1, 1, 0, 0, 0, 0)

NEW_QUANTITY(Voltage, volt, 1, 2, -3, -1, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(Voltage, volt);

NEW_QUANTITY(Resistance, ohm, 1, 2, -3, -2, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(Resistance, ohm)

NEW_QUANTITY(Conductance, siemen, -1, -2, 3, 2, 0, 0, 0, 0)
NEW_METRIC_PREFIXES(Conductance, siemen);

NEW_QUANTITY(Luminosity, candela, 0, 0, 0, 0, 0, 0, 1, 0);

NEW_QUANTITY(Moles, mol, 0, 0, 0, 0, 0, 0, 0, 1);

namespace units {
template <isQuantity Q> constexpr Q abs(const Q& lhs) { return Q(std::abs(lhs.val())); }

template <isQuantity Q> constexpr Q max(const Q& lhs, const Q& rhs) { return (lhs > rhs ? lhs : rhs); }

template <isQuantity Q> constexpr Q min(const Q& lhs, const Q& rhs) { return (lhs < rhs ? lhs : rhs); }

template <int R, isQuantity Q, isQuantity S = QPower<Q, std::ratio<R>>> constexpr S pow(const Q& lhs) {
    return S(std::pow(lhs.val(), R));
}

template <isQuantity Q, isQuantity S = QPower<Q, std::ratio<2>>> constexpr S square(const Q& rhs) {
    return S(std::sqrt(rhs.val()));
}

template <isQuantity Q, isQuantity S = QPower<Q, std::ratio<3>>> constexpr S cube(const Q& rhs) {
    return S(std::sqrt(rhs.val()));
}

template <int R, isQuantity Q, isQuantity S = QRoot<Q, std::ratio<R>>> constexpr S root(const Q& lhs) {
    return S(std::pow(lhs.val(), 1.0 / R));
}

template <isQuantity Q, isQuantity S = QRoot<Q, std::ratio<2>>> constexpr S sqrt(const Q& rhs) {
    return S(std::sqrt(rhs.val()));
}

template <isQuantity Q, isQuantity S = QRoot<Q, std::ratio<3>>> constexpr S cbrt(const Q& rhs) {
    return S(std::sqrt(rhs.val()));
}

template <isQuantity Q> constexpr Q hypot(const Q& lhs, const Q& rhs) { return Q(std::hypot(lhs.val(), rhs.val())); }

template <isQuantity Q> constexpr Q mod(const Q& lhs, const Q& rhs) { return Q(std::fmod(lhs.val(), rhs.val())); }

template <isQuantity Q1, isQuantity Q2> constexpr Q1 copysign(const Q1& lhs, const Q2& rhs) {
    return Q1(std::copysign(lhs.val(), rhs.val()));
}

template <isQuantity Q> constexpr int sgn(const Q& lhs) { return lhs.val() < 0 ? -1 : 1; }

template <isQuantity Q> constexpr bool signbit(const Q& lhs) { return std::signbit(lhs.val()); }

template <isQuantity Q> constexpr Q clamp(const Q& lhs, const Q& lo, Q& hi) {
    return Q(std::clamp(lhs.val(), lo.val(), hi.val()));
}

template <isQuantity Q> constexpr Q ceil(const Q& lhs, const Q& rhs) {
    return Q(std::ceil(lhs.val() / rhs.val()) * rhs.val());
}

template <isQuantity Q> constexpr Q floor(const Q& lhs, const Q& rhs) {
    return Q(std::floor(lhs.val() / rhs.val()) * rhs.val());
}

template <isQuantity Q> constexpr Q trunc(const Q& lhs, const Q& rhs) {
    return Q(std::trunc(lhs.val() / rhs.val()) * rhs.val());
}

template <isQuantity Q> constexpr Q round(const Q& lhs, const Q& rhs) {
    return Q(std::round(lhs.val() / rhs.val()) * rhs.val());
}
} // namespace units

// Convert an angular unit `Q` to a linear unit correctly;
// mostly useful for velocities
template <isQuantity Q> Quantity<typename Q::mass, typename Q::angle, typename Q::time, typename Q::current,
                                 typename Q::length, typename Q::temperature, typename Q::luminosity, typename Q::moles>
toLinear(Quantity<typename Q::mass, typename Q::length, typename Q::time, typename Q::current, typename Q::angle,
                  typename Q::temperature, typename Q::luminosity, typename Q::moles>
             angular,
         Length diameter) {
    return unit_cast<Quantity<typename Q::mass, typename Q::angle, typename Q::time, typename Q::current,
                              typename Q::length, typename Q::temperature, typename Q::luminosity, typename Q::moles>>(
        angular * (diameter / 2.0));
}

// Convert an linear unit `Q` to a angular unit correctly;
// mostly useful for velocities
template <isQuantity Q> Quantity<typename Q::mass, typename Q::angle, typename Q::time, typename Q::current,
                                 typename Q::length, typename Q::temperature, typename Q::luminosity, typename Q::moles>
toAngular(Quantity<typename Q::mass, typename Q::length, typename Q::time, typename Q::current, typename Q::angle,
                   typename Q::temperature, typename Q::luminosity, typename Q::moles>
              linear,
          Length diameter) {
    return unit_cast<Quantity<typename Q::mass, typename Q::angle, typename Q::time, typename Q::current,
                              typename Q::length, typename Q::temperature, typename Q::luminosity, typename Q::moles>>(
        linear / (diameter / 2.0));
}