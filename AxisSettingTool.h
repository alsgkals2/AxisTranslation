#pragma once
#ifndef AXISSETTING_H
#define AXISSETTING_H

#include "VTK_Def.h"
#include "vtkRotationalExtrusionFilter.h"
#include <vtkOBBTree.h>
#include <vtkPointSource.h>
#include <vtkNamedColors.h>
#include <vtkGlyph3DMapper.h>
#include <vtkSuperquadricSource.h>
#include <vtkKdTreePointLocator.h>
#include <vtkMatrix4x4.h>
#include <vtkInteractorStyleTrackballActor.h>
#include "InteractorActor.h"
#include <vtkAssembly.h>

using namespace std;

const int WRITE = 1;
const int RESET = 2;

class AxisSettingTool : public InteractorActor
{
private:
	bool bSetState;
	bool bActorMouseDown;
	vtkSmartPointer<vtkTransformPolyDataFilter> m_ChangedTransFilter;
	vtkSmartPointer<vtkTransform> readerTrans;
	stack<vtkMatrix4x4*> stUndoMatrix;
	stack<vtkMatrix4x4*> stRedoMatrix;
	vtkSmartPointer<vtkMatrix4x4> mFirst;
	vtkMatrix4x4* restoreMatrix;
	vtkMatrix4x4* cmAxesTrans;
	

public:
	vtkMatrix4x4* acceptMatrix;//치아들 행렬구하기 위하여 임시 추가 1219
	static AxisSettingTool* New()
	{
		return new AxisSettingTool;
	}

	AxisSettingTool()
	{
		currentPolydata = vtkSmartPointer<vtkPolyData>::New();
		m_ChangedTransFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		bSetState = false;
		bActorMouseDown = true;
		mFirst =  vtkSmartPointer<vtkMatrix4x4>::New();
		restoreMatrix = vtkMatrix4x4::New();
		cmAxesTrans = vtkMatrix4x4::New(); ///처음 tooth의 좌표
	}

	//원본 Tooth polydata의 위치 정보를 원점으로 돌리기 위하여 위치정보만 담긴 메트릭스를 setting
	virtual void SetFirstAxisMatrix(vtkMatrix4x4* _mFirst)
	{
		mFirst->DeepCopy(_mFirst);
	}

	//원본 Tooth polydata의 위치를 메트릭스를 이용하여 원점으로 돌림
	virtual void FirstChangeAxesTool(vtkMatrix4x4* firstUndoMatrix, vtkPolyData* polydata);
	virtual void SetAxis()
	{
		bSetState = true;
		ChangeAxesTool(currentMatrix, currentPolydata);

	}

	//XYZ 축에 맞게 Tooth의 메트릭스를 변경시킴
	virtual void ChangeAxesTool(vtkMatrix4x4* matrix, vtkPolyData* polydata);

	virtual bool GetAxisState()
	{
		return bSetState;
	}
	vtkMatrix4x4* GetRestoreMatrix()
	{
		return restoreMatrix;
	}
	virtual void UndoAxis();
	virtual void RedoAxis();

	//param 1 : txt write하는 경우 , 2 : reset하는 경우
	vtkMatrix4x4* GetOriginAxis(int bWrite);

	virtual void ResetAxis();

	virtual const bool GetSetState();
	private:
		virtual void CleanStack(stack<vtkMatrix4x4*>* st);
};
#endif
