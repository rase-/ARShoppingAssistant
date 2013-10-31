#include "product_matcher.h"

ProductMatcher::ProductMatcher(Mat& freak_descriptors, Mat& surf_descriptors, std::string& imagebase_path)
{
    this->freak_descriptors = freak_descriptors;
    this->surf_descriptors = surf_descriptors;
    this->imagebase_path = imagebase_path;
}

double ProductMatcher::match()
{
    // TODO: implement matching with OpenCV bruteforcematcher
}
