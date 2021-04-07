#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include "harrisdetec.h"

int main(){
  cv::Mat image1 = cv::imread("Image1.jpg",0);
  if(!image1.data){
    std::cout<<"image coudn't loaded";
    return -1;
  }

  //resmin uzerinde nokta olarak gosterme islemi
  HarrisDetector harris;
  //yuridaki islemleri ayni sekilde yapiyoruz
  harris.detect(image1);

  //harris cornerlerinin x ve y kordinatlarini tutacagimiz yer
  std::vector<cv::Point> points;
  harris.getCorners(points, 0.01);
  //cv::imshow("img",image1);
  //for (int i; i<points.size(); i++){
  //  std::cout<<points.at(i)<<' ' ;
  //}
  harris.drawOnImage(image1,points);

  cv::imshow("deneme", image1);



  cv::waitKey(0);

  return 0;
}
