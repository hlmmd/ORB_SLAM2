//g++ -o test Timestamp.cc
#include <iostream>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <stdlib.h>
using namespace std;
int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        cout << "usage: ./test N " << endl;
        cout << "N is number of photo" << endl;
    }

    struct timeval temp;
    char timestr[128];
    memset(timestr, 0, sizeof(timestr));

    int N = atoi(argv[1]);

    vector<time_t> secs(N);
    vector<time_t> usecs(N);

    for (int i = 0; i < N; i++)
    {
        gettimeofday(&temp, NULL);
        secs[i] = temp.tv_sec;
        usecs[i] = temp.tv_usec;
        //30 fps
        usleep(1000000 / 30);
    }

    //生成rgb.txt
    FILE *fp = fopen("rgb.txt", "w");
    if (fp == NULL)
        return -1;

    //orb_slam2 will ignore first 3 lines
    for (int i = 0; i < 3; i++)
    {
        fprintf(fp, "#\n");
    }

    for (int i = 0; i < N; i++)
    {
        int ret = snprintf(timestr, 128, "%010ld.%06ld rgb/%010ld.%06ld.png", secs[i], usecs[i], secs[i], usecs[i]);
        fprintf(fp, "%s\n", timestr);
    }

    fclose(fp);

    //重命名png图片

    char oldName[256];
    char newName[256];

    for (int i = 0; i < N; i++)
    {
        snprintf(oldName, sizeof(oldName), "rgb/output_%08d.png", i + 1);
        snprintf(newName, sizeof(newName), "rgb/%010ld.%06ld.png", secs[i], usecs[i]);
        if (!rename(oldName, newName))
        {
        }
        else
        {
            std::cout << "rename " << oldName << " error " << std::endl;
        }
    }

    return 0;
}
