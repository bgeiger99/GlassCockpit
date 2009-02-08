/*=========================================================================

  OpenGC - The Open Source Glass Cockpit Project
  Please see our web site at http://www.opengc.org

  Albatross UAV Project - http://www.albatross-uav.org

  Copyright (c) 2001-2003 Damion Shelton
  All rights reserved. Contributions by Hugo Vincent.
  See Copyright.txt or http://www.opengc.org/Copyright.htm for details.

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include <stdio.h>

#include "Globals.h"
#include "GaugeComponent.h"
#include "SpeedTicker.h"

namespace OpenGC
{

SpeedTicker::SpeedTicker()
{
	m_Font = globals->m_FontManager->LoadDefaultFont();

	m_PhysicalPosition.x = 0;
	m_PhysicalPosition.y = 0;

	m_PhysicalSize.x = 21;
	m_PhysicalSize.y = 18;

	m_Scale.x = 1.0;
	m_Scale.y = 1.0;
}

SpeedTicker::~SpeedTicker()
{

}

void SpeedTicker::Render()
{
	// Call base class to setup viewport and projection
	GaugeComponent::Render();

	// Draw black background
	glColor3ub(0,0,0);
	// Rectangular part
	glRectd(0.0,0.0,18.0,18.0);
	// Triangular part
	glBegin(GL_TRIANGLES);
	glVertex2f(18.0,7.0);
	glVertex2f(21.0,9.0);
	glVertex2f(18.0,11.0);
	glEnd();

	// White border around background
	glColor3ub(255,255,255);
	glLineWidth(1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(0.0,0.0);
	glVertex2f(0.0,18.0);
	glVertex2f(18.0,18.0);
	glVertex2f(18.0,11.0);
	glVertex2f(21.0,9.0);
	glVertex2f(18.0,7.0);
	glVertex2f(18.0,0.0);
	glEnd();

	char buffer[4];
	double airspeed = globals->m_DataSource->GetAirframe()->GetAirspeed_KT();
	
	if (airspeed > 999.0)
		airspeed = 999.0; 
	
	// Speed for integer calculations
	long int spd = (long int) airspeed;

	// y position of the text (for easy changes)
	const double fontHeight = 7.0;
	double texty = m_PhysicalSize.y / 2 - fontHeight / 2;

	globals->m_FontManager->SetSize(m_Font, 6.0, fontHeight);

	// Draw text in white
	glColor3ub(255,255,255);

	if(airspeed>=100.0)
	{
		// 100's
		sprintf(buffer, "%li", spd/100);
		globals->m_FontManager->Print(2.0, texty, &buffer[0], m_Font);
		spd = spd-100*(int)(spd/100);
	}

	if(airspeed>10.0)
	{
		// 10's
		sprintf(buffer, "%li", spd/10);
		globals->m_FontManager->Print(6.5, texty, &buffer[0], m_Font);
		spd = spd-10*(int)(spd/10);
	}

	// 1's - the most complicated, since they scroll
	// Middle digit
	int three_one = spd;
	// Now figure out the digits above and below
	int five_one = (three_one+2)%10;
	int four_one = (three_one+1)%10;
	int two_one = (three_one - 1 + 10)%10;
	int one_one = (three_one - 2 + 10)%10;

	// Figure out the Speed translation factor for the one's place
	glTranslated(0, -1*(airspeed - (int)airspeed)*fontHeight, 0);

	// Display all of the digits
	sprintf(buffer, "%i", five_one);
	globals->m_FontManager->Print(11.0, texty+fontHeight*2+fontHeight/5, &buffer[0], m_Font);

	sprintf(buffer, "%i", four_one);
	globals->m_FontManager->Print(11.0, texty+fontHeight+fontHeight/10, &buffer[0], m_Font);

	sprintf(buffer, "%i", three_one);
	globals->m_FontManager->Print(11.0, texty, &buffer[0], m_Font);

	sprintf(buffer, "%i", two_one);
	globals->m_FontManager->Print(11.0, texty-fontHeight-fontHeight/10, &buffer[0], m_Font);

	sprintf(buffer, "%i", one_one);
	globals->m_FontManager->Print(11.0, texty-fontHeight*2-fontHeight/5, &buffer[0], m_Font);
}

} // end namespace OpenGC