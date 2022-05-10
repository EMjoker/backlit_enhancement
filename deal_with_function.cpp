#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

const double b_alpha = 48;
const double b_beta = 48;

double Y_Pixel(double r_px, double g_px, double b_px) {
    return 0.299 * r_px + 0.587 * g_px + 0.114 * b_px;
}

double alphafunc(const double alpha_max,double Y_Px) {
    return 1 + (alpha_max - 1) * pow(Y_Px - 128 - b_alpha, 2) / pow(128, 2);
}

double betafunc(const double beta_th, double Y_Px) {
    return beta_th * pow(128 - Y_Px - b_beta, 3) / pow(128, 3);
}

int main(int argc, char** argv)
{
    using namespace cv;
    using namespace std;

    Mat dst;
    Mat src = imread("/*Image Path*/", IMREAD_COLOR);
    if (src.empty()) {
        cout << "LOAD FAIL." << endl;
    }
    resize(src, src, Size(1400, 1050));
    namedWindow("test", WINDOW_AUTOSIZE);
    imshow("test", src);

    const char input_title[] = "input_demo";

    //contrast-brightness
    const int height = src.rows;
    const int width = src.cols;
    dst = Mat::zeros(src.size(), src.type());
    const double alpha_max = 1.3;
    const double beta_th = 50.0;

    Mat m1;
    src.convertTo(m1, CV_64F);

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (src.channels() == 3) {
                double r = m1.at<Vec3d>(row, col)[2];
                double g = m1.at<Vec3d>(row, col)[1];
                double b = m1.at<Vec3d>(row, col)[0];

                double Y_Px = Y_Pixel(r, g, b);
                double alpha = alphafunc(alpha_max, Y_Px);
                double beta = betafunc(beta_th, Y_Px);

                dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(b * alpha + beta);
                dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(g * alpha + beta);
                dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(r * alpha + beta);
            }
            else if (src.channels() == 1) {
                float v = src.at<uchar>(row, col);
                dst.at<uchar>(row, col) = saturate_cast<uchar>(v * 1 + 0);
            }
        }
    }

    const char output_title[] = "contrast-brightness_alter_test";
    namedWindow(output_title, WINDOW_AUTOSIZE);
    imshow(output_title, dst);

    waitKey(0);
    return 0;
}
