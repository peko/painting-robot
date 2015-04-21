from cv2 import *
from numpy import *
import sys



if __name__ == '__main__':

    try:
        img_fn = sys.argv[1]
    except:
        img_fn = '../data/test03.png'

    img = imread(img_fn)
    if img is None:
        print 'Failed to load image file:', img_fn
        sys.exit(1)

    namedWindow("kernel"  , flags=WINDOW_NORMAL)
    imshow("image", img)
    # namedWindow("controls")
    waitKey()

    process()
