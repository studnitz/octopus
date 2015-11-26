gst-launch-1.0 -v -e v4l2src device=/dev/video0 \
! videoscale \
! videorate \
! videoconvert \
! 'video/x-raw,width=1280,height=720,framerate=30/1' \
! progressreport name=progess \
! omxh264enc \
! 'video/x-h264,profile=high,control-rate=variable' \
! h264parse \
! matroskamux \
! filesink location='meeting.mkv'
