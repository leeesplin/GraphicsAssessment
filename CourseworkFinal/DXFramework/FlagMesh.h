/**
* \class Plane Mesh
*
* \brief Simple plane mesh object
*
* Inherits from Base Mesh, Builds a simple plane with texture coordinates and normals.
* Provided resolution values deteremines the subdivisions of the plane.
* Builds a plane from unit quads.
*
* \author Paul Robertson
*/


#ifndef _FLAGMESH_H_
#define _FLAGMESH_H_

#include "BaseMesh.h"

class FlagMesh : public BaseMesh
{

public:
	/** \brief Initialises and builds a plane mesh
	*
	* Can specify resolution of plane, this deteremines how many subdivisions of the plane.
	* @param device is the renderer device
	* @param device context is the renderer device context
	* @param resolution is a int for subdivision of the plane. The number of unit quad on each axis. Default is 100.
	*/
	FlagMesh(ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 100);
	~FlagMesh();

protected:
	void initBuffers(ID3D11Device* device);
	int resolution;
};

#endif