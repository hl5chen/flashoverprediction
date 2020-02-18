# Fire detection test script based on https://pdfs.semanticscholar.org/e0d0/daa8c39dcecac9e3bb8afc4384d7534c93f1.pdf

# Brightest Spot Test pyimagesearch.com

import numpy as np
import sys
sys.path.remove('/opt/ros/kinetic/lib/python2.7/dist-packages')
import cv2
from matplotlib import pyplot as plt

# Default args
ir_img_name = "example.png"
vr_img_name = "vrexample.png"
img_dir = "data_ir/"
white_px_threshold = 200

# Parse arguments
args = sys.argv[1:]
if len(args) > 0:
    img_name = args[0]

#vr img convert to grayscale
vr_image = cv2.imread(img_dir + vr_img_name)
grey_img = cv2.cvtColor(vr_image, cv2.COLOR_BGR2GRAY)

#ir img convert to RGB
ir_image = cv2.imread(img_dir + ir_img_name)
bgr_img = cv2.cvtColor(ir_image, cv2.COLOR_BGR2RGB)

#minMaxLoc METHOD
#bright_image = original_image.copy()
bright_image = cv2.cvtColor(bgr_img, cv2.COLOR_BGR2GRAY)
bright_image = cv2.GaussianBlur(bright_image, (5,5),0)
(minVal,maxVal,minLoc,maxLoc) = cv2.minMaxLoc(bright_image)
cv2.circle(bright_image, maxLoc, 50, (0,255,0),3)

#Kmeans (Color Quantization:Process of reducing number of colours in an image)

#1. Reshape the image to array into RGB
#2. Clustering 
#3. Apply centroid values to all pizels 
#Reshape back to original image 

Z = bgr_img.reshape((-1,3))

#convert to np.float 32
Z = np.float32(Z)
XY=np.indices((bgr_img.shape[0],bgr_img.shape[1]))

#define criteria 
criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 10, 1.0)
K = 8 
attempts = 10 
ret,label,center = cv2.kmeans(Z, K,criteria, attempts,cv2.KMEANS_PP_CENTERS )
center = np.uint8(center)
res = center[label.flatten()]
res2 = res.reshape((bgr_img.shape))

#Create masks for filtering images based on each channel




# Experimental masks
# Super white pixels
#R5 = np.where(Lab_img[:,:,0] >= white_px_threshold, 1, 0)

#fire_mask = R1 & R2 & R3 & R4 | R5
#masked_image = rgb_img * np.repeat(fire_mask.reshape(fire_mask.shape[0], #fire_mask.shape[1], 1), 3, axis=2)
#cv2.imwrite(img_dir + img_name.split('.')[0] + '_masked.' + img_name.split('.')[1], 
#cv2.cvtColor(masked_image.astype(np.float32), cv2.COLOR_RGB2BGR))



figure_size = 15
plt.figure(figsize=(figure_size,figure_size))
plt.subplot(2,4,1),plt.imshow(grey_img)
plt.title('Gray VR Original Image'), plt.xticks([]), plt.yticks([])
plt.subplot(2,4,2),plt.imshow(rgb_img)
plt.title('RGB IR Image'), plt.xticks([]), plt.yticks([])
plt.subplot(2,4,3),plt.imshow(res2)
plt.title('try')
plt.subplot(2,4,4),plt.imshow(gry_image)
plt.title('Method by minMaxLoc')
plt.subplot(2,4,5),plt.imshow(res2)
#plt.title('R4 (b > a)')
#plt.subplot(2,4,6),plt.imshow(R5)
#plt.title('R5')
#plt.subplot(2,4,7),plt.imshow(masked_image)
#plt.title('Extracted Fire Pixels')
plt.show()
