#ifndef PRODUCTMATCHER_H
#define PRODUCTMATCHER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>

// This could actually just be moved to the server man...
class ProductMatcher
{
  private:
    cv::Mat freak_descriptors;
    cv::Mat surf_descriptors;
    std::string imagebase_path;
    
    void loadProductInfo();
  public:
    ProductMatcher(cv::Mat& freak_descriptors, cv::Mat& surf_desciptors, std::string& imagebase_path);
    double match();
};

#endif
