#include "recordinggrid.h"
#include <QDebug>

RecordingGrid::RecordingGrid(quint8 width, quint8 height, QObject* parent)
    : QObject(parent) {
  grid.clear();
  height_ = height;
  width_ = width;
  for (int i = 0; i < height; ++i) {
    GridRow row;
    for (int j = 0; j < width; ++j) {
      GridElement entry;
      entry.id = 0;
      entry.hostname = "";
      entry.device = "";
      row.append(entry);
    }
    grid.append(row);
  }
}

void RecordingGrid::updateEntry(quint8 x, quint8 y, GridElement entry) {
  grid[y][x] = entry;
}

void RecordingGrid::addRow() {
  height_ += 1;
  GridRow row;
  for (int j = 0; j < width_; ++j) {
    GridElement entry;
    entry.id = 0;
    entry.hostname = "";
    entry.device = "";
    row.append(entry);
  }
  grid.append(row);
}

void RecordingGrid::addColumn() {
  width_ += 1;
  for (int i = 0; i < height_; ++i) {
    GridElement entry;
    entry.id = 0;
    entry.hostname = "";
    entry.device = "";
    grid[i].append(entry);
  }
}

void RecordingGrid::removeRow() {
  height_ -= 1;
  grid.removeLast();
}

void RecordingGrid::removeColumn() {
  width_ -= 1;
  for (int i = 0; i < height_; ++i) {
    grid[i].removeLast();
  }
}
