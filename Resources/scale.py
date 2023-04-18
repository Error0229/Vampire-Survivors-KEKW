# scale up bitmap by nearest neighbor
import os
import cv2
directory = 'enemy/'
for filename in os.listdir(directory):
    if filename.endswith(".bmp"):
        img = cv2.imread(os.path.join(directory, filename))
        height, width = img.shape[:2]
        res = cv2.resize(img,(width*2, height*2), interpolation = cv2.INTER_NEAREST)
        filename = "2x_" + filename
        cv2.imwrite(os.path.join(directory, filename), res)
        continue
    else:
        continue