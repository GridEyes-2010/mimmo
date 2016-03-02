/*---------------------------------------------------------------------------*\
 * 
 *  MiMMO
 *
 *  Copyright (C) 2015-2016 OPTIMAD engineering Srl
 *
 *  -------------------------------------------------------------------------
 *  License
 *  This file is part of MiMMO.
 *
 *  MiMMO is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License v3 (LGPL)
 *  as published by the Free Software Foundation.
 *
 *  MiMMO is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with MiMMO. If not, see <http://www.gnu.org/licenses/>.
 *
 \ *---------------------------------------------------------------------------*/

#ifndef __MIMMOBASICSHAPES_HH
#define __MIMMOBASICSHAPES_HH

// local libs
#include "bitpit.hpp"
#include "MiMMO_TypeDef.hpp"

/*!
 *	\date			03/01/2015
 *	\authors		Edoardo Lombardi
 *	\authors		Arpa Rocco
 *	\copyright		Copyright 2015 Optimad engineering srl. All rights reserved.
 *	\par			License:\n
 *
 *	\brief Abstract Interface class for Elementary Shape Representation
 *
 *	Interface class for Volumetric Core Element, suitable for interaction with Data Structure stored in a MimmoObject class.
 *  Object orientation in 3D space can be externally manipulated with dedicated transformation blocks. Class 
 *  internally implement transformation to/from local sdr to/from world sdr, that can be used in derived objects from it.
 *	Class works with three reference systems:
 * 	1) Global Absolute SDR: is the external World reference system
 *  2) Local Relative SDR: is the local reference system, not affected by Rigid Transformations as RotoTranslations or Scalings 
 *  3) basic SDR: local system remapping to unitary cube, not accounting of the shape type.  
 *   
 */
 
class BasicShape {

public:
	enum ShapeType{CUBE, CYLINDER, SPHERE}; /**< type of possible shape in your class */


protected:
	ShapeType	m_shape; 		/**< shape identifier, see BasicShape::ShapeType enum */
	darray3E	m_origin; 		/**< origin of your shape. Meant as its baricenter*/
	dmatrix32E	m_limitSpan;	/**< coordinate limits of your current shape, in its local reference system*/
	dmatrix33E	m_sdr;			/**< axis position of the local reference system w.r.t absolute one*/
	bvector1D	m_closedLoops;	/**< boolean identifiers for periodic/simple shape coordinate definition*/
	darray3E 	m_scaling; 		/**< scaling vector */
	
	
public:
	
	//Costructors, Destructor, Copy/Assignment operators
    BasicShape();
    virtual ~BasicShape();
 	
	//set-get methods
	void	setOrigin(darray3E);
	void	setSpan(double, double, double);
	void	setInfLimits(double, double, double);
	void	setRefSystem(darray3E, darray3E, darray3E);
	void	setRefSystem(darray3E, darray3E);
	
	void	setClosedLoops(bool, bool, bool);
	
	darray3E	getOrigin();
	darray3E	getSpan();
	darray3E	getInfLimits();
	dmatrix33E	getRefSystem();
	bvector1D	areClosedLoops();
	ShapeType   getShapeType();
	const ShapeType   getShapeType() const;
	
	darray3E	getScaling();
	virtual darray3E 	getLocalSpan()=0;
	virtual darray3E 	getLocalInfLimits()=0;
	
	
	//functionalities
    ivector1D	includeGeometry(bitpit::Patch * );
    ivector1D	excludeGeometry(bitpit::Patch * );
    ivector1D	includeCloudPoints(dvecarr3E &);
    ivector1D	excludeCloudPoints(dvecarr3E &);
	ivector1D	includeCloudPoints(bitpit::Patch * );
	ivector1D	excludeCloudPoints(bitpit::Patch * );
	bool	isSimplexIncluded(dvecarr3E &);
	bool	isSimplexIncluded(bitpit::Patch * , int indexT);
    bool	isPointIncluded(darray3E);
	bool	isPointIncluded(bitpit::Patch * , int indexV);
	
