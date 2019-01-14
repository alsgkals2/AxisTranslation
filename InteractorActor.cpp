#pragma once

#include "InteractorActor.h"
#include "InteractorCamera.h"
#include "InteractorChange.h"

using namespace std;

//std::vector<vtkSmartPointer<vtkFollower>> vtkPositionCallback::conLabels = { nullptr,nullptr,nullptr };

vtkSmartPointer<vtkFollower> vtkPositionCallback::xLabel = nullptr;
vtkSmartPointer<vtkFollower> vtkPositionCallback::yLabel = nullptr;
vtkSmartPointer<vtkFollower> vtkPositionCallback::zLabel = nullptr;
vtkSmartPointer<vtkAssembly> vtkPositionCallback::Axes = nullptr;
vtkPositionCallback* InteractorActor::callback = nullptr;


void InteractorActor::OnLeftButtonDown()
{
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	static InteractorChange* changer = new InteractorChange();
	static InteractorCamera* camera = InteractorCamera::New();

	this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1]);

	if (this->InteractionPicker->Pick(x, y, 0, this->CurrentRenderer))
		bActorMouseDown = true;
	else
	{
		changer->GiveInteractor(this->Interactor);
		changer->ChangeStyle(camera);
		camera->OnLeftButtonDown(0);
		return;
	}
	vtkInteractorStyleTrackballActor::OnLeftButtonDown();
}

void InteractorActor::SetInformation(vtkPolyData* polydata,
	vtkMatrix4x4* matrix,
	vtkRenderWindowInteractor* renwinin)
{
	this->currentPolydata = polydata;
	this->currentMatrix = matrix;
	this->renwinin = renwinin;
}

void InteractorActor::GetInformation(vtkPolyData* polydata,
	vtkMatrix4x4* matrix,
	vtkRenderWindowInteractor* renwinin)
{
	polydata = this->currentPolydata;
	matrix = this->currentMatrix;
	renwinin = this->renwinin;
}