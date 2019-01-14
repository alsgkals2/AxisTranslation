#include "Arrow.h"
#define SET_BACKGROUND_AXIS
#include "vtkRegularPolygonSource.h"

std::vector< vtkSmartPointer<vtkFollower>> Arrow::m_conLabels = { nullptr,nullptr,nullptr };
vtkSmartPointer<vtkAssembly> Arrow::movableAxes = nullptr;

vtkPolyData*  Arrow::GetPolydata() const {
	return changedPolydata;
}
void Arrow::SetPolydata(vtkPolyData* polydata)
{
	changedPolydata = polydata;
}

void Arrow::MakeArrow(vtkOpenGLRenderer* qtRen)
{
	static vtkSmartPointer<vtkAxesActor> axes = vtkSmartPointer<vtkAxesActor>::New();
	auto matrix = vtkMatrix4x4::New();
	matrix->Identity();
	//처음 진입하는 경우
	if (!movableAxes)
	{
		movableAxes = vtkSmartPointer<vtkAssembly>::New();
		for(int i=0; i<3; i++)
		m_conLabels[i] = vtkSmartPointer<vtkFollower>::New();
	}

	vtkSmartPointer<vtkPropCollection> collection =
		vtkSmartPointer<vtkPropCollection>::New();
	axes->GetActors(collection);
	axes->Modified();
	//axes->GetXAxisTipProperty()->SetLineWidth(0.1);
	axes->GetXAxisShaftProperty()->SetLineWidth(6.5);

	collection->InitTraversal();
	auto xText = vtkSmartPointer<vtkVectorText>::New();
	auto xTextMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	xText->SetText("M");
	xTextMapper->SetInputConnection(xText->GetOutputPort());
	m_conLabels[0]->SetMapper(xTextMapper);
	m_conLabels[0]->SetScale(1.2);
	m_conLabels[0]->SetCamera(qtRen->GetActiveCamera());

	double* tPosition = ((vtkProp3D*)collection->GetItemAsObject(3))->GetCenter();

	double arrPositionX[3];
	arrPositionX[0] = tPosition[0] * ARROW_SCALE+2; //팁과 겹치는 것을 방지하기 위해 +1
	arrPositionX[1] = tPosition[1];
	arrPositionX[2] = tPosition[2];
	m_conLabels[0]->SetPosition(arrPositionX);
	m_conLabels[0]->PickableOff();

	auto yText = vtkSmartPointer<vtkVectorText>::New();
	auto yTextMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	yText->SetText("L");
	yTextMapper->SetInputConnection(yText->GetOutputPort());
	m_conLabels[1]->SetMapper(yTextMapper);
	m_conLabels[1]->SetScale(1.2);
	m_conLabels[1]->SetCamera(qtRen->GetActiveCamera());
	tPosition = ((vtkProp3D*)collection->GetItemAsObject(4))->GetCenter();

	double arrPositionY[3];
	arrPositionY[0] = tPosition[0];
	arrPositionY[1] = tPosition[1] * ARROW_SCALE + 2;
	arrPositionY[2] = tPosition[2];
	m_conLabels[1]->SetPosition(arrPositionY);
	m_conLabels[1]->PickableOff();

	auto zText = vtkSmartPointer<vtkVectorText>::New();
	auto zTextMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	zText->SetText("O");
	zTextMapper->SetInputConnection(zText->GetOutputPort());
	m_conLabels[2]->SetMapper(zTextMapper);
	m_conLabels[2]->SetScale(1.2);
	m_conLabels[2]->SetCamera(qtRen->GetActiveCamera());
	tPosition = ((vtkProp3D*)collection->GetItemAsObject(5))->GetCenter();
	double arrPositionZ[3];
	arrPositionZ[0] = tPosition[0];
	arrPositionZ[1] = tPosition[1];
	arrPositionZ[2] = tPosition[2] * ARROW_SCALE + 2;

	m_conLabels[2]->SetPosition(arrPositionZ);
	m_conLabels[2]->PickableOff();

	for (int i = 0; i < collection->GetNumberOfItems(); ++i)
		movableAxes->AddPart((vtkProp3D*)collection->GetNextProp());

	movableAxes->SetUserMatrix(matrix);
	movableAxes->SetScale(ARROW_SCALE);
	qtRen->AddActor(movableAxes);
	qtRen->AddActor(m_conLabels[0]);
	qtRen->AddActor(m_conLabels[1]);
	qtRen->AddActor(m_conLabels[2]);

	renWinIn->Render();
}

