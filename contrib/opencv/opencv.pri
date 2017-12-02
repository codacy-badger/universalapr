INCLUDEPATH += $$PWD/include \
               $$PWD/include/opencv \
               $$PWD/include/opencv2 \

LIBS += -L$$PWD/lib \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc \
        -lopencv_objdetect \
        -lopencv_imgcodecs