#pragma once
#ifndef  INTERACTORCHANGE_H
#define INTERACTORCHANGE_H

#include "VTK_Def.h"
#include "AxisSettingTool.h"

using namespace std;
enum CurrentInteractorStyle
{
	NONE,
	TRACKBALL_CAMERA,
	TRACKBALL_ACTOR,
	COLOR_ON,
	COLOR_OFF
};

class InteractorChange
{
private:
	CurrentInteractorStyle currentStyleMode;
	vtkInteractorObserver* style;

public:
	 vtkRenderWindowInteractor* renWinIn;
	 InteractorChange() :
		 style(nullptr),
		 currentStyleMode(NONE)
	{}
	void SetCurrentStyleMode(CurrentInteractorStyle style)
	{
		currentStyleMode = style;
	}

	CurrentInteractorStyle GetCurrentStyleMode()
	{
		return currentStyleMode;
	}

	void ChangeStyle(vtkInteractorObserver* style)
	{
		this->style = style;
		renWinIn->SetInteractorStyle(style);
		/*
		switch (currentStyleMode)
		{
		case TRACKBALL_CAMERA:
			
			break;

		case TRACKBALL_ACTOR:
			

			break;
		case COLOR_ON:
			

			break;
		case COLOR_OFF:


			break;
		default:
			break;
		}
		*/
	}

	 void GiveInteractor(vtkRenderWindowInteractor* renwinin)
	{
		this->renWinIn = renwinin;
	}

	void ChangeInteractor()
	{}
};

#endif // ! INTERACTORCHANGE_H
