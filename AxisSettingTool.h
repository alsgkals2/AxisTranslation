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
	vtkMatrix4x4* acceptMatrix;//ġ�Ƶ� ��ı��ϱ� ���Ͽ� �ӽ� �߰� 1219
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
		cmAxesTrans = vtkMatrix4x4::New(); ///ó�� tooth�� ��ǥ
	}

	//���� Tooth polydata�� ��ġ ������ �������� ������ ���Ͽ� ��ġ������ ��� ��Ʈ������ setting
	virtual void SetFirstAxisMatrix(vtkMatrix4x4* _mFirst)
	{
		mFirst->DeepCopy(_mFirst);
	}

	//���� Tooth polydata�� ��ġ�� ��Ʈ������ �̿��Ͽ� �������� ����
	virtual void FirstChangeAxesTool(vtkMatrix4x4* firstUndoMatrix, vtkPolyData* polydata);
	virtual void SetAxis()
	{
		bSetState = true;
		ChangeAxesTool(currentMatrix, currentPolydata);

	}

	//XYZ �࿡ �°� Tooth�� ��Ʈ������ �����Ŵ
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

	//param 1 : txt write�ϴ� ��� , 2 : reset�ϴ� ���
	vtkMatrix4x4* GetOriginAxis(int bWrite);

	virtual void ResetAxis();

	virtual const bool GetSetState();
	private:
		virtual void CleanStack(stack<vtkMatrix4x4*>* st);
};
#endif
