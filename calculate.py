#coding=utf-8

from __future__ import division
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

np.set_printoptions(precision=8)

MA = []
MB = []

with open("transed.txt", 'r') as f:#with语句自动调用close()方法
    line = f.readline()
    line = f.readline()
    while line:
        eachline = line.split()###按行读取文本文件，每行数据以列表形式返回
        read_data = [ float(x) for x in eachline[0:3] ] #TopN概率字符转换为float型
        MA.append(read_data[0:3])
        line = f.readline()

with open("points-2.txt", 'r') as f:#with语句自动调用close()方法
    line = f.readline()
    line = f.readline()
    while line:
        eachline = line.split()###按行读取文本文件，每行数据以列表形式返回        
        read_data = [ float(x) for x in eachline[0:3] ] #TopN概率字符转换为float型
        MB.append(read_data[0:3])
        line = f.readline()


# def eucliDist(A,B):
# return np.sqrt(sum([(a - b)**2 for (a,b) in zip(A,B)]))

NA = np.mat(MA)
NB = np.mat(MB)

# print(NA[0])
# print(NB[0])

#dist = np.sqrt(np.sum(np.square(NA[0] - NB[0])))
#dist = eucliDist(MA[0],MB[0])

#print(dist)

count = 0
total = min(len(NA),len(NB))

visited = [0]*len(NB)

#print(total)

value = np.sqrt(3) * 0.01   #  18.09 %
#value = np.sqrt(3) * 0.02   # 46.86 %
#value = np.sqrt(3) * 0.03  # 68.09%
#value = np.sqrt(3) * 0.04  # 79.71%
#value = np.sqrt(3) * 0.05  #  85.19%
#value = 0.03
#print(value)


for vec1 in NA:
    mindist = 100.0
    minidx = -1
    #for vec2 in NB:
    for j in range(0,len(NB)):
        if visited [j] == 1 :
            continue
        vec2 = NB[j]
        mindist = min(mindist,np.linalg.norm(vec1 - vec2))
        minidx = j

    if(mindist<value):
        visited[minidx] =1
        print(mindist)
        count = count+1

print(count,total)
print(count/total)


