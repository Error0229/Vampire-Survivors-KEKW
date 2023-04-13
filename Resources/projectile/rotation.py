import numpy as np
import sys
from PIL import Image


if __name__ == '__main__':
    file_dir = ''
    file_name = sys.argv[1]
    # file_name = 'ProjectileScythe.bmp'
    image = Image.open(file_dir + file_name)

    # Save the rotated image
    for i in range(0, 360, 10):
        rotated_img = image.rotate(i, expand=True, fillcolor=(1,11,111))
        rotated_img.save(file_dir + file_name.split('.')[0]+'_r' + str(i) + '.' + file_name.split('.')[-1])
        