======================
Simple Image Viewer
======================

Simple Image viewer is a simple image viewer for the TI Nspire CX and the GCW0.
\\
The current alternative, mViewerCX, is enormous and has a huge, complex codebase built upon legacy code.


In comparaison, it uses smaller alternatives like LodePNG, a modified version of Tiny JPEG Decoder,
\\
and the image viewer itself is no more than 300 lines of code.


There are two screen mode, trigerred by the DEL button : Scaling mode and the cropping mode.
\\
The cropping mode is very fast but you can't zoom in or out.


The scaling mode is much slower, since it is not cropped,
\\
but it allows you to zoom in/out the picture.
\\
The further you zoom in, the slower it gets though. (as of now)

=======
Controls
=======

+/- or Ctrl/Shift = Zoom In/Out
\\
DEL = Switch screen modes
\\
Touchpad = Move around
\\
ESC = Quit the app

