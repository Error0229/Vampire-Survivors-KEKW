# scale up bitmap by nearest neighbor
import os
import cv2
directorys = ['tmp/']

for directory in directorys:
    for filename in os.listdir(directory):
        if filename.endswith(".bmp"):
            img = cv2.imread(os.path.join(directory, filename))
            height, width = img.shape[:2]
            res = cv2.resize(img, (round(width*1.25), round(height*1.25)), interpolation=cv2.INTER_NEAREST)
            cv2.imwrite(os.path.join(directory, filename), res)
            print('Scaled up ' + filename)
            continue
        else:
            continue
