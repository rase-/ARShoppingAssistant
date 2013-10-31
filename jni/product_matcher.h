#ifndef PRODUCTMATCHER_H
#define PRODUCTMATCHER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <string>

class ProductMatcher
{
  private:
    Mat* freak_descriptors;
    Mat* surf_descriptors;
    std::string imagebase_path;

  public:
    ProductMatcher(Mat& freak_descriptors, Mat& surf_desciptors, std::string& imagebase_path);
    double match();
};

#endif
