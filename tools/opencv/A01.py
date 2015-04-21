from cv2 import imread 
import numpy as np

import matplotlib.pyplot as plt

img = None

def onClick(event):
    if event.dblclick:
        print("DBLCLICK", event)
    else:
        print("DOWN    ", event)

if __name__ == '__main__':

    import sys
    
    try:
        img_fn = sys.argv[1]
    except:
        img_fn = '../data/test03.png'

    img = imread(img_fn)
    if img is None:
        print 'Failed to load image file:', img_fn
        sys.exit(1)

    img1 = plt.imshow(img, cmap="gray", interpolation="nearest")
    plt.xticks([]), plt.yticks([])  # to hide tick values on X and Y axis
    img1.canvas.mpl_connect("button_press_event", onClick)
    plt.show()