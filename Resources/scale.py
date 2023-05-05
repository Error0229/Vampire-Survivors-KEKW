# scale up bitmap by nearest neighbor
import os
import cv2
directorys = ['ui/']

for directory in directorys:
    for filename in os.listdir(directory):
        if filename.endswith("_b.bmp"):
            img = cv2.imread(os.path.join(directory, filename))
            height, width = img.shape[:2]
            res = cv2.resize(img, (round(width*0.333), round(height*0.333)), interpolation=cv2.INTER_NEAREST)
            cv2.imwrite(os.path.join(directory, "s_" + filename), res)
            print('Scaled up ' + filename)
            continue
        else:
            continue
