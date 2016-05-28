#include <type_traits>

template<unsigned D, unsigned D2 = D>
class CrazyPoint : public CrazyPoint<D-1> {
 public:
    CrazyPoint() {
        std::cout << "D == " << D << std::endl;
    }
};

template<unsigned D>
class CrazyPoint<D, 1> {
 public:
    CrazyPoint() {
        std::cout << "D == 1" << std::endl;
    }
};