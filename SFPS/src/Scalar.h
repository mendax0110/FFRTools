#ifndef SCALAR_H_INCLUDED
#define SCALAR_H_INCLUDED

#include <cassert>

/**
 * @brief Scalar Structure. \struct Scalar
 * @tparam Quantity The physical quantity type.
 * @tparam Unit The unit type for the scalar value.
 */
template< typename Quantity, typename Unit = long double >
struct Scalar
{
    typedef Quantity quantity_t;
    typedef Unit unit_t;

    unit_t value;

    /**
     * @brief Default constructor for Scalar.
     */
    Scalar()
        :   value(0)
    {
    }

    /**
     * @brief Parameterized constructor for Scalar.
     * @param s The scalar value.
     */
    Scalar(unit_t s)
        :   value(s)
    {
    }

    /**
     * @brief Copy constructor for Scalar.
     * @param rhs The scalar to copy from.
     */
    Scalar(const Scalar< Quantity, Unit >& rhs)
        :   value(rhs.value)
    {
    }

    /**
     * @brief Assignment operator for Scalar.
     * @param rhs The scalar to assign from.
     * @return Reference to the assigned Scalar.
     */
    Scalar< Quantity, Unit >& operator=(const Scalar< Quantity, Unit >& rhs) { value = rhs.value; return *this; }

    /**
     * @brief Addition assignment operator for Scalar.
     * @param rhs The scalar to add.
     * @return Reference to the updated Scalar.
     */
    Scalar< Quantity, Unit >& operator+(const Scalar< Quantity, Unit >& rhs) { value += rhs.value; return *this; }

    /**
     * @brief Subtraction assignment operator for Scalar.
     * @param rhs The scalar to subtract.
     * @return Reference to the updated Scalar.
     */
    Scalar< Quantity, Unit >& operator-(const Scalar< Quantity, Unit >& rhs) { value -= rhs.value; return *this; }

    /**
     * @brief Multiplication assignment operator for Scalar.
     * @param rhs The scalar to multiply.
     * @return Reference to the updated Scalar.
     */
    Scalar< Quantity, Unit >& operator*(const Scalar< Quantity, Unit >& rhs) { value *= rhs.value; return *this; }

    /**
     * @brief Division assignment operator for Scalar.
     * @param rhs The scalar to divide.
     * @return Reference to the updated Scalar.
     */
    Scalar< Quantity, Unit >& operator/(const Scalar< Quantity, Unit >& rhs) { value /= rhs.value; return *this; }

    /**
     * @brief Static factory method to create Scalar from one parameter.
     * @tparam T1 The type of the parameter.
     * @param param1 The parameter value.
     * @return A Scalar instance.
     */
    template< typename T1 >
    static Scalar< Quantity, Unit > from(const T1& param1);

    /**
     * @brief Static factory method to create Scalar from two parameters.
     * @tparam T1 The type of the first parameter.
     * @tparam T2 The type of the second parameter.
     * @param param1 The first parameter value.
     * @param param2 The second parameter value.
     * @return A Scalar instance.
     */
    template< typename T1, typename T2 >
    static Scalar< Quantity, Unit > from(const T1& param1, const T2& param2);

    /**
     * @brief Static factory method to create Scalar from three parameters.
     * @tparam T1 The type of the first parameter.
     * @tparam T2 The type of the second parameter.
     * @tparam T3 The type of the third parameter.
     * @param param1 The first parameter value.
     * @param param2 The second parameter value.
     * @param param3 The third parameter value.
     * @return A Scalar instance.
     */
    template< typename T1, typename T2, typename T3 >
    static Scalar< Quantity, Unit > from(const T1& param1, const T2& param2, const T3& param3);
};

/**
 * @brief Overloaded output stream operator for Scalar.
 * @tparam Q The physical quantity type.
 * @tparam U The unit type.
 * @param os The output stream.
 * @param s The Scalar to output.
 * @return Reference to the output stream.
 */
template< typename Q, typename U >
std::ostream& operator<<(std::ostream& os, Scalar< Q, U > s)
{
    os << s.value;
    return os;
}

/**
 * @brief Overloaded addition operator for Scalar.
 * @tparam LHS_Q The physical quantity type of the left-hand side.
 * @tparam LHS_U The unit type of the left-hand side.
 * @tparam RHS_Q The physical quantity type of the right-hand side.
 * @tparam RHS_U The unit type of the right-hand side.
 * @param lhs The left-hand side Scalar.
 * @param rhs The right-hand side Scalar.
 * @return A new Scalar representing the sum.
 */
template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator+(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value + rhs.value;
    return result;
}

/**
 * @brief Overloaded subtraction operator for Scalar.
 * @tparam LHS_Q The physical quantity type of the left-hand side.
 * @tparam LHS_U The unit type of the left-hand side.
 * @tparam RHS_Q The physical quantity type of the right-hand side.
 * @tparam RHS_U The unit type of the right-hand side.
 * @param lhs The left-hand side Scalar.
 * @param rhs The right-hand side Scalar.
 * @return A new Scalar representing the difference.
 */
template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator-(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value - rhs.value;
    return result;
}

/**
 * @brief Overloaded multiplication operator for Scalar.
 * @tparam LHS_Q The physical quantity type of the left-hand side.
 * @tparam LHS_U The unit type of the left-hand side.
 * @tparam RHS_Q The physical quantity type of the right-hand side.
 * @tparam RHS_U The unit type of the right-hand side.
 * @param lhs The left-hand side Scalar.
 * @param rhs The right-hand side Scalar.
 * @return A new Scalar representing the product.
 */
template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator*(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value * rhs.value;
    return result;
}

/**
 * @brief Overloaded division operator for Scalar.
 * @tparam LHS_Q The physical quantity type of the left-hand side.
 * @tparam LHS_U The unit type of the left-hand side.
 * @tparam RHS_Q The physical quantity type of the right-hand side.
 * @tparam RHS_U The unit type of the right-hand side.
 * @param lhs The left-hand side Scalar.
 * @param rhs The right-hand side Scalar.
 * @return A new Scalar representing the quotient.
 */
template< typename LHS_Q, typename LHS_U, typename RHS_Q, typename RHS_U >
Scalar< LHS_Q, LHS_U > operator/(
    const Scalar< LHS_Q, LHS_U >& lhs,
    const Scalar< RHS_Q, RHS_U >& rhs)
{
    Scalar< LHS_Q, LHS_U > result = lhs.value / rhs.value;
    return result;
}

#endif // SCALAR_H_INCLUDED