void Arrow::InitAixsLabels()
{
	vtkMatrix4x4* identityMat = vtkMatrix4x4::New();
	identityMat->Identity();
	movableAxes->SetUserMatrix(identityMat);

	for (int i = 0; i < 3; i++)
	{
		double arrPosition[3] = { 0, };
		arrPosition[i] = ARROW_SCALE + 2;
		m_conLabels[i]->SetPosition(arrPosition);
	}
}

void Arrow::SetToothPolydataAndRenderer(vtkPolyData* sourOrigin,
	vtkOpenGLRenderer* qtRen,
	vtkGenericOpenGLRenderWindow* qtRenWin)
{
	vtkSmartPointer<vtkPolyDataMapper> toothMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	double* bound = new double[6];
	bound = sourOrigin->GetBounds();
	toothMapper->SetInputData(sourOrigin);

	// Create an actor
	toothActor->SetMapper(toothMapper);
	toothActor->GetProperty()->SetOpacity(0.8);
	SetToothPickableAndDragable(false, false);

	double* dToothCenter = toothActor-> GetCenter();
	
#ifdef  SET_BACKGROUND_AXIS
	vtkSmartPointer<vtkPolyData> linesPolyData =
		vtkSmartPointer<vtkPolyData>::New();

	// Create three points
	double origin_p0[3] = { -1000.0, 0.0, 0.0 };
	double p0[3] = { 1000.0, 0.0, 0.0 };
	double origin_p1[3] = { 0.0, -1000.0, 0.0 };
	double p1[3] = { 0.0, 1000.0, 0.0 };
	double origin_p2[3] = { 0.0, 0.0, -1000.0 };
	double p2[3] = { 0.0, 0, 1000.0 };
	// Create a vtkPoints container and store the points in it
	vtkSmartPointer<vtkPoints> pts =
		vtkSmartPointer<vtkPoints>::New();
	pts->InsertNextPoint(origin_p0);
	pts->InsertNextPoint(p0);
	pts->InsertNextPoint(origin_p1);
	pts->InsertNextPoint(p1);
	pts->InsertNextPoint(origin_p2);
	pts->InsertNextPoint(p2);
	linesPolyData->SetPoints(pts);

	vector<vtkSmartPointer<vtkLine>> line(3);
	for(int i=0; i<3; i++)
		line[i] = vtkSmartPointer<vtkLine>::New();

	for (int i = 0; i < 3; i++)
	{
		line[i]->GetPointIds()->SetId(0, 2*i);
		line[i]->GetPointIds()->SetId(1, 2 * i + 1);
	}

vtkSmartPointer<vtkCellArray> cellArrLines =
		vtkSmartPointer<vtkCellArray>::New();

foreach(vtkSmartPointer<vtkLine> _line, line)
cellArrLines->InsertNextCell(_line);

	linesPolyData->SetLines(cellArrLines);

	unsigned char red[3] = { 91, 22, 89 };
	unsigned char green[3] = { 16, 58, 14 };
	unsigned char blue[3] = { 13, 35, 60 };

	vtkSmartPointer<vtkUnsignedCharArray> colors =
		vtkSmartPointer<vtkUnsignedCharArray>::New();
	colors->SetNumberOfComponents(3);
	colors->InsertNextTypedTuple(red);
	colors->InsertNextTypedTuple(green);
	colors->InsertNextTypedTuple(blue);
	linesPolyData->GetCellData()->SetScalars(colors);

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(linesPolyData);

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->PickableOff();
	actor->GetProperty()->SetLineWidth(1.5);
	qtRen->AddActor(actor);

	vtkSmartPointer<vtkVectorText> textPlus =
		vtkSmartPointer<vtkVectorText>::New();
	textPlus->SetText("Occlusal");
	vtkSmartPointer<vtkVectorText> textMinus =
		vtkSmartPointer<vtkVectorText>::New();
	textMinus->SetText("Buccal");

	// Create a mapper
	vtkSmartPointer<vtkPolyDataMapper> textPlusMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	vtkSmartPointer<vtkPolyDataMapper> textMinusMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();

	textPlusMapper->SetInputConnection(textPlus->GetOutputPort());
	textMinusMapper->SetInputConnection(textMinus->GetOutputPort());
	vtkSmartPointer<vtkFollower> textPlusfollower =
		vtkSmartPointer<vtkFollower>::New();
	textPlusfollower->SetMapper(textPlusMapper);
	textPlusfollower->GetProperty()->SetColor(1, 1, 1);
	textPlusfollower->PickableOff();
	textPlusfollower->SetPosition(0, 0, 20);
	textPlusfollower->SetScale(2);
	textPlusfollower->SetCamera(qtRen->GetActiveCamera());

	vtkSmartPointer<vtkFollower> textMinusfollower =
		vtkSmartPointer<vtkFollower>::New();
	textMinusfollower->SetMapper(textMinusMapper);
	textMinusfollower->GetProperty()->SetColor(1, 1, 1);
	textMinusfollower->PickableOff();
	textMinusfollower->SetPosition(0, -20, 0);
	textMinusfollower->SetScale(2);
	textMinusfollower->SetCamera(qtRen->GetActiveCamera());

	qtRen->AddActor(textPlusfollower);
	qtRen->AddActor(textMinusfollower);

	//외면이 자글자글하게 나타나는 부분 해결
	qtRen->SetUseFXAA(true);
	qtRen->SetUseDepthPeeling(1);
	qtRen->SetMaximumNumberOfPeels(8);
	qtRen->Modified();
#endif
	qtRen->AddActor(toothActor);
	qtRen->GetActiveCamera()->SetPosition(-13, -4, 76);
	qtRen->GetActiveCamera()->SetFocalPoint(-13, -4, 6);
	qtRen->GetActiveCamera()->SetViewUp(0, 1, 0);
	qtRen->Render();

	qtRenWin->AddRenderer(qtRen);
	qtRenWin->Render();

	changedPolydata = sourOrigin;
	arrowMatrix = movableAxes->GetMatrix();
	renWinIn = qtRenWin->GetInteractor();

	vtkMatrix4x4* firstCenterMatrix = vtkMatrix4x4::New();
	firstCenterMatrix->SetElement(0, 3, dToothCenter[0]);
	firstCenterMatrix->SetElement(1, 3, dToothCenter[1]);
	firstCenterMatrix->SetElement(2, 3, dToothCenter[2]);

	style = vtkSmartPointer<AxisSettingTool>::New();
	style->SetInformation(changedPolydata, arrowMatrix, renWinIn);
	style->SetLabelnAxes(Arrow::m_conLabels[0], Arrow::m_conLabels[1], Arrow::m_conLabels[2], Arrow::movableAxes);
	SetStyle(style);
	style->SetFirstAxisMatrix(firstCenterMatrix);

	//0102 QA의 요구로 좌표축을 즉시 확인해 볼 수 있도록 하기 위해 원점변환 함수 주석
	//style-> FirstChangeAxesTool(firstCenterMatrix, sourOrigin);
}
void Arrow::InvertAxis(int invertNum)
{
	switch (invertNum)
	{
	case 0:
		movableAxes->RotateX(180);
		break;

	case 1:
		movableAxes->RotateY(180);
		break;

	case 2:
		movableAxes->RotateZ(180);
		break;
	}
}