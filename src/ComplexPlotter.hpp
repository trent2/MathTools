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

#include <complex>
#include <QtCore/QThread>
#include <QtCore/QWaitCondition>

#include "Plotter.hpp"
#include "MathFunction.hpp"

#define MAX_THREADS 16

class QImage;
class QLabel;
class QElapsedTimer;

class ImageRendererThread;
typedef MathFunction<std::complex<double> > MFC_t;

class ComplexPlotter : public Plotter {

  Q_OBJECT


public:

  struct Parameter {
    MFC_t *mF;
    double mRayThickness, mRayOpacity, mCircleThickness, mCircleOpacity;
    int mInfinityThreshold, mNThreads;
    QImage *mImage;
    qreal mUnitCircleColor[3];

    Parameter() :
      mF(0), mRayThickness(1), mRayOpacity(1), mCircleThickness(1), mCircleOpacity(1),
      mInfinityThreshold(1000), mNThreads(10), mImage(0) {
      QColor(Qt::darkRed).getRgbF(&mUnitCircleColor[0], &mUnitCircleColor[1], &mUnitCircleColor[2]);
    }
  };

  ComplexPlotter(QWidget *parent=0);
  ~ComplexPlotter();

  void setRayThickness(double rt) { mP.mRayThickness = rt; }
  void setRayOpacity(double ro) { mP.mRayOpacity = ro; }
  void setCircleThickness(double ct) { mP.mCircleThickness = ct; }
  void setCircleOpacity(double co) { mP.mCircleOpacity = co; }
  void setInfinityThreshold(int it) { mP.mInfinityThreshold = it; }
  void setNumThreads(int th);

  MFC_t& getFunction() const { return *mP.mF; }
  ImageRendererThread* getImageRendererThread(int) const;

  void doRepaint();

  // overwrites abstract declaration from Exportable
  void paintIt(QPaintDevice*, QPainter::RenderHints=0) const { }

protected:
  void paintEvent(QPaintEvent *);

private:
  ComplexPlotter::Parameter mP;
  QLabel *mLabel;
  bool mRepaintEnabled;
  int finishCounter;
  QElapsedTimer *mStopwatch;
  QMutex mMutex;

  void resizeImage();
  void setEnabledThreadStuff(bool b);

  ImageRendererThread **paintThreads;
public slots:
  void checkFinished();
};

class ImageRendererThread : public QThread {

  Q_OBJECT

public:
  ImageRendererThread(int threadNumber, QMutex &m) : QThread(), mThreadNumber(threadNumber), mGMutex(m), mFinish(false) { }
  void render(const ComplexPlotter::Parameter &p, const PlotHelp::cs_params &cs_p, int xmin, int ymin);
  ~ImageRendererThread();
protected:
  void run();
private:
  // ImageRendererThread() { }  // disallow standard constructor

  int mThreadNumber;
  QMutex mMutex;
  QMutex &mGMutex;
  QWaitCondition condition;
  bool mFinish;

  // initialized in render
  ComplexPlotter::Parameter mP;
  PlotHelp::cs_params mCSP;
  int mXmin, mYmin;

signals:
  void rendered();
};

#endif
