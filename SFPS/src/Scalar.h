#ifndef SCALAR_H_INCLUDED
#define SCALAR_H_INCLUDED

#include <cassert>
#include <ostream>
#include <type_traits>

template< typename Quantity, typename Unit = long double >
struct Scalar
{
    typedef Quantity quantity_t;
    typedef Unit unit_t;

    unit_t value;

    Scalar()
        :   value(0)
    {
    }

    Scalar(unit_t s)
        :   value(s)
    {
    }

    Scalar(const Scalar< Quantity, Unit >& rhs)
        :   value(rhs.value)
    {
    }

    Scalar< Quantity, Unit >& operator=(const Scalar< Quantity, Unit >& rhs) { value = rhs.value; return *this; }
    Scalar< Quantity, Unit >& operator+(const Scalar< Quantity, Unit >& rhs) { value += rhs.value; return *this; }
    Scalar< Quantity, Unit >& operator-(const Scalar< Quantity, Unit >& rhs) { value -= rhs.value; return *this; }
    Scalar< Quantity, Unit >& operator*(const Scalar< Quantity, Unit >& rhs) { value *= rhs.value; return *this; }
    //Scalar< Quantity, Unit >& operator/(const Scalar< Quantity, Unit >& rhs) { value /= rhs.value; return *this; }

    template< typename T1 >
    static Scalar< Quantity, Unit > from(const T1& param1);

    template< typename T1, typename T2 >
    static Scalar< Quantity, Unit > from(const T1& param1, const T2& param2);

    template< typename T1, typename T2, typename T3 >
    static Scalar< Quantity, Unit > from(const T1& param1, const T2& param2, const T3& param3);
};

template< typename Q, typename U >
std::ostream& operator<<(std::ostream& os, Scalar< Q, U > s)
{
    os << s.value;
    return os;
}

template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator+(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value + rhs.value;
    return result;
}

template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator-(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value - rhs.value;
    return result;
}

template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator*(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value * rhs.value;
    return result;
}

template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator/(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value / rhs.value;
    return result;
}

template<typename Q, typename U>
Scalar<Q, U> operator*(const Scalar<Q, U>& s, double d)
{
    return Scalar<Q, U>(s.value * d);
}
template<typename Q, typename U>
Scalar<Q, U> operator*(double d, const Scalar<Q, U>& s)
{
    return Scalar<Q, U>(s.value * d);
}
template<typename Q, typename U>
Scalar<Q, U> operator/(const Scalar<Q, U>& s, double d)
{
    return Scalar<Q, U>(s.value / d);
}

template<typename Q, typename U>
Scalar<Q, U> operator-(const Scalar<Q, U>& s)
{
    return Scalar<Q, U>(-s.value);
}

#endif // SCALAR_H_INCLUDED
