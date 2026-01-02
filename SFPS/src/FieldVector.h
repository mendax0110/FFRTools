#ifndef FIELDVECTOR_H_INCLUDED
#define FIELDVECTOR_H_INCLUDED

/// @brief A struct to represent a field vector. \struct FieldVector
struct FieldVector
{
    typedef long long magnitude_t;

    magnitude_t fx;
    magnitude_t fy;
    magnitude_t fz;
};

#endif // FIELDVECTOR_H_INCLUDED
