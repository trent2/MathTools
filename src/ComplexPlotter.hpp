/*  MathTools 
    Copyright (C) 2010, 2011, 2012 Robert Spillner <Robert.Spillner@gmx.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*  file: --- ComplexPlotter.hpp --- */

#ifndef _COMPLEX_PLOTTER_HPP_
#define _COMPLEX_PLOTTER_HPP_

#define PAINT_THREAD_COUNT 4

#include <complex>
#include <QtCore/QThread>
#include <QtCore/QMutex>

#include "Plotter.hpp"
#include "MathFunction.hpp"

class QImage;
class QLabel;
class QElapsedTimer;

class ComplexPlotter : public Plotter {

  Q_OBJECT

  class ImagePainterThread;
public:
  ComplexPlotter(QWidget *parent=0);
  ~ComplexPlotter();

  void setRayThickness(double rt) { mRayThickness = rt; }
  void setRayOpacity(double ro) { mRayOpacity = ro; }
  void setCircleThickness(double ct) { mCircleThickness = ct; }
  void setCircleOpacity(double co) { mCircleOpacity = co; }
  void setInfinityThreshold(int it) { mInfinityThreshold = it; }

  MathFunction<std::complex<double> >& getFunction() const { return *mF; }
  ComplexPlotter::ImagePainterThread* getImagePainterThread(int i) { return paintThreads[i]; }

  void doRepaint();

  // overwrites abstract declaration from Exportable
  void paintIt(QPaintDevice*, QPainter::RenderHints=0) const { }

protected:
  void paintEvent(QPaintEvent *);

private:
  double mRayThickness, mRayOpacity, mCircleThickness, mCircleOpacity;
  int mInfinityThreshold;

  QImage *mImage;
  QLabel *mLabel;
  qreal mUnitCircleColor[3];
  bool mRepaintEnabled;
  MathFunction<std::complex<double> > *mF;
  int finishCounter;
  QMutex *mMutex;
  QElapsedTimer *mStopwatch;

  friend class ComplexPlotter::ImagePainterThread;

  class ImagePainterThread : public QThread {
  public:
    ImagePainterThread(ComplexPlotter *p, int threadNumber, QMutex *mutex) :
      mPlotter(p), mMux(mutex), mThreadNumber(threadNumber) { }
    void run();
  private:
    ComplexPlotter *mPlotter;
    QMutex *mMux;
    int mThreadNumber;
    inline void setPixel(int x, int y, QRgb c) { mPlotter->mImage->setPixel(x, y, c); }
  } *paintThreads[PAINT_THREAD_COUNT];

  void resizeImage();
  void setEnabledRepaintButton(bool b);

public slots:
  void checkFinished();
};
#endif
