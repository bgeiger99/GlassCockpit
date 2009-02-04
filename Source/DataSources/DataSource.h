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

#ifndef DataSource_h
#define DataSource_h

//#include "Object.h"
#include "AirframeDataContainer.h"

namespace OpenGC
{

class DataSource// : public Object FIXME
{
	public:
		
		DataSource();
		virtual ~DataSource();

		/** Initialize the data to a default value*/
		void InitializeData();

		/** Called by the base AppObject after all the init parameters
		 * have been complete. This should open the connection to the sim
		 */
		virtual bool Open();

		/** Called by the render window during idle processing
		 * This function is the one and only place where OpenGC
		 * should acquire data from the sim
		 */
		virtual bool OnIdle();

		/** Get access to airframe data */
		AirframeDataContainer* GetAirframe() { return m_Airframe; }

	protected:

		/** Data that describes the airframe (alt, heading, control surfaces, etc.) */
		AirframeDataContainer* m_Airframe;
};

} // end namespace OpenGC

#endif

