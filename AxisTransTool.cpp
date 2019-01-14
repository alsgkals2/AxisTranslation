#pragma once
#include "AxisTransTool.h"
#include "qfiledialog.h"
#include "InteractorChange.h"
#include "qdir.h"

AxisTransTool::AxisTransTool(QWidget *parent)
	: QMainWindow(parent)
	, numOfFile("11")
	, m_strLastExportFilePath("D:\\")
	, m_strLastImportFilePath("D:\\")
{
	this->setGeometry(200, 100, 1000, 1000);
	this->setFixedSize(1200, 900);
	this->setWindowTitle(" Transform Tool");
	arrow = new Arrow();

	ui.setupUi(this);
	
	renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();

	vtkWidget = new QVTKOpenGLWidget(this);
	vtkWidget->setGeometry(0, 50, width(), height());
	vtkWidget->SetRenderWindow(renderWindow);
	//button setting
	setButton = new QPushButton("Set", this);
	setButton->setGeometry(QRect(QPoint(20, 70), QSize(80, 30)));
	connect(setButton, SIGNAL(clicked()), this, SLOT(SetButton()));

	xInvertButton = new QPushButton("X Axis", this);
	xInvertButton->setGeometry(QRect(QPoint(20, 100), QSize(80, 30)));
	connect(xInvertButton, SIGNAL(released()), this, SLOT(InvertX()));

	yInvertButton = new QPushButton("Y Axis", this);
	yInvertButton->setGeometry(QRect(QPoint(20, 130), QSize(80, 30)));
	connect(yInvertButton, SIGNAL(released()), this, SLOT(InvertY()));

	zInvertButton = new QPushButton("Z Axis", this);
	zInvertButton->setGeometry(QRect(QPoint(20, 160), QSize(80, 30)));
	connect(zInvertButton, SIGNAL(released()), this, SLOT(InvertZ()));

	ImportButton = new QPushButton("Import STL", this);
	ImportButton->setGeometry(QRect(QPoint(5, 15), QSize(80, 30)));

	connect(ImportButton, SIGNAL(released()), this, SLOT(ImportSTL()));

	ExportButton = new QPushButton("Export STL", this);
	ExportButton->setGeometry(QRect(QPoint(90, 15), QSize(80, 30)));

	connect(ExportButton, SIGNAL(released()), this, SLOT(ExportSTL()));
	//connect(ColorRemoveButton, SIGNAL(released()), this, SLOT(EraseColor()));
	Reset = new QPushButton("Reset", this);
	Reset->setGeometry(QRect(QPoint(930, 15), QSize(80, 30)));

	connect(Reset, SIGNAL(released()), this, SLOT(ResetButton()));

	Undo = new QPushButton("Undo", this);
	Undo->setGeometry(QRect(QPoint(1015, 15), QSize(80, 30)));

	connect(Undo, SIGNAL(released()), this, SLOT(UndoButton()));

	Redo = new QPushButton("Redo", this);
	Redo->setGeometry(QRect(QPoint(1100, 15), QSize(80, 30)));

	connect(Redo, SIGNAL(released()), this, SLOT(RedoButton()));

	m_pVTKRenderer = vtkOpenGLRenderer::New();
	renderWindow->AddRenderer(m_pVTKRenderer);
	renderWindow->Modified();
	m_pVTKRenderer->SetBackground((193 / 255.0), 200 / 255.0, 206 / 255.0);
	changer = new InteractorChange();

	importedPolydata = nullptr;
	exportedPolydata = nullptr;
	etcTool = new EtcTool();
}

AxisTransTool::~AxisTransTool()
{
	delete etcTool;
}
void AxisTransTool::ExportSTL()
{
#ifdef onlyTransformFilterApply
	auto importer = vtkSmartPointer<vtkSTLReader>::New();
	importer->SetFileName("D:\\36.stl");
	importer->Update();
	auto importedPoly = importer->GetOutput();

	auto trans = vtkSmartPointer<vtkTransform>::New();
	trans->Identity();

	auto filter = vtkSmartPointer<vtkTransformFilter>::New();
	filter->SetInputData(importedPoly);
	filter->SetTransform(trans);
	filter->Update();

	auto exportedPoly = filter->GetOutput();

	auto exporter = vtkSmartPointer<vtkSTLWriter>::New();
	exporter->SetFileName("D:\\36result.stl");
	exporter->SetInputData(exportedPoly);
	exporter->SetFileTypeToBinary();
	exporter->Write();
#endif
	//set버튼을 한번도 누르지 않은 상태에서는 export 막음

	if (!arrow->style) return;
	if (arrow->style->GetAxisState() == false) return;

	QFileDialog::Options options;
	options |= QFileDialog::DontUseNativeDialog;

	QString _strFileName = QFileDialog::getSaveFileName(this, tr("Save STL file"), m_strLastExportFilePath, tr("STL Files (*.stl)"));
	QFileInfo info(_strFileName);
	if (_strFileName.size() == 0) return;
	else
	{
		m_strLastExportFilePath = _strFileName;
	}
	m_strLastExportFilePath = info.absolutePath();

	QFile file_(m_strLastExportFilePath);
	file_.open(QIODevice::ReadOnly);
	exportedPolydata = arrow->GetPolydata();
	etcTool->STLWriter(exportedPolydata, _strFileName);
	vtkMatrix4x4* mOriginMatrix = arrow->style->GetOriginAxis(WRITE);
	etcTool->WriteArrayText(mOriginMatrix, _strFileName);

#ifdef temp1220
	QString strAcceptMatrix = "D:\\Toothmatrix\\";
	strAcceptMatrix.append(numOfFile);
	etcTool->WriteArrayText(arrow->style->acceptMatrix, strAcceptMatrix);

	QString::iterator iter = numOfFile.end();
	if (QString::compare(*iter, QString("8")))
	{
		//*iter->
		//numOfFile.replace("8", "1");
		int nextNum = stoi(numOfFile.toUtf8().toStdString());
		nextNum++;
		numOfFile.clear();

		numOfFile.append(QString::fromStdString(to_string(nextNum)));
		ImportSTL();

	}
#endif
}

