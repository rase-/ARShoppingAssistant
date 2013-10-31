#include "product_matcher.h"

ProductMatcher::ProductMatcher(cv::Mat& freak_descriptors, cv::Mat& surf_descriptors, std::string& imagebase_path) {
    this->freak_descriptors = freak_descriptors;
    this->surf_descriptors = surf_descriptors;
    this->imagebase_path = imagebase_path;
}

// Actually rethink this one
double ProductMatcher::match() {
    cv::BFMatcher matcher(cv::NORM_L2);
    std::vector<cv::DMatch> matches;
    matcher.match(freak_descriptors, freak_descriptors, matches);
    // do domething with them matches and actually match with stuff from the
    // images we have in store
    return 0.0;
}

void ProductMatcher::loadProductInfo() {
    // Load from assets folder or something
}
