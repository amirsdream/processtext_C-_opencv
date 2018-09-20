#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "tesseract/baseapi.h"
#include "iostream"

using namespace cv;
using namespace std;

int main() {
    // Load image
    Mat im = imread("/home/amir/image.jpg");
    if (im.empty()) {
        cout << "Cannot open source image!" << endl;
        return -1;
    }

    Mat gray;

    cvtColor(im, gray, CV_BGR2GRAY);


    // ...other image pre-processing here...
    double angle = -90;

    // get rotation matrix for rotating the image around its center in pixel coordinates
    Point2f center((gray.cols - 1) / 2.0, (gray.rows - 1) / 2.0);
    Mat rot = getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    Rect2f bbox = RotatedRect(Point2f(), gray.size(), angle).boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0, 2) += bbox.width / 2.0 - gray.cols / 2.0;
    rot.at<double>(1, 2) += bbox.height / 2.0 - gray.rows / 2.0;
    Mat dst;
    warpAffine(gray, dst, rot, bbox.size());
    addWeighted(dst, 1.5, dst, -0.5, 0, dst);
    adaptiveThreshold(dst, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
    GaussianBlur(dst, dst, Size(5, 5), 11);
    threshold(dst, dst, 0, 255, THRESH_BINARY + CV_THRESH_OTSU);
//    imshow("Display window", dst);

    // Pass it to Tesseract API
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetImage((uchar *) dst.data, dst.cols, dst.rows, 1, dst.cols);


    // Get the text
    char *out = tess.GetUTF8Text();
//    cout << out << endl;
    string arr[200];
    int i, j = 0;
    stringstream ssin(out);
    while (ssin.good() && i < 200) {
        ssin >> arr[i];
        if (arr[i].length()>10&&arr[i].length()<12){
            cout<<arr[i]<<"\n";
        }
        if ( ssin.str().empty() ) {
            break;
        }
        ++i;
    }
//    for (j = 0; j < i; j++) {
//
//        cout << arr[j] << endl;
//    }
//    waitKey(0);
    return 0;
}