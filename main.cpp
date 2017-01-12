/**
 * @file MatchTemplate_Demo.cpp
 * @brief Sample code to use the function MatchTemplate
 * @author OpenCV team
 */

#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace cv;

/// Global Variables
map <int,string> heroes;
bool use_mask;
Mat img; Mat templ; Mat mask; Mat result;
const char* image_window = "Source Image";

int match_method;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod( int, void* );

/**
 * @function main
 */
int main( int argc, char** argv )
{
  if (argc < 3)
  {
    cout << "Not enough parameters" << endl;
    cout << "Usage:\n./MatchTemplate_Demo <image_name> <template_name> [<mask_name>]" << endl;
    return -1;
  }

  /// Load image and template
  img = imread( argv[1], IMREAD_COLOR );
  templ = imread( argv[2], IMREAD_COLOR );

  if(argc > 3) {
    use_mask = true;
    cout << "ACCEPTING MASK";
    mask = imread( argv[3], IMREAD_COLOR );
  }

  if(img.empty() || templ.empty() || (use_mask && mask.empty()))
  {
    cout << "Can't read one of the images" << endl;
    return -1;
  }

  /// Create windows
  namedWindow( image_window, WINDOW_AUTOSIZE );

  MatchingMethod( 0, 0 );


  unsigned int microseconds = 1000000;

  while (waitKey(0)){};
  return 0;
}



/**
 * @function MatchingMethod
 * @brief Trackbar callback
 */
void MatchingMethod( int, void* )
{
  /// Source image to display
  Mat img_display;
  img.copyTo( img_display );

  /// Create the result matrix
  int result_cols =  img.cols - templ.cols + 1;
  int result_rows = img.rows - templ.rows + 1;

  result.create( result_rows, result_cols, CV_32FC1 );

  /// Do the Matching and Normalize
  bool method_accepts_mask = (CV_TM_SQDIFF == match_method || match_method == CV_TM_CCORR_NORMED);

  matchTemplate( img, templ, result, TM_SQDIFF);

  normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

  /// Localizing the best match with minMaxLoc
  double minVal; double maxVal; Point minLoc; Point maxLoc;
  Point matchLoc;

  minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );


  /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
  if( match_method  == TM_SQDIFF || match_method == TM_SQDIFF_NORMED )
    { matchLoc = minLoc; }
  else
    { matchLoc = maxLoc; }

  /// Show me what you got
  rectangle( img_display, matchLoc, Point( matchLoc.x + templ.cols , matchLoc.y + templ.rows ), Scalar::all(255), 2, 8, 0 );
  circle(img_display, Point(matchLoc.x + templ.cols / 2, matchLoc.y + templ.rows / 2 ), 5, Scalar::all(255));

  imshow( image_window, img_display );

  return;
}
