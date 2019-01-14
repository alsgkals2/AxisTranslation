#pragma once
#ifndef InteractorActor_H
#define InteractorActor_H
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
#include "vtkAssembly.h"
#include "vtkAssemblyPath.h"
#include <vector>
class vtkPositionCallback : public vtkCommand
{
public:

	static vtkPositionCallback *New()
	{
		return new vtkPositionCallback;
	}
	
	static bool firstStart;
	//static vector<vtkSmartPointer<vtkFollower>> conLabels;
	
						   
	static vtkSmartPointer<vtkFollower> xLabel;
	static vtkSmartPointer<vtkFollower> yLabel;
	static vtkSmartPointer<vtkFollower> zLabel;
	static vtkSmartPointer<vtkAssembly> Axes;

	void Execute(vtkObject* vtkNotUsed(caller), unsigned long vtkNotUsed(event),
		void *vtkNotUsed(callData))
	{
		if (!Axes) return;

		int followerId = 0;

		Axes->InitPathTraversal();
		vtkAssemblyPath *path = 0;
		vtkSmartPointer<vtkFollower> followers[3] = { vtkPositionCallback::xLabel, vtkPositionCallback::yLabel, vtkPositionCallback::zLabel };

		//Axes는 arrow3개와 라벨 3개로 구성. 라벨만 follower 적용하기 위하여 path 시작점 변경
		path = Axes->GetNextPath();
		path = Axes->GetNextPath();
		path = Axes->GetNextPath();

		for (int i = 0; i < 3; i++)
		{
			path = Axes->GetNextPath();
			vtkProp3D *prop3D = static_cast<vtkProp3D *>(path->GetLastNode()->GetViewProp());
			if (prop3D)
			{
				prop3D->PokeMatrix(path->GetLastNode()->GetMatrix());
				double* pos = prop3D->GetCenter();
				pos[i] = pos[i] + 2;//tip과 텍스트간의 간격을 두기 위하여 +2
				followers[followerId]->SetPosition(pos);
				followerId++;
				prop3D->PokeMatrix(NULL);
			}
		}
	}

	vtkPositionCallback(){
		//for (int i = 0; i<3; i++)
		//	conLabels[i] = vtkSmartPointer<vtkFollower>::New();
	}
};

class InteractorActor : public vtkInteractorStyleTrackballActor
{
	friend class Arrow;
private:
	bool bSetState;
	vtkSmartPointer<vtkTransformPolyDataFilter> readerTransFilter;
	bool bActorMouseDown;
	static vtkPositionCallback* callback;


protected:

	vtkMatrix4x4* currentMatrix;
	vtkPolyData* currentPolydata = 0;
	vtkSmartPointer<vtkRenderWindowInteractor> renwinin;

	virtual void SetInformation(vtkPolyData* polydata,
		vtkMatrix4x4* matrix,
		vtkRenderWindowInteractor* renwinin);

	virtual void GetInformation(vtkPolyData* polydata,
		vtkMatrix4x4* matrix,
		vtkRenderWindowInteractor* renwinin);

public:
	static InteractorActor* New()
	{
		return new InteractorActor;
	}

	void SetLabelnAxes(vtkSmartPointer<vtkFollower> xL,
		vtkSmartPointer<vtkFollower> yL,
		vtkSmartPointer<vtkFollower> zL,
		vtkSmartPointer<vtkAssembly> axes)
	{
		vtkPositionCallback::xLabel = xL;
		vtkPositionCallback::yLabel = yL;
		vtkPositionCallback::zLabel = zL;
		vtkPositionCallback::Axes = axes;
	}

	InteractorActor()
	{
		readerTransFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		bSetState = false;
		bActorMouseDown = true;
		renwinin = vtkSmartPointer<vtkRenderWindowInteractor>::New();
		callback = vtkPositionCallback::New();
		this->AddObserver(vtkCommand::InteractionEvent, callback);

	}
	~InteractorActor() {}
	virtual void OnLeftButtonDown();

	vtkTypeMacro(InteractorActor, vtkInteractorStyleTrackballActor);
};
#endif
