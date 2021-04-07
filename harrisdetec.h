#if !defined HARRISD
#define HARRISD

#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>

class HarrisDetector {

  private:

    // 32-bit float image of corner strength
    cv::Mat cornerStrength;
    // 32-bit float image of thresholded corners
    cv::Mat cornerTh;
    // image of local maxima (internal)
    cv::Mat localMax;
    // size of neighbourhood for derivatives smoothing
    int neighborhood; 
    // aperture for gradient computation
    int aperture; 
    // Harris parameter
    double k;
    // maximum strength for threshold computation
    double maxStrength;
    // calculated threshold (internal)
    double threshold;
    // size of neighbourhood for non-max suppression
    int nonMaxSize; 
    // kernel for non-max suppression
    cv::Mat kernel;

  public:
    void printMatValues(cv::Mat& img){
      cv::Mat float_img;
      img.convertTo(float_img, CV_32F);
      std::cout<<img;


    }

    HarrisDetector() : neighborhood(3), aperture(3), k(0.1), maxStrength(0.0), threshold(0.01), nonMaxSize(3) {
    
      setLocalMaxWindowSize(nonMaxSize);
    }

    // Create kernel used in non-maxima suppression
    void setLocalMaxWindowSize(int size) {

      nonMaxSize= size;
      kernel.create(nonMaxSize,nonMaxSize,CV_8U);
    }

    void findLocalMaxima(cv::Mat& src){
      cv::Mat dilate;
      cv::dilate(src, dilate, cv::Mat());
      // compare den sonra 0 - 255 arasinda degerler olusuyor.
      cv::compare(src, dilate, localMax, cv::CMP_EQ);

      //printMatValues(localMax);
    }


    // Compute Harris corners
    void detect(const cv::Mat& image) {

      // Harris computation
      cv::cornerHarris(image,cornerStrength,
                 neighborhood,// neighborhood size
           aperture,     // aperture size
           k);           // Harris parameter

      //printMatValues(cornerStrength);
      // internal threshold computation
      cv::minMaxLoc(cornerStrength,0,&maxStrength);
      findLocalMaxima(cornerStrength);

    }

    // Get the corner map from the computed Harris values
    cv::Mat getCornerMap(double qualityLevel) {

      cv::Mat cornerMap;

      // thresholding the corner strength
      threshold= qualityLevel*maxStrength;
      cv::threshold(cornerStrength,cornerTh,threshold,255,cv::THRESH_BINARY);

      // convert to 8-bit image
      cornerTh.convertTo(cornerMap,CV_8U);
      // non-maxima suppression
      cv::bitwise_and(cornerMap,localMax,cornerMap);

      return cornerMap;
    }

    // Get the feature points vector from the computed Harris values
    void getCorners(std::vector<cv::Point> &points, double qualityLevel) {

      // Get the corner map
      // corners represent as 255
      // other places represent as 0
      cv::Mat cornerMap= getCornerMap(qualityLevel);
      // now we will find the coordinat of corners that represent as 255 in cornermap
      //points vector x ve y lerin tutulacagi yer oluyor 
      //quality level ?

      for (int y = 0;  y <cornerMap.rows; y++){
        // dongudekn gelen rowun baslangic adresini tutuyor
        const uchar* rowptr = cornerMap.ptr<uchar>(y);

        for (int x = 0; x<cornerMap.cols; x++){
          if(rowptr[x]){
            points.push_back(cv::Point(x,y));

          }

        }

      }

    }
    void drawOnImage(cv::Mat &image, const std::vector<cv::Point> &points, cv::Scalar color= cv::Scalar(255,255,255), int radius=3, int thickness=1) {
      std::vector<cv::Point>::const_iterator it = points.begin();

      while(it!=points.end()){
          cv::circle(image, *it, radius, color, thickness);
          ++it;

          }


    }

};



#endif
