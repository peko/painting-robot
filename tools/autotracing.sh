#!/bin/bash
file=test03
autotrace \
-background-color=FFFFFF \
-centerline \
-line-threshold=0 \
-error-threshold=2 \
-output-file=data/${file}_c.pdf \
-report-progress \
data/${file}.tga

# -filter-iterations 20 \
# -despeckle-level=20 \
# -color-count=5 \
# -despeckle-tightness=8 \

# centerline: trace a character's centerline, rather than its outline.


# corner-always-threshold <angle-in-degrees>: if the angle at a pixel is
#   less than this, it is considered a corner, even if it is within
#   `corner-surround' pixels of another corner; default is 60.

# corner-surround <unsigned>: number of pixels on either side of a
#   point to consider when determining if that point is a corner;
#   default is 4.

# corner-threshold <angle-in-degrees>: if a pixel, its predecessor(s),
#   and its successor(s) meet at an angle smaller than this, it's a
#   corner; default is 100.

# despeckle-level <unsigned>: 0..20; default is no despeckling.

# despeckle-tightness <real>: 0.0..8.0; default is 2.0.

# dpi <unsigned>: The dots per inch value in the input image, affects scaling
#   of mif output image

# error-threshold <real>: subdivide fitted curves that are off by
#   more pixels than this; default is 2.0.

# filter-iterations <unsigned>: smooth the curve this many times
#   before fitting; default is 4.

# line-reversion-threshold <real>: if a spline is closer to a straight
#   line than this, weighted by the square of the curve length, keep it a
#   straight line even if it is a list with curves; default is .01.

# line-threshold <real>: if the spline is not more than this far away
#   from the straight line defined by its endpoints,
#   then output a straight line; default is 1.

# preserve-width: whether to preserve line width prior to thinning.
# remove-adjacent-corners: remove corners that are adjacent.

# tangent-surround <unsigned>: number of points on either side of a
#   point to consider when computing the tangent at that point; default is 3.

# report-progress: report tracing status in real time.

# width-weight-factor <real>: weight factor for fitting the linewidth.

# background-color <hexadezimal>: the color of the background that
#   should be ignored, for example FFFFFF;
#   default is no background color.

# color-count <unsigned>: number of colors a color bitmap is reduced to,
#   it does not work on grayscale, allowed are 1..256;
#   default is 0, that means not color reduction is done.

# Supported output formats:
#        eps Encapsulated PostScript
#         ai Adobe Illustrator
#        p2e pstoedit frontend format
#         sk Sketch
#        svg Scalable Vector Graphics
#        fig XFIG 3.2
#        emf Enhanced Metafile format
#        mif FrameMaker MIF format
#         er Elastic Reality Shape file
#        dxf DXF format (without splines)
#        epd EPD format
#        pdf PDF format
#        cgm Computer Graphics Metafile
#       dr2d IFF DR2D format