void AxisTransTool::ImportSTL()
{
	QFileDialog::Options options;
	options |= QFileDialog::DontUseNativeDialog;

	QString _strFileName;
	_strFileName = QFileDialog::getOpenFileName(this, tr("Open STL file"), m_strLastImportFilePath, tr("STL Files (*.stl)"));
	QFileInfo info(_strFileName);
	if (_strFileName.size() == 0) return;
	else
	{
		m_strCurrentFileName = _strFileName;
	}
	m_strLastImportFilePath = info.absolutePath();
	//QString fileName = strPath;

	QFile file_(m_strCurrentFileName);
	file_.open(QIODevice::ReadOnly);

	etcTool = new EtcTool();

	QString::iterator iter;
	for (iter = m_strCurrentFileName.end(); iter != m_strCurrentFileName.begin(); iter--)
		if (*iter == ".")break;

	//1220 임시 주석
	numOfFile.clear();
	numOfFile = *(iter - 2);
	numOfFile.append(*(iter - 1));

	importedPolydata = vtkSmartPointer<vtkPolyData>::New();
	etcTool->STLReader(importedPolydata, m_strCurrentFileName);

	arrow->MakeArrow(m_pVTKRenderer);
	arrow->SetToothPolydataAndRenderer(importedPolydata, m_pVTKRenderer, renderWindow);
	importedPolydata->Modified();
	m_pVTKRenderer->Modified();
	renderWindow->Modified();
}
void AxisTransTool::SetButton()
{
	//style이 axissetting에 정보를 넘겨준다.
	if (importedPolydata == nullptr) return;

	arrow->style->SetAxis();
	arrow->InitAixsLabels();
	m_pVTKRenderer->Render();
	renderWindow->Render();
}
void AxisTransTool::InvertX()
{
	eInvertMode = INVERT_X;
	if (importedPolydata == nullptr) return;
	arrow->InvertAxis(eInvertMode);
	//to do invert 할 경우 label 따라가도록
	m_pVTKRenderer->Render();
	renderWindow->Render();
}
void AxisTransTool::InvertY()
{
	eInvertMode = INVERT_Y;
	if (importedPolydata == nullptr) return;
	arrow->InvertAxis(eInvertMode);
	m_pVTKRenderer->Render();
	renderWindow->Render();
}
void AxisTransTool::InvertZ()
{
	eInvertMode = INVERT_Z;
	if (importedPolydata == nullptr) return;
	arrow->InvertAxis(eInvertMode);
	m_pVTKRenderer->Render();
	renderWindow->Render();
}
void AxisTransTool::ResetButton()
{
	if (importedPolydata == nullptr) return;

	importedPolydata = vtkSmartPointer<vtkPolyData>::New();
	etcTool->STLReader(importedPolydata, m_strCurrentFileName);

	arrow->MakeArrow(m_pVTKRenderer);
	arrow->SetToothPolydataAndRenderer(importedPolydata, m_pVTKRenderer, renderWindow);
	importedPolydata->Modified();
	m_pVTKRenderer->Modified();
	renderWindow->Modified();


	//arrow->style->ResetAxis();
	//m_pVTKRenderer->Render();
	//renderWindow->Render();
}
void AxisTransTool::UndoButton()
{
	if (importedPolydata == nullptr) return;
	arrow->style->UndoAxis();
	m_pVTKRenderer->Render();
	renderWindow->Render();
}

void AxisTransTool::RedoButton()
{
	if (importedPolydata == nullptr) return;
	arrow->style->RedoAxis();
	m_pVTKRenderer->Render();
	renderWindow->Render();
}