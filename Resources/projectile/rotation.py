import cv2
import numpy as np
import sys
def rotation_cv2(image, angle, scale = 1.0):
    (h, w) = image.shape[:2]
    (cX, cY) = (w / 2, h / 2)
    # grab the rotation matrix (applying the negative of the
    # angle to rotate clockwise), then grab the sine and cosine
    # (i.e., the rotation components of the matrix)
    M = cv2.getRotationMatrix2D((cX, cY), -angle, 1.0)
    cos = np.abs(M[0, 0])
    sin = np.abs(M[0, 1])
    # compute the new bounding dimensions of the image
    nW = int((h * sin) + (w * cos))
    nH = int((h * cos) + (w * sin))
    # adjust the rotation matrix to take into account translation
    M[0, 2] += (nW / 2) - cX
    M[1, 2] += (nH / 2) - cY
    return cv2.warpAffine(image, M, (nW, nH), borderValue=(111,11,1))
if __name__ == '__main__':
    file_dir = ''
    file_name = sys.argv[1]
    # file_name = 'ProjectileScythe.bmp'
    image = cv2.imread(file_dir + file_name)
    for i in range(0, 360, 15):
        rotated_cv2 = rotation_cv2(image, i)
        cv2.imwrite(file_dir + file_name.split('.')[0]+'_r' + str(i) + '.' + file_name.split('.')[-1], rotated_cv2)
        