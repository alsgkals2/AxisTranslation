#pragma once
#ifndef ARROW_H
#define ARROW_H

#include "AxisSettingTool.h"
#include "vtkCaptionActor2D.h"
#include "VTK_Def.h"
#include <vector>
const int ARROW_SCALE = 10;

//ToothPolyData, interactor, ArrowActor, Matrix ������ ������ ����
class Arrow
{
public:
	vtkSmartPointer<AxisSettingTool> style;
	vtkSmartPointer<vtkPolyData> appendPolydata;
	vtkSmartPointer<vtkPolyData> changedPolydata;
	vtkMatrix4x4* arrowMatrix;
	vtkSmartPointer<vtkRenderWindowInteractor> renWinIn;
	vtkSmartPointer<vtkActor> toothActor;

	static vector<vtkSmartPointer<vtkFollower>> m_conLabels;
	static vtkSmartPointer<vtkAssembly> movableAxes;

	Arrow()
	{
		changedPolydata = vtkSmartPointer<vtkPolyData>::New();
		arrowMatrix = vtkMatrix4x4::New();
		renWinIn = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		toothActor = vtkSmartPointer<vtkActor>::New();
	}

	//���� �������� ��ȯ
	//polydata�� ��ȯ����
	vtkPolyData*  GetPolydata()const;
	void SetPolydata(vtkPolyData*);

	virtual void MakeArrow(vtkOpenGLRenderer* qtRen);
	virtual void SetToothPolydataAndRenderer(vtkPolyData*, vtkOpenGLRenderer*, vtkGenericOpenGLRenderWindow*);

	//���� X,Y,Z�� 180�� ȸ��
	virtual void InvertAxis(int invertNum);

	//AxisLabel���� ��ġ�� �ʱ� ��ġ�� ��������
	void InitAixsLabels();

	void SetToothPickableAndDragable(bool bPickable=true, bool bPragable=true)
	{
		toothActor->SetPickable(bPickable);
		toothActor->SetDragable(bPragable);
		toothActor->Modified();
	}

	void SetTrackballActorStyle()
	{
		renWinIn->SetInteractorStyle(style);
	}

	void SetStyle(vtkInteractorObserver* style)
	{
		renWinIn->SetInteractorStyle(style);
	}
private:
};

#endif