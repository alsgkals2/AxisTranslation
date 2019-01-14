#pragma once
#ifndef INTERACTORCAMERA_H
#define INTERACTORCAMERA_H
#include "VTK_Def.h"

//Actor �̿��� �������� �巡���� ��� ��Ÿ�� ������ TrackballCamera
class InteractorCamera : public vtkInteractorStyleTrackballCamera// ChangeTool
{
public:

	static InteractorCamera* New()
	{
		return new InteractorCamera;
	}

	InteractorCamera()
	{
	}

	~InteractorCamera()
	{
	}


	virtual void GiveInteractor(vtkRenderWindowInteractor* renwinin);
	virtual void OnLeftButtonDown(bool);
	virtual void OnLeftButtonDown();
private:

	vtkTypeMacro(InteractorCamera, vtkInteractorStyleTrackballCamera);

};
#endif