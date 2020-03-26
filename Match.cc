//g++ -std=c++11 -o Match Match.cc `pkg-config opencv --cflags` `pkg-config opencv --libs`
#include <iostream>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include "opencv2/opencv.hpp"

using namespace cv;

using namespace std;

void ReadDP(const string &filename, vector<vector<unsigned>> &des, vector<vector<float>> &points)
{
    ifstream fin;
    fin.open(filename);
    int N;
    fin >> N;

    for (int n = 0; n < N; n++)
    {
        vector<unsigned> td(32);
        vector<float> tp(3);
        for (int i = 0; i < td.size(); i++)
        {
            fin >> td[i];
            //cout << fixed << setprecision(7) << td[i];
        }

        des.push_back(td);
        for (int i = 0; i < 3; i++)
        {
            fin >> tp[i];
            //   cout << fixed << setprecision(7) << tp[i] << " ";
        }
        //cout << endl;
        points.push_back(tp);
    }

    fin.close();
}

int main(int argc, char **argv)
{

    const char MapPoint1[] = "MapPoint-1.txt";
    const char MapPoint2[] = "MapPoint-2.txt";

    vector<vector<unsigned>> d1, d2;
    vector<vector<float>> p1, p2;

    ReadDP(MapPoint1, d1, p1);
    ReadDP(MapPoint2, d2, p2);

    cv::Mat md1(d1.size(), d1[0].size(), CV_8U);
    cv::Mat md2(d2.size(), d2[0].size(), CV_8U);

    for (int i = 0; i < md1.rows; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            md1.at<uchar>(i, j) = d1[i][j];
        }
    }

    for (int i = 0; i < md2.rows; i++)
    {
        for (int j = 0; j < 32; j++)
        {
            md2.at<uchar>(i, j) = d2[i][j];
        }
    }

    BFMatcher matcher(NORM_HAMMING, true); //汉明距离做为相似度度量
    vector<DMatch> matches;
    matcher.match(md1, md2, matches);

    cout << "Match Points: " << matches.size() << endl;

    vector<int> queryIdxs(matches.size()), trainIdxs(matches.size());
    for (size_t i = 0; i < matches.size(); i++)
    {
        queryIdxs[i] = matches[i].queryIdx;
        trainIdxs[i] = matches[i].trainIdx;
    }

    ofstream fout;
    fout.open("MatchPoints.txt");
    fout << matches.size() << endl;
    for (int i = 0; i < queryIdxs.size(); i++)
    {
        fout << fixed << setprecision(7) << p1[i][0] << " " << p1[i][1] << " " << p1[i][2] << " "
             << p2[queryIdxs[i]][0] << " " << p2[queryIdxs[i]][1] << " " << p2[queryIdxs[i]][2] << endl;
    }
    return 0;
}
