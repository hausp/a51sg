/* created by Ghabriel Nunes<ghabriel.nunes@gmail.com> 
   and Marleson Graf<aszdrick@gmail.com> [2016] */

#include <cmath>
#include "Drawable.hpp"
#include "BaseDrawer.hpp"

template<size_t D>
void BaseDrawer::translate(size_t index, const std::array<double, D>& ds) {
    if (index < displayFile.size()) {
        //displayFile[index]->transform(utils::translationMatrix(ds));
    }
}

template<size_t D>
void BaseDrawer::scale(size_t index, const std::array<double, D>& ss) {
    if (index < displayFile.size()) {
        auto& shape = displayFile[index];
        auto center = shape->center();
        // auto m = utils::translationMatrix((-center).toArray());
        // m *= utils::scalingMatrix(ss);
        // m *= utils::translationMatrix(center.toArray());
        // shape->transform(BaseTransformation(m));
    }
}