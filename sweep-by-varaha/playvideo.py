# https://stackoverflow.com/questions/49949639/fullscreen-a-video-on-opencv

import cv2
from showimage import show_image

def play_video(file_name, hold_image_name=None):
    window_name = file_name
    interframe_wait_ms = 30

    cap = cv2.VideoCapture(file_name)
    if not cap.isOpened():
        print("Error: Could not open video.")

    cv2.namedWindow(window_name, cv2.WND_PROP_FULLSCREEN)
    cv2.setWindowProperty(window_name, cv2.WND_PROP_FULLSCREEN, cv2.WINDOW_FULLSCREEN)
    # This will prevent ANY other window from being seen!! Hence removed.
    # cv2.setWindowProperty(window_name, cv2.WND_PROP_TOPMOST, 1)

    while (True):
        ret, frame = cap.read()
        if not ret:
            print("Reached end of video, exiting.")
            break

        cv2.imshow(window_name, frame)
        if cv2.waitKey(interframe_wait_ms) & 0x7F == ord('q'):
            print("Exit requested.")
            break

    cap.release()
    cv2.destroyWindow(window_name)
    if hold_image_name != None:
        show_image(hold_image_name)
