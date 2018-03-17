#include "stdafx.h"
#include "DrawContext.h"

#include "CADShapePencil.h"
#include "CADShapeLine.h"
#include "CADShapeRect.h"
#include "CADShapeEllipse.h"
#include "CADShapePolygon.h"
#include "CADShapeSelection.h"
#include "CADShapeDrag.h"
#include "CADSimulation.h"

extern CCADSimulationApp theApp;

CDrawContext::CDrawContext()
{
  m_pobjCurrentShape = new CCADShape;
  //m_pobjCurrentShape = new CCADShapeLine;

}

CDrawContext::CDrawContext(CString &strShapeName)
{
//   if (m_pobjCurrentShape != NULL)
//   {
//     delete m_pobjCurrentShape;
//     m_pobjCurrentShape = NULL;
//   }
// 
//   SetShape(strShapeName);
}

CDrawContext::~CDrawContext()
{
//   if (m_pobjCurrentShape != NULL)
//   {
//     delete m_pobjCurrentShape;
//     m_pobjCurrentShape = NULL;
//   }
}


int CDrawContext::SetShape()
{
  if (*(theApp.GetLastShapeName()) == _T("Ǧ��"))
  {
    m_pobjCurrentShape = new CCADShapePencil;
  }

  if (*(theApp.GetLastShapeName()) == _T("ֱ��"))
  {
    m_pobjCurrentShape = new CCADShapeLine;
  }

  if (*(theApp.GetLastShapeName()) == _T("����"))
  {
    m_pobjCurrentShape = new CCADShapeRect;
  }

  if (*(theApp.GetLastShapeName()) == _T("��Բ"))
  {
    m_pobjCurrentShape = new CCADShapeEllipse;
  }

  if (*(theApp.GetLastShapeName()) == _T("�Զ���ͼ��") && 
    theApp.m_nCustomiezedShapePointCount >= 3)
  {
    m_pobjCurrentShape = new CCADShapePolygon;

    memcpy(((CCADShapePolygon *)m_pobjCurrentShape)->m_aryPoints,
      theApp.m_aryCustomizedShapePoints,
      sizeof(((CCADShapePolygon *)m_pobjCurrentShape)->m_aryPoints));

    ((CCADShapePolygon *)m_pobjCurrentShape)->m_nPointCount = 
      theApp.m_nCustomiezedShapePointCount;
  }

  if (*(theApp.GetLastShapeName()) == _T("ѡ��"))
  {
    m_pobjCurrentShape = new CCADShapeSelection;
  }

  if (*(theApp.GetLastShapeName()) == _T("ȡ��"))
  {
    m_pobjCurrentShape = new CCADShape;
    //m_pobjCurrentShape = new CCADShapeLine;
  }

  if (*(theApp.GetLastShapeName()) == _T("�϶�"))
  {
    m_pobjCurrentShape = new CCADShapeDrag;
  }


  return 0;
}

int CDrawContext::OnLButtonDown(CPoint & objPoint)
{
  SetShape();
  m_pobjCurrentShape->SetBeginPoint(objPoint);

  return 0;
}

int CDrawContext::OnMouseMove(CPoint & objPoint)
{
  m_pobjCurrentShape->SetEndPoint(objPoint);

  return 0;
}

int CDrawContext::OnLButtonUp(CPoint & objPoint)
{
  m_pobjCurrentShape->SetEndPoint(objPoint);
  m_pobjCurrentShape->SaveThisShape(objPoint);

  return 0;
}

int CDrawContext::Draw(HWND hWnd, CDC * pDC)
{
  m_pobjCurrentShape->DrawTheBigPicture(hWnd, pDC);

  return 0;
}
