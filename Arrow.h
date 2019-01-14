#pragma once
#ifndef ARROW_H
#define ARROW_H

#include "AxisSettingTool.h"
#include "vtkCaptionActor2D.h"
#include "VTK_Def.h"
#include <vector>
const int ARROW_SCALE = 10;

//ToothPolyData, interactor, ArrowActor, Matrix 정보를 가지고 있음
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

	//축을 원점으로 변환
	//polydata를 반환해줌
	vtkPolyData*  GetPolydata()const;
	void SetPolydata(vtkPolyData*);

	virtual void MakeArrow(vtkOpenGLRenderer* qtRen);
	virtual void SetToothPolydataAndRenderer(vtkPolyData*, vtkOpenGLRenderer*, vtkGenericOpenGLRenderWindow*);

	//축을 X,Y,Z로 180도 회전
	virtual void InvertAxis(int invertNum);

	//AxisLabel들의 위치를 초기 위치로 돌려놓음
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