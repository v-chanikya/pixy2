# FFmpeg guide

### Using raw rgb images to create a video
`ffmpeg -f image2 -r 30 -s 316x208 -pix_fmt rgb24 -vcodec rawvideo -i out_%03d.rgb -vcodec huffyuv video.avi`

### Using raw bayer images to create a video
`ffmpeg -f image2 -r 30 -s 316x208 -pix_fmt bayer_bggr8 -vcodec rawvideo -i out_%03d.bayer -vcodec huffyuv video.avi`

### UDP stream transmission
`ffmpeg -i reliance_jio_tv_ad_7-HKKlnVTZ4_360p.mp4 -v 0 -vcodec mpeg4 -f mpegts udp://127.0.0.1:23000`

### UDP stream reception ffplay
`ffplay udp://127.0.0.1:23000`

### UDP stream local playback using VLC
`udp://@:23000`
