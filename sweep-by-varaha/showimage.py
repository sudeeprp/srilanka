import numpy as np
import cv2
import screeninfo
import time

stoprequested = False

def stop_image():
    global stoprequested
    stoprequested = True

def show_image(filename):
    global stoprequested
    # you asked me to show image... that means stop is not requested
    stoprequested = False

    print(f'showing image {filename}')
    screen_id = 0
    screen = screeninfo.get_monitors()[screen_id]
    width, height = screen.width, screen.height
    image = cv2.imread(filename, cv2.IMREAD_COLOR)
    window_name = 'projector'
    cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)
    cv2.moveWindow(window_name, screen.x - 1, screen.y - 1)
    cv2.setWindowProperty(window_name, cv2.WND_PROP_FULLSCREEN,
                            cv2.WINDOW_FULLSCREEN)
    cv2.imshow(window_name, image)
    while not stoprequested:
        cv2.waitKey(100)
    print(f'stopping image {filename}')
    stoprequested = False
    cv2.destroyWindow('projector')
