# scale up bitmap by nearest neighbor
import os
import cv2
directorys = ['chars_bmp/']

for directory in directorys:
    for filename in os.listdir(directory):
        if filename.endswith("_M.bmp"):
            img = cv2.imread(os.path.join(directory, filename))
            height, width = img.shape[:2]
            res = cv2.resize(img, (round(width//12.8), round(height//12.8)), interpolation=cv2.INTER_NEAREST)
            cv2.imwrite(os.path.join(directory, filename), res)
            print('Scaled up ' + filename)
            continue
        else:
            continue
