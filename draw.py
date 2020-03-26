#coding=utf-8
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

np.set_printoptions(precision=6)

MA = []
MB = []
MC = []


with open("points-1.txt", 'r') as f:#with语句自动调用close()方法
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


with open("transed.txt", 'r') as f:#with语句自动调用close()方法
    line = f.readline()
    line = f.readline()
    while line:
        eachline = line.split()###按行读取文本文件，每行数据以列表形式返回
        read_data = [ float(x) for x in eachline[0:3] ] #TopN概率字符转换为float型

        MC.append(read_data[0:3])
        line = f.readline()

NA = np.mat(MA)
NB = np.mat(MB)
NC = np.mat(MC)
def draw1(data):    
    x = data[:, 0]
    y = data[:, 1] 
    z = data[:, 2]
    # 绘制散点图
    fig = plt.figure()
    ax = Axes3D(fig)
    ax.scatter(x, y, z)
    
    # 添加坐标轴(顺序是Z, Y, X)
    ax.set_zlabel('Z', fontdict={'size': 15, 'color': 'red'})
    ax.set_ylabel('Y', fontdict={'size': 15, 'color': 'red'})
    ax.set_xlabel('X', fontdict={'size': 15, 'color': 'red'})
    plt.show()


def draw2(data1,data2):    
    x1 = data1[:, 0]
    y1 = data1[:, 1] 
    z1 = data1[:, 2]
    # 绘制散点图
    fig = plt.figure()
    ax = Axes3D(fig)
    
    
    x2 = data2[:, 0]
    y2 = data2[:, 1] 
    z2 = data2[:, 2]


    ax.scatter(x1, y1, z1,c='b',label='video2')
    ax.scatter(x2, y2, z2,c='r',label='transformed')
    
    ax.legend(loc='best')

    # 添加坐标轴(顺序是Z, Y, X)
    ax.set_zlabel('Z', fontdict={'size': 15, 'color': 'red'})
    ax.set_ylabel('Y', fontdict={'size': 15, 'color': 'red'})
    ax.set_xlabel('X', fontdict={'size': 15, 'color': 'red'})
    plt.show()


def draw3(data1,data2,data3):    
    x1 = data1[:, 0]
    y1 = data1[:, 1] 
    z1 = data1[:, 2]
    # 绘制散点图
    fig = plt.figure()
    ax = Axes3D(fig)
    
    
    x2 = data2[:, 0]
    y2 = data2[:, 1] 
    z2 = data2[:, 2]

    x3 = data3[:, 0]
    y3 = data3[:, 1] 
    z3 = data3[:, 2]

    ax.scatter(x1, y1, z1,c='g',label='video1')
    ax.scatter(x2, y2, z2,c='b',label='video2')
    ax.scatter(x3, y3, z3,c='r',label='transformed')
    
    ax.legend(loc='best')

    # 添加坐标轴(顺序是Z, Y, X)
    ax.set_zlabel('Z', fontdict={'size': 15, 'color': 'red'})
    ax.set_ylabel('Y', fontdict={'size': 15, 'color': 'red'})
    ax.set_xlabel('X', fontdict={'size': 15, 'color': 'red'})
    plt.show()

draw2(NB,NC)

draw3(NA,NB,NC)
