#coding=utf-8
import numpy as np


np.set_printoptions(precision=7)

MA = []
MB = []
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

MRc = []
with open("Rc.txt", 'r') as f:
    line = f.readline()
    while line:
        eachline = line.split()###按行读取文本文件，每行数据以列表形式返回
        read_data = [ float(x) for x in eachline[0:3] ] #TopN概率字符转换为float型
        MRc.append(read_data[0:3])
        line = f.readline()

Mtc = []
with open("tc.txt", 'r') as f:
    line = f.readline()
    while line:
        eachline = line.split()###按行读取文本文件，每行数据以列表形式返回
        read_data = [ float(x) for x in eachline[0:3] ] #TopN概率字符转换为float型
        Mtc.append(read_data[0:3])
        line = f.readline()



NA = np.mat(MA)
NB = np.mat(MB)

Rc = np.mat(MRc)
tc = np.mat(Mtc)

print('Rc',Rc)
print('tc',tc)

n = len(MA)
print(n)
A_transformed = (Rc*NA.T) + np.tile(tc, (1, n))
A_transformed = A_transformed.T

np.savetxt("transed.txt",A_transformed,'%.6f')