	virtual	darray3E	toWorldCoord(darray3E & point)=0;
	virtual	darray3E	toLocalCoord(darray3E & point)=0;

private:	
	virtual	darray3E	basicToLocal(darray3E & point)=0;
	virtual	darray3E	localToBasic(darray3E & point)=0;
	virtual void 		checkSpan(double &, double &, double &)=0;
	virtual void 		checkInfLimits(double &, double &, double &)=0;
	virtual void		setScaling()=0;
};

/*!
 *	\date			03/01/2015
 *	\authors		Edoardo Lombardi
 *	\authors		Arpa Rocco
 *	\copyright		Copyright 2015 Optimad engineering srl. All rights reserved.
 *	\par			License:\n
 *
 *	\brief Elementary Shape Representation of a Cube
 *
 *	Volumetric Core Element, shaped as a cube, directly derived from BasicShape class.   
 */

class Cube: public BasicShape {
	
public:
	
	//Costructors, Destructor, Copy/Assignment operators
	Cube();
	Cube(darray3E &origin, dmatrix32E &limits);
	~Cube();

	Cube(const Cube &);
	Cube & operator=(const Cube &);
	
	//reimplementing pure virtuals	

	darray3E 	getLocalSpan();
	darray3E 	getLocalInfLimits();
	darray3E	toWorldCoord(darray3E & point);
	darray3E	toLocalCoord(darray3E & point);
	
private:	
	darray3E	basicToLocal(darray3E & point);
	darray3E	localToBasic(darray3E & point);
	void 		checkSpan(double &, double &, double &);
	void 		checkInfLimits(double &, double &, double &);
	void 		setScaling();
	
};

/*!
 *	\date			03/01/2015
 *	\authors		Edoardo Lombardi
 *	\authors		Arpa Rocco
 *	\copyright		Copyright 2015 Optimad engineering srl. All rights reserved.
 *	\par			License:\n
 *
 *	\brief Elementary Shape Representation of a Cylinder or portion of it
 *
 *	Volumetric Core Element, shaped as a cylinder, directly derived from BasicShape class.   
 */

class Cylinder: public BasicShape {
	
public:
	//Costructors, Destructor, Copy/Assignment operators
	Cylinder();
	Cylinder(darray3E &origin, dmatrix32E &limits);
	~Cylinder();

	Cylinder(const Cylinder &);
	Cylinder & operator=(const Cylinder &);
	
	
	//reimplementing pure virtuals
	darray3E 	getLocalSpan();
	darray3E 	getLocalInfLimits();
	darray3E	toWorldCoord(darray3E & point);
	darray3E	toLocalCoord(darray3E & point);
		
private:	
	darray3E	basicToLocal(darray3E & point);
	darray3E	localToBasic(darray3E & point);
	void 		checkSpan(double &, double &, double &);
	void 		checkInfLimits(double &, double &, double &);
	void 		setScaling();
};


/*!
 *	\date			03/01/2015
 *	\authors		Edoardo Lombardi
 *	\authors		Arpa Rocco
 *	\copyright		Copyright 2015 Optimad engineering srl. All rights reserved.
 *	\par			License:\n
 *
 *	\brief Elementary Shape Representation of a Sphere or portion of it
 *
 *	Volumetric Core Element, shaped as a sphere, directly derived from BasicShape class.   
 */

class Sphere: public BasicShape {
	
public:
	//Costructors, Destructor, Copy/Assignment operators
	Sphere();
	Sphere(darray3E &origin, dmatrix32E &limits);
	~Sphere();
	
	Sphere(const Sphere &);
	Sphere & operator=(const Sphere &);
	
	
	//reimplementing pure virtuals
	darray3E 	getLocalSpan();
	darray3E 	getLocalInfLimits();
	darray3E	toWorldCoord(darray3E & point);
	darray3E	toLocalCoord(darray3E & point);
	
private:	
	darray3E	basicToLocal(darray3E & point);
	darray3E	localToBasic(darray3E & point);
	void 		checkSpan(double &, double &, double &);
	void 		checkInfLimits(double &, double &, double &);
	void 		setScaling();
	
};


#endif //  __MIMMOBASICSHAPES_HH

