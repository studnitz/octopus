#include "gst_recorder.h"

GstRecorder::GstRecorder(QObject *parent) : QObject(parent)
{

}

QGst::BinPtr GstRecorder::createVideoSrcBin()
{
    QGst::BinPtr videoBin;

    try {
        videoBin = QGst::Bin::fromDescription("v4l2src device=/dev/video0"
                                              "")
    }
}
