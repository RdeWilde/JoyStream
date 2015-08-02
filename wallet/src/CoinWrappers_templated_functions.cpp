#include <wallet/CoinWrappers.hpp>

template<unsigned array_length>
std::array<unsigned char, array_length> toArray(const uchar_vector & vector) {

    // Check that vector is of correct length
    uchar_vector::size_type vectorLength = vector.size();

    if(vectorLength != array_length) {

        std::stringstream s;

        s << "Provided vector must be of length "
          << array_length
          << ", but was of length"
          << vectorLength;

        throw std::runtime_error(s.str());
    } else {

        // Create new array
        std::array<unsigned char, array_length> array;

        // Copy content of vector into array
        for(int i = 0;i < array_length;i++)
            array[i] = vector[i];

        // Return array
        return array;
    }
}

template<unsigned array_length>
uchar_vector toUCharVector(const std::array<unsigned char, array_length> & array) {
    return uchar_vector(array.cbegin(), array.cend());
}
