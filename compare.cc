//g++ -std=c++11 -o compare compare.cc `pkg-config opencv --cflags` `pkg-config opencv --libs`
#include <iostream>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>

#include "opencv2/opencv.hpp"

using namespace cv;

using namespace std;

vector<string> ReadKeyFrame(const string &filename)
{
    FILE *fp;
    fp = fopen(filename.c_str(), "r");
    if (fp == NULL)
    {
        cout << "file open error" << endl;
        exit(0);
    }
    vector<float> pos(3);
    vector<float> rotate(4);
    vector<string> timestamps;

    while (1)
    {
        char tempname[256];
        int ret = fscanf(fp, "%s %f %f %f %f %f %f %f\n", tempname, &pos[0], &pos[1], &pos[2],
                         &rotate[0], &rotate[1], &rotate[2], &rotate[3]);
        if (ret == -1)
            break;
        timestamps.push_back(string(tempname));
    }

    fclose(fp);
    return timestamps;
}

double ransac(const string &f1, const string &f2)
{
    Mat obj = imread("../data1/rgb/" + f1 + ".png");   //载入目标图像
    Mat scene = imread("../data2/rgb/" + f2 + ".png"); //载入场景图像

    //  Mat obj = imread("1.png");   //载入目标图像
    //  Mat scene = imread("2.png"); //载入场景图像

    if (obj.empty() || scene.empty())
    {
        cout << "Can't open the picture!\n";
        exit(0);
    }
    //滤除误匹配前
    vector<KeyPoint> obj_keypoints, scene_keypoints;
    Mat obj_descriptors, scene_descriptors;
    Ptr<ORB> detector = ORB::create(); //采用ORB算法提取特征点
    detector->detect(obj, obj_keypoints);
    detector->detect(scene, scene_keypoints);
    detector->compute(obj, obj_keypoints, obj_descriptors);
    detector->compute(scene, scene_keypoints, scene_descriptors);
    BFMatcher matcher(NORM_HAMMING, true); //汉明距离做为相似度度量
    vector<DMatch> matches;
    matcher.match(obj_descriptors, scene_descriptors, matches);

    //   cout << obj_descriptors.rows << " " << obj_descriptors.cols << endl;
    // cout << scene_descriptors.rows << " " << scene_descriptors.cols << endl;

    // Mat match_img;
    // drawMatches(obj, obj_keypoints, scene, scene_keypoints, matches, match_img);
    // imshow("滤除误匹配前", match_img);

    //匹配点转为Point2f像素坐标形式保存到points中
    vector<int> queryIdxs(matches.size()), trainIdxs(matches.size());
    for (size_t i = 0; i < matches.size(); i++)
    {
        queryIdxs[i] = matches[i].queryIdx;
        trainIdxs[i] = matches[i].trainIdx;
    }
    vector<Point2f> points1;
    KeyPoint::convert(obj_keypoints, points1, queryIdxs);
    vector<Point2f> points2;
    KeyPoint::convert(scene_keypoints, points2, trainIdxs);
    int ransacReprojThreshold = 50; //拒绝阈值

    //       std::cout << obj_keypoints.size() << " " << points1.size() << " " << points2.size() << matches.size() << std::endl;

    vector<uchar> RansacStatus;

    Mat H12; //单应性矩阵H
    H12 = findHomography(Mat(points1), Mat(points2), CV_RANSAC, ransacReprojThreshold, RansacStatus);

    vector<char> matchesMask(matches.size(), 0);
    Mat points1t;
    perspectiveTransform(Mat(points1), points1t, H12); //透视变换,points1t为变换后的点

    int innercount = 0;

    for (size_t i1 = 0; i1 < points1.size(); i1++) //保存‘内点’
    {
        if (norm(points2[i1] - points1t.at<Point2f>((int)i1, 0)) <= ransacReprojThreshold) //给内点做标记
        {
            innercount++;
            matchesMask[i1] = 1;
        }
    }

    // // draw
    // Mat match_img2; //滤除‘外点’后
    // drawMatches(obj, obj_keypoints, scene, scene_keypoints, matches, match_img2, Scalar(0, 0, 255), Scalar::all(-1), matchesMask);

    // //画出目标位置
    // std::vector<Point2f> obj_corners(4); //初始化为整个目标图像的四个角
    // obj_corners[0] = cvPoint(0, 0);
    // obj_corners[1] = cvPoint(obj.cols, 0);
    // obj_corners[2] = cvPoint(obj.cols, obj.rows);
    // obj_corners[3] = cvPoint(0, obj.rows);
    // std::vector<Point2f> scene_corners(4);
    // perspectiveTransform(obj_corners, scene_corners, H12); //透视变换
    // line(match_img2, scene_corners[0] + Point2f(static_cast<float>(obj.cols), 0),
    //      scene_corners[1] + Point2f(static_cast<float>(obj.cols), 0), Scalar(0, 0, 255), 2);
    // line(match_img2, scene_corners[1] + Point2f(static_cast<float>(obj.cols), 0),
    //      scene_corners[2] + Point2f(static_cast<float>(obj.cols), 0), Scalar(0, 0, 255), 2);
    // line(match_img2, scene_corners[2] + Point2f(static_cast<float>(obj.cols), 0),
    //      scene_corners[3] + Point2f(static_cast<float>(obj.cols), 0), Scalar(0, 0, 255), 2);
    // line(match_img2, scene_corners[3] + Point2f(static_cast<float>(obj.cols), 0),
    //      scene_corners[0] + Point2f(static_cast<float>(obj.cols), 0), Scalar(0, 0, 255), 2);

    // imshow("滤除误匹配后", match_img2);
    // imwrite("x.png", match_img2);
    // waitKey(0);

    //std::cout << innercount << points1.size() << " " << points2.size() << std::endl;

    double output = static_cast<double>(innercount) / points1.size();
    //std::cout << output << std::endl;
    return output;
}

int main(int argc, char **argv)
{
    vector<string> files1;
    vector<string> files2;
    const char keyframe1[] = "KeyFrameTrajectory-1.txt";
    const char keyframe2[] = "KeyFrameTrajectory-2.txt";

    files1 = ReadKeyFrame(keyframe1);
    files2 = ReadKeyFrame(keyframe2);

    cout << "keyframe of video1:" << files1.size() << endl;
    cout << "keyframe of video2:" << files2.size() << endl;

    //  ransac(files1[0], files2[0]);
    //  return 0;

    vector<vector<double>> results(files1.size(), vector<double>(files2.size()));
    double maxresult = 0;
    pair<int, int> maxij;
    for (size_t i = 0; i < files1.size(); i++)
    {
        for (size_t j = 0; j < files2.size(); j++)
        {
            results[i][j] = ransac(files1[i], files2[j]);
            if (maxresult < results[i][j])
            {
                cout << maxresult << endl;
                maxresult = results[i][j];
                maxij = {i, j};
            }
        }
    }

    cout << files1[maxij.first] << " " << files2[maxij.second] << " " << maxresult << endl;

    return 0;
}
