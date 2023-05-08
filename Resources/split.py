# pip install Pillow
from PIL import Image, ImageFont, ImageDraw

# use a truetype font (.ttf)
# font file from fonts.google.com (https://fonts.google.com/specimen/Courier+Prime?query=courier)
font_path = ""
font_name = "CourierPrime-Regular.ttf"
out_path = "chars_bmp/"

font_size = 256  # px
font_color = "#FFFFFF"  # HEX Black

# Create Font using PIL
font = ImageFont.truetype(font_path+font_name, font_size)

# Copy Desired Characters from Google Fonts Page and Paste into variable
desired_characters = "0123456789: LV"

# Loop through the characters needed and save to desired location
for character in desired_characters:

    # Get text size of character
    width, height = font.getsize(character)

    # Create PNG Image with that size
    img = Image.new("RGB", (width, height), color="#010B6F")
    draw = ImageDraw.Draw(img)

    # Draw the character
    draw.text((0, 0), str(character), font=font, fill=font_color, align="center")

    # Save the character as png
    try:
        img.save(out_path + str(ord(character)) + "_M.bmp")
    except:

        print(f"[-] Couldn't Save:\t{character}")
