#pragma once
#include "InteractorCamera.h"
#include "InteractorActor.h"
#include "InteractorChange.h"

void InteractorCamera::GiveInteractor(vtkRenderWindowInteractor* renwinin)
{
	static InteractorChange* changer = new InteractorChange();
	changer->GiveInteractor(renwinin);
}

 void InteractorCamera::OnLeftButtonDown(bool)
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1]);
	vtkPicker* picker = vtkPicker::New();

	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}

 void InteractorCamera::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	static InteractorChange* changer = new InteractorChange();
	static InteractorActor* actorStyle = new InteractorActor();

	vtkPicker* picker = vtkPicker::New();

	//style을 trackball actor의 axistool로 전환
	if (picker->Pick(x, y, 0, this->CurrentRenderer))
	{
		changer->GiveInteractor(this->Interactor);
		changer->ChangeStyle(actorStyle);
		actorStyle->OnLeftButtonDown();

		return;
	}
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}
