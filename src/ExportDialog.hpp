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

/*  file: --- ExportDialog.hpp --- */

#ifndef _EXPORT_DIALOG_HPP_
#define _EXPORT_DIALOG_HPP_
#include "ui_exportdialog.h"

class Exportable;

class ExportDialog : public QDialog, public Ui::ExportDialog {

Q_OBJECT

public:
  enum OutputFormat {pdf, png};
  enum Measure {cm, in};
  enum ResolutionRatio {in_px, cm_px};

  ExportDialog(QWidget* parent, Exportable *expo, double width, double height,
	       OutputFormat format = pdf, Measure m = cm, ResolutionRatio rr = in_px);

private:
  Exportable *mExp;
  double mWidth, mHeight;
  Measure mMeas;
  ResolutionRatio mResType;
  double mRes;

  static const int minInchResRatio;
  static const int maxInchResRatio;

  static const int minMMSize;
  static const int maxMMSize;

private slots:
  void on_buttonBox_accepted();
};
#endif
