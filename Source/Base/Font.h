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

/**
 * Wraps the FTGL font toolkit to make its interface fit well into the
 * OpenGC environment. Font's are accessed via the FontManager singleton,
 * which is accessible Globals.h: globals->m_FontManager
 */

#ifndef Font_h
#define Font_h

#include "OrderedPair.h"
#include <string>

#ifdef MACOSX
#include "Font_GLTexture.h"
#define FTTextureFont Font_GLTexture
#else
#define FTGL_LIBRARY_STATIC
#include <FTGL/ftgl.h>
#endif

namespace OpenGC
{
	
using namespace std;

class Font
{
	public:
		Font();
		~Font();

		/** Load a font from a specified file */
		bool LoadFont(const string& name);

		/** Print a character string at location (x,y) */
		void Print(double x, double y, const char *string);

		/** Set the size of the font (width and height) in physical units */
		void SetSize(double x, double y);

		/** Sets the font to render to the left of the provided coordinates */
		void SetRightAligned(bool rightAligned) {m_RightAligned = rightAligned; }

		/** Get the font name */
		string& GetName() { return m_Name; }

	protected:

		/** The name of the font */
		string m_Name;
		
		/** Whether to render right-aligned or not */
		bool m_RightAligned;

		/** The size of the font in physical units */
		OrderedPair<double> m_Size;

		/** The font object in GL texture mapped mode */
		FTTextureFont* m_TextureFont;
};

} // end namespace OpenGC

#endif // !defined(Font_h)