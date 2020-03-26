#coding=utf-8
#https://medium.com/machine-learning-world/linear-algebra-points-matching-with-svd-in-3d-space-2553173e8fed
#https://colab.research.google.com/drive/1pZPV5GuR7ZPW6vyoG8DnawJx13k6zHmD#scrollTo=eH0dKg3PpTLo
import numpy as np

def euclidean_transform_3D(A, B):
    '''
        A,B - Nx3 matrix
        return:
            R - 3x3 rotation matrix
            t = 3x1 column vector
    '''
    assert len(A) == len(B)

    # number of points
    N = A.shape[0]; 

    centroid_A = np.mean(A, axis=0)
    centroid_B = np.mean(B, axis=0)
    
    # centre matrices
    AA = A - np.tile(centroid_A, (N, 1))
    BB = B - np.tile(centroid_B, (N, 1))

    # covariance of datasets
    H = np.transpose(AA) * BB

    # matrix decomposition on rotation, scaling and rotation matrices
    U, S, Vt = np.linalg.svd(H)

    # resulting rotation
    R = Vt.T * U.T
    print('R',R)
    #prinyt(Vt)
    print(Vt)
    # handle svd sign problem
    if np.linalg.det(R) < 0:
        print ("sign")
        # thanks to @valeriy.krygin to pointing me on a bug here
        Vt[2,:] *= -1
        R = Vt.T * U.T
        print('new R',R)

    t = -R*centroid_A.T + centroid_B.T

    return R, t

def test():
    R = np.mat(np.random.rand(3,3))
    t = np.mat(np.random.rand(3,1))

    U, S, Vt = np.linalg.svd(R)
    R = U*Vt

    if np.linalg.det(R) < 0:
        print('sign')
        Vt[2,:] *= -1
        R = U*Vt
    n = 10

    A = np.mat(np.random.rand(n,3));
    B = R*A.T + np.tile(t, (1, n))
    B = B.T;

    # recover the transformation
    Rc, tc = euclidean_transform_3D(A, B)

    print('Rc',Rc)
    print('tc',tc)

    A_transformed = (Rc*A.T) + np.tile(tc, (1, n))
    A_transformed = A_transformed.T

    # Find the error
    rmse = np.sqrt(np.mean(np.square(A_transformed - B)))
    print ("RMSE:", rmse)
    

MA = []
MB = []

with open("MatchPoints.txt", 'r') as f:#with语句自动调用close()方法
    line = f.readline()
    line = f.readline()
    while line:
        eachline = line.split()###按行读取文本文件，每行数据以列表形式返回
        
        read_data = [ float(x) for x in eachline[0:6] ] #TopN概率字符转换为float型

        MA.append(read_data[0:3])
        MB.append(read_data[3:6])
        line = f.readline()
   

NA = np.mat(MA)
NB = np.mat(MB)


 # recover the transformation
Rc, tc = euclidean_transform_3D(NA, NB)

print('Rc',Rc)
print('tc',tc)

n = len(MA)
A_transformed = (Rc*NA.T) + np.tile(tc, (1, n))
A_transformed = A_transformed.T

# Find the error
rmse = np.sqrt(np.mean(np.square(A_transformed - NB)))
print ("RMSE:", rmse)


np.savetxt("Rc.txt",Rc,'%.6f')
np.savetxt("tc.txt",tc,'%.6f')
