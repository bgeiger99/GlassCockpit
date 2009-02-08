/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Hugo Vincent.
  All rights reserved. 
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "Globals.h"
#include "GLHeaders.h"
#include "GenericBargraph.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object)->*(ptrToMember))

namespace OpenGC
{

GenericBargraph::GenericBargraph()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 16;
	m_PhysicalSize.y = 37;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;

	m_DataFn = 0;
	m_Min = 0.0, m_Max = 0.0;
}

GenericBargraph::~GenericBargraph()
{

}


void GenericBargraph::Render()
{
	GaugeComponent::Render();

	double value = CALL_MEMBER_FN(globals->m_DataSource->GetAirframe(), m_DataFn)();

	if (value < m_Min)
		value = m_Min;
	if (value > m_Max)
		value = m_Max;

	//double boxY = 25.0, boxX = 6.0, stripeX = 3.0, boxOffsetX = 4.0, gapY = 1.5;
	double boxY = 25.0, boxX = 6.0, stripeX = 3.0, boxOffsetX = 2.0, gapY = 1.5;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslated(boxOffsetX, 0.0, 0.0);

	// fill colour
	if (value > m_MaxYellow)
		glColor3ub(51, 51, 76); // blue-grey
	else if (value > m_MaxRed)
		glColor3ub(247, 231, 8); // yellow 
	else
		glColor3ub(255, 20, 20); // red

	// draw the filled box
	double fillY = (value - m_Min) / m_Max * boxY;
	glBegin(GL_POLYGON);
	glVertex2f(stripeX,        0.0);
	glVertex2f(boxX + stripeX, 0.0);
	glVertex2f(boxX + stripeX, fillY);
	glVertex2f(stripeX,        fillY);
	glEnd();

	// yellow stripe
	double yellowStripeY = (m_MaxYellow - m_Min) / m_Max * boxY;
	glColor3ub(247, 231, 8);
	glLineWidth(2.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0, yellowStripeY);
	glVertex2f(stripeX, yellowStripeY);
	glEnd();

	// red stripe
	double redStripeY = (m_MaxRed - m_Min) / m_Max * boxY;
	glColor3ub(255, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0, redStripeY);
	glVertex2f(stripeX, redStripeY);
	glEnd();

	// white indicator line
	glColor3ub(255, 255, 255);
	glBegin(GL_LINES);
	glVertex2f(stripeX,        fillY);
	glVertex2f(boxX + stripeX, fillY);
	glEnd();

	// white outline
	glBegin(GL_LINE_STRIP);
	glVertex2f(boxX + stripeX, boxY);
	glVertex2f(boxX + stripeX, 0.0);
	glVertex2f(stripeX,        0.0);
	glVertex2f(stripeX,        boxY);
	glEnd(); 

	glTranslated(-1.0 * boxOffsetX, gapY, 0.0);

	// white rectangle containing the text
	glColor3ub(255, 255, 255);
	glLineWidth(1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.0,  boxY);
	glVertex2f(0.0,  boxY + 8.5);
	//glVertex2f(20.0, boxY + 8.5);
	//glVertex2f(20.0, boxY);
	glVertex2f(16.0, boxY + 8.5);
	glVertex2f(16.0, boxY);
	glEnd();

	// text above the bar
	globals->m_FontManager->SetSize(m_Font, 4, 4);
	glColor3ub(255, 255, 255);
	char buf[10];
	sprintf(buf, "%.1f", value);
	globals->m_FontManager->Print(1.9, boxY + 2.1, buf, m_Font);

	glPopMatrix();
}

} // end namespace OpenGC