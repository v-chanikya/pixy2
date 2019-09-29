# FFmpeg guide

### Using raw rgb images to create a video
`ffmpeg -f image2 -r 30 -s 316x208 -pix_fmt rgb24 -vcodec rawvideo -i out_%03d.rgb -vcodec huffyuv video.avi`

### Using raw bayer images to create a video
`ffmpeg -f image2 -r 30 -s 316x208 -pix_fmt bayer_bggr8 -vcodec rawvideo -i out_%03d.bayer -vcodec huffyuv video.avi`
