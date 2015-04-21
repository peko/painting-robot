import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('../data/test04.jpg',0)
equ = cv2.equalizeHist(img)


blur = cv2.bilateralFilter(img,9,75,75)
# kernel = np.ones((5,5),np.uint8)
# erosion = cv2.dilate(img, kernel, iterations = 5)
# erosion = cv2.erode(erosion, kernel, iterations = 5)

thr = cv2.adaptiveThreshold(blur,255,cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY,11,2)
cnt = thr.copy()
contours, hierarchy = cv2.findContours(cnt, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

# corners = cv2.goodFeaturesToTrack(img,25,0.01,10)
# corners = np.int0(corners)
# for i in corners:
# 	x,y = i.ravel()
# 	cv2.circle(img,(x,y),3,255,-1)

# gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
gray = np.float32(img)
dst = cv2.cornerHarris(gray,2,3,0.04)
dst = cv2.dilate(dst,None)

cnt = cv2.cvtColor(thr, cv2.COLOR_GRAY2RGB)
cv2.drawContours(cnt, contours, -1, (0,0,255), 1)
cnt[dst>0.01*dst.max()]=[255,0,0]

# cv2.imshow('img',img)
# cv2.imshow('erosion',erosion)
# cv2.waitKey(0)
# cv2.destroyAllWindows()
# plt.imshow(img, cmap = 'gray', interpolation = 'bicubic')
# plt.xticks([]), plt.yticks([])  # to hide tick values on X and Y axis
mng = plt.get_current_fig_manager()
mng.resize(*mng.window.maxsize())

plt.subplot(1,3,1), plt.imshow(img      ,'gray'), plt.xticks([]), plt.yticks([])
plt.subplot(1,3,2), plt.imshow(equ      ,'gray'), plt.xticks([]), plt.yticks([])
plt.subplot(1,3,3), plt.imshow(blur     ,'gray'), plt.xticks([]), plt.yticks([])
plt.show()

plt.subplot(1,3,1), plt.imshow(thr      ,'gray'), plt.xticks([]), plt.yticks([])
plt.subplot(1,3,2), plt.imshow(cnt      ,'gray'), plt.xticks([]), plt.yticks([])
plt.subplots_adjust(left=0, right=1, top=1, bottom=0,wspace=0, hspace=0)
mng = plt.get_current_fig_manager()
mng.resize(*mng.window.maxsize())
plt.show()
