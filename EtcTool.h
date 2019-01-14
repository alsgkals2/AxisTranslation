#pragma once
#define SET_CIRCLEPLANE
#include "VTK_Def.h"
#include "vtkPolyData.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkCleanPolyData.h"
#include "vtkSTLWriter.h"
#include "vtkSTLReader.h"
#include "qfiledialog.h"
#include "vtkRegularPolygonSource.h"
#include "vtkArrayWriter.h"
#include "vtkArrayData.h"
#include "vtkDenseArray.h"
#include "vtkArrayReader.h"

using namespace std;
class EtcTool
{
public:
	EtcTool() {}
	vtkSmartPointer<vtkSTLReader> sourceOrigin;

	void EtcTool::STLReader(vtkPolyData* importedPolydata, QString inputFilename)
	{
		sourceOrigin = vtkSmartPointer<vtkSTLReader>::New();


		sourceOrigin->SetFileName(inputFilename.toLocal8Bit().toStdString().c_str());
		//sourceOrigin->SetFileName(inputFilename.toLocal8Bit().toStdString().c_str());
		sourceOrigin->Update();

		importedPolydata->DeepCopy(sourceOrigin->GetOutput());
	}

	virtual void STLWriter(vtkPolyData* polydata, QString inputFilename)
	{
		////////////////////////////stl writer
		vtkSmartPointer<vtkSTLWriter> stlWriter =

			vtkSmartPointer<vtkSTLWriter>::New();
		QString _inputFilename = inputFilename;
		//_inputFilename += ".stl";
		const char* cFilename = _inputFilename.toUtf8().constData();
		char str1[1024] = { 0, };
		qsnprintf(str1, sizeof(str1), "%s", _inputFilename.toLocal8Bit().toStdString().c_str());
		stlWriter->SetFileName(str1);

		stlWriter->SetInputData(polydata);
		stlWriter->SetFileTypeToBinary();
		stlWriter->Write();
	}

	virtual void WriteArrayText(vtkMatrix4x4* _restoreMatrix, QString inputFilename)
	{
		if (!_restoreMatrix) return;
		
		vtkDenseArray<double>* matrix = vtkDenseArray<double>::New();
		matrix->Resize(4, 4);
		matrix->Fill(0.0);

		for (int n = 0; n != matrix->GetNonNullSize(); n++)
			matrix->SetValueN(n, _restoreMatrix->GetElement(n/4,n%4));

		vtkSmartPointer<vtkArrayData> arrayData =
			vtkSmartPointer<vtkArrayData>::New();
		arrayData->AddArray(matrix);

		vtkArrayWriter* aWriter = vtkArrayWriter::New();
		QString _inputFilename = inputFilename;
		_inputFilename.replace(QString(".stl"), QString(".txt"));

		//_inputFilename += ".txt";
		vtkStdString stdstring;
		stdstring = _inputFilename.toLocal8Bit().toStdString().c_str();
		aWriter->Write(matrix, stdstring);
	}

};