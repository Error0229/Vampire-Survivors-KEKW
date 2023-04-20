# change a directory's bmp files to mirrored image
import os
import cv2
directorys = ['enemy/', 'character/']

for directory in directorys:
    for filename in os.listdir(directory):
        if filename.endswith(".bmp"):
            img = cv2.imread(os.path.join(directory, filename))
            res = cv2.flip(img, 1)
            mirror_filename = filename.split('.')[0] + '_m.bmp'
            cv2.imwrite(os.path.join(directory, mirror_filename), res)
            print('Mirrored ' + mirror_filename)
            continue
        else:
            continue