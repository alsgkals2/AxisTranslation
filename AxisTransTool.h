#pragma once
#ifndef TEST_H
#define TEST_H
#include <QtWidgets/QMainWindow>
#include <qwindow.h>
#include <qwidget.h>
#include "VTK_Def.h"
#include <qpushbutton.h>
#include "ui_AxisTransTool.h"
#include "EtcTool.h"
#include "Arrow.h"
#include "InteractorChange.h"
using namespace std;

enum Invert { INVERT_X, INVERT_Y, INVERT_Z };
class AxisTransTool : public QMainWindow
{
	Q_OBJECT

public:
	AxisTransTool(QWidget *parent = Q_NULLPTR);
	~AxisTransTool();
	//Test();
	private slots:
	virtual void ResetButton();
	virtual void UndoButton();
	virtual void RedoButton();
	virtual void SetButton();
	virtual void ExportSTL();
	virtual void ImportSTL();
	void InvertX();
	void InvertY();
	void InvertZ();


private:
	Ui::AxisTransToolClass ui;
	vtkOpenGLRenderer* m_pVTKRenderer;
	vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

	QVTKOpenGLWidget* vtkWidget;
	QPushButton *setButton;
	QPushButton *xInvertButton;
	QPushButton *yInvertButton;
	QPushButton *zInvertButton;
	QPushButton *ImportButton;
	QPushButton *ExportButton;
	QPushButton *ColorOnButton;
	QPushButton *ColorOffButton;
	QPushButton *ColorRemoveButton;
	QPushButton *Reset;
	QPushButton *Undo;
	QPushButton *Redo;

	vtkSmartPointer<vtkPolyData> importedPolydata;
	vtkSmartPointer<vtkPolyData> exportedPolydata;

	vtkSmartPointer<vtkActor> actorOrigin;
	Arrow* arrow;
	InteractorChange* changer;
	EtcTool* etcTool;
	Invert eInvertMode;

	QString m_strLastExportFilePath;
	QString m_strLastImportFilePath;
	QString m_strCurrentFileName;
	QString numOfFile;///파일 tooth 번호
};
#endif