#include "recording.h"

Recording::Recording(QObject *parent) : QObject(parent) {}

Recording::Recording(QDateTime datetime, Grid grid)
    : datetime(datetime), grid(grid) {}
