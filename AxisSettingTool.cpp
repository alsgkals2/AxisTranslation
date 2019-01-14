#pragma once

#include "AxisSettingTool.h"
#include "vtkAssembly.h"

bool const AxisSettingTool::GetSetState()
{
	return bSetState;
}

void AxisSettingTool::FirstChangeAxesTool(vtkMatrix4x4* firstMatrix, vtkPolyData* polydata)
{
	//local ��ǥ��� �̵���Ű�� ���� ����� transform �̸� ����
	auto forStartTrans
		= vtkSmartPointer<vtkTransform>::New();
	forStartTrans->SetMatrix(firstMatrix);
	forStartTrans->Update();

	auto axesTrans
		= vtkSmartPointer<vtkTransform>::New();
	axesTrans->DeepCopy(forStartTrans);

	CleanStack(&stUndoMatrix);
	CleanStack(&stRedoMatrix);
	cmAxesTrans->DeepCopy(axesTrans->GetMatrix());
	
	//���� world ��ǥ��� ������������ �ּ� Ǯ���
	//cmAxesTrans : ó�� tooth�� ��ǥ
	//stUndoMatrix.push(cmAxesTrans);

	//ó�� tooth�� ��ǥ�� ����� �����Ͽ� ������ġ�� �̵�
	axesTrans->Inverse();
	axesTrans->Update();

	auto readerTrans
		= vtkSmartPointer<vtkTransform>::New();
	readerTrans->Identity();
	readerTrans->Concatenate(axesTrans);

	auto polydataCopy
		= vtkSmartPointer<vtkPolyData>::New();
	polydataCopy->DeepCopy(polydata);

	m_ChangedTransFilter
		= vtkSmartPointer<vtkTransformPolyDataFilter>::New(); 
	m_ChangedTransFilter->SetInputData(polydataCopy);
	m_ChangedTransFilter->SetTransform(readerTrans);
	m_ChangedTransFilter->Update();
	polydataCopy->DeepCopy(m_ChangedTransFilter->GetOutput());
	polydata->DeepCopy(m_ChangedTransFilter->GetOutput());
	this->Interactor->Render();
}

void AxisSettingTool::CleanStack(stack<vtkMatrix4x4*>* st)
{
	int size = st->size();
	if (size == 0) return;

	for (int i = 0; i < size; i++)
		st->pop();
}

void AxisSettingTool::ChangeAxesTool(vtkMatrix4x4* currentMatrix, vtkPolyData* currentPolydata)
{
	//set�ϴ� �������ʹ� redo ������ ������� �Ѵ�.
	CleanStack(&stRedoMatrix);

	auto forScaleTransform
		= vtkSmartPointer<vtkTransform>::New();
	forScaleTransform->SetMatrix(currentMatrix);
	forScaleTransform->Scale(0.1, 0.1, 0.1);
	
	auto readerTrans = vtkSmartPointer<vtkTransform>::New();
	readerTrans->Identity();

	//local ��ǥ��� �̵���Ű�� ���� transform �̸� ����
	auto axesTrans = vtkSmartPointer<vtkTransform>::New();
	axesTrans->DeepCopy(forScaleTransform);

	vtkMatrix4x4* _cmAxesTrans = vtkMatrix4x4::New();
	_cmAxesTrans->DeepCopy(axesTrans->GetMatrix());
	stUndoMatrix.push(_cmAxesTrans);

	axesTrans->Inverse();
	axesTrans->Update();
	readerTrans->Concatenate(axesTrans);

	#ifdef temp1220
	double num18[16] = {
		0.1960927617925638, -0.9805413551510664
		, -0.009288681880327327

		, 0.0842960133029816

		, -0.9708584502498953

		, -0.1928080028035787
		, -0.1423339159626582
		, 0.2047153842492254
		, 0.1377733586399592

		, 0.03692864597306776
		, -0.9897751142333602
		, -1.19218899306988

		,0,0,0,1
	};
	double num11[16] = {


		0.9982478527165726

,-0.05291564615703018
,-0.02647940593245712

,-0.1179456159256016

,-0.05866076542700963
,-0.8263643714798353
,-0.5600721740528203

,1.839157485116232
,0.007754943344043494
,0.5606441473345808

,-0.8280205316978642
,-5.088395340064346
,0,0,0,1 };


	readerTrans->SetMatrix(num11);
#endif
	readerTrans->SetMatrix(readerTrans->GetMatrix());
	//view���� data�� �������� �ʵ��� ���� ������ ����
	auto polydataCopy = vtkSmartPointer<vtkPolyData>::New();
	polydataCopy->DeepCopy(currentPolydata);
	m_ChangedTransFilter
		= vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	m_ChangedTransFilter->SetInputData(polydataCopy);
	m_ChangedTransFilter->SetTransform(readerTrans);
	m_ChangedTransFilter->Update();

	if (bSetState)
		currentPolydata->DeepCopy(m_ChangedTransFilter->GetOutput());
	acceptMatrix = readerTrans->GetMatrix();
}

