#!/usr/bin/env python


import numpy as np
from cv2 import *
from multiprocessing.pool import ThreadPool

theta = 0
ksize = 31
sigma =  4.0
lmbda = 10.0
gamma =  0.5
psi   =  1.0

img  = None
fimg = None
kern = None

def build_kern():
    global kern
    # cv2.getGaborKernel(ksize, sigma, theta, lambda, gamma, psi, ktype)
    kern = getGaborKernel((ksize, ksize), sigma, theta, lmbda, gamma, psi, ktype=CV_32F)
    kern /=kern.sum()*1.5
    imshow("kernel",kern)

def process():
    global fimg
    build_kern()
    fimg = filter2D(img, CV_8UC3, kern)
    imshow("gabor", fimg)

def onTheta(v):
    global theta 
    theta = v/180.0*np.pi
    process()

def onLambda(v):
    global lmbda 
    lmbda = v/10.0
    process()

def onSigma(v):
    global sigma 
    sigma = v/10.0
    process()

def onGamma(v):
    global gamma 
    gamma = v/100.0
    process()


def onPsi(v):
    global psi 
    psi = v/100.0
    process()


if __name__ == '__main__':
    import sys
    from common import Timer

    print __doc__
    try:
        img_fn = sys.argv[1]
    except:
        img_fn = '../data/test01.jpg'

    img = imread(img_fn)
    if img is None:
        print 'Failed to load image file:', img_fn
        sys.exit(1)

    namedWindow("kernel"  , flags=WINDOW_NORMAL)
    # namedWindow("controls")

    process()

    # imshow('img', img)

    createTrackbar("theta","kernel",   0, 180, onTheta ), setTrackbarPos("theta" ,"kernel", int(theta    ))
    createTrackbar("lmbda","kernel",   0, 200, onLambda), setTrackbarPos("lmbda" ,"kernel", int(lmbda*10 ))
    createTrackbar("sigma","kernel",   0, 200, onSigma ), setTrackbarPos("sigma" ,"kernel", int(sigma*10 ))
    createTrackbar("gamma","kernel",   0, 300, onGamma ), setTrackbarPos("gamma" ,"kernel", int(gamma*100))
    createTrackbar("psi"  ,"kernel",-500, 500, onPsi   ), setTrackbarPos("psi"   ,"kernel", int(psi  *100))

    
    waitKey()
    destroyAllWindows()