void AxisSettingTool::UndoAxis()
{

	vtkTransformPolyDataFilter* tfReset = m_ChangedTransFilter;
	//import�� ���°� �ƴϸ� return
	if (!tfReset->GetTransform()) return;
	//����ũ�Ⱑ 0�̸� return
	if (stUndoMatrix.size() == 0) return;
	//vtkTransformFilter* tfReset = m_ChangedTransFilter;
	vtkTransform* trans = vtkTransform::New();


	trans->DeepCopy(vtkTransform::SafeDownCast(tfReset->GetTransform()));
	cout << endl;
	cout << "����� ���� �� (Position�� ������ ������ �;���.) " << endl;
	vtkMatrix4x4* transmatrix = vtkMatrix4x4::New();
	transmatrix->DeepCopy(trans->GetMatrix());
	restoreMatrix = vtkMatrix4x4::New();

	//����� : ������ stack -1 ������ result�� ��������̴�.
	//���������� ����� ����� ���� tooth�� matrix�̴�.

	vtkMatrix4x4::Multiply4x4(transmatrix, stUndoMatrix.top(), restoreMatrix);
	restoreMatrix->Modified();

	//redo�Ϸ��� �ٽ� ����� ���ؾ���
	stUndoMatrix.top()->Invert();
	stRedoMatrix.push(stUndoMatrix.top());
	stUndoMatrix.pop();

	trans->SetMatrix(restoreMatrix);
	tfReset->SetTransform(trans);
	tfReset->Update();
	currentPolydata->DeepCopy(tfReset->GetOutput());
}
 void AxisSettingTool::RedoAxis()
{
	//import�� ���°� �ƴϸ� return
	//vtkTransformPolyDataFilter* tfReset = m_ChangedTransFilter;
	if (!m_ChangedTransFilter->GetTransform()|| stRedoMatrix.size() == 0) return;

	vtkTransform* trans = vtkTransform::New();

	trans->DeepCopy(vtkTransform::SafeDownCast(m_ChangedTransFilter->GetTransform()));
	vtkMatrix4x4* transmatrix = vtkMatrix4x4::New();
	transmatrix->DeepCopy(trans->GetMatrix());
	restoreMatrix = vtkMatrix4x4::New();

	//����� : ������ stack -1 ������ result�� ��������̴�.
	//���������� ����� ����� ���� tooth�� matrix�̴�.

	vtkMatrix4x4::Multiply4x4(transmatrix, stRedoMatrix.top(), restoreMatrix);
	restoreMatrix->Modified();

	stRedoMatrix.top()->Invert();
	stUndoMatrix.push(stRedoMatrix.top());
	stRedoMatrix.pop();

	trans->SetMatrix(restoreMatrix);
	m_ChangedTransFilter->SetTransform(trans);
	m_ChangedTransFilter->Update();
	currentPolydata->DeepCopy(m_ChangedTransFilter->GetOutput());
}
 vtkMatrix4x4* AxisSettingTool::GetOriginAxis(int bWrite)
 {
	 int szUndoMatrix = stUndoMatrix.size();
	 if (szUndoMatrix == 0) return nullptr;

	vtkTransform* trans = vtkTransform::New();

	 if (!m_ChangedTransFilter->GetTransform()
		 || bSetState == false) return nullptr;

	 if (bWrite == WRITE)
		 trans->Identity();
	 else
		 trans->DeepCopy(vtkTransform::SafeDownCast(m_ChangedTransFilter->GetTransform()));

	 vtkMatrix4x4* transmatrix = vtkMatrix4x4::New();
	 transmatrix->DeepCopy(trans->GetMatrix());

	 restoreMatrix->Identity();

	 //����� : ������ stack -1 ������ result�� ��������̴�.
	 //���������� ����� ����� ���� tooth�� matrix�̴�.

	 //copy, export�ص� stack�� �ʱ�ȭ���� �ʱ� ����
	 stack<vtkMatrix4x4*> copy_stUndoMatrix;

	 copy_stUndoMatrix = stUndoMatrix;

	 while (szUndoMatrix != 0)
	 {
		 szUndoMatrix--;

		 vtkMatrix4x4::Multiply4x4(copy_stUndoMatrix.top(), transmatrix, restoreMatrix);
		 restoreMatrix->Modified();
		 transmatrix->DeepCopy(restoreMatrix);

		 copy_stUndoMatrix.pop();
	 }

	 if (bWrite == RESET)//reset�Ϸ��� ���
	 {
		 int szMtrix = stUndoMatrix.size();
		 for (int i = 0; i < szMtrix; i++)
		 {
			 stUndoMatrix.pop();
		 }
	 }

	 if (bWrite == WRITE) {
		 auto test_restoreMat = vtkMatrix4x4::New();
		 test_restoreMat->Identity();
		 vtkMatrix4x4::Multiply4x4(cmAxesTrans, restoreMatrix, test_restoreMat);
		 return test_restoreMat;
	 }
	 else {
		 return restoreMatrix;
	 }
 }

 void AxisSettingTool::ResetAxis()
 {


	 vtkMatrix4x4* restoreMatrix = GetOriginAxis(RESET);
	 if (restoreMatrix == nullptr) return;
	// CleanStack(&stUndoMatrix);
	 //CleanStack(&stRedoMatrix);

	 vtkTransform* _trans = vtkTransform::New();
	 _trans->SetMatrix(restoreMatrix);
	 m_ChangedTransFilter->SetTransform(_trans);
	 m_ChangedTransFilter->Update();
	 currentPolydata->DeepCopy(m_ChangedTransFilter->GetOutput());

 }
