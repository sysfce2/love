/**
 * Copyright (c) 2006-2025 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

#ifndef LOVE_PHYSICS_BOX2D_CHAIN_SHAPE_H
#define LOVE_PHYSICS_BOX2D_CHAIN_SHAPE_H

// Module
#include "Shape.h"
#include "EdgeShape.h"

namespace love
{
namespace physics
{
namespace box2d
{

/**
 * A ChainShape is a freeform collection of line segments.
 **/
class ChainShape : public Shape
{
public:

	static love::Type type;

	/**
	 * Create a new ChainShape from a Box2D chain shape.
	 * @param c The chain shape.
	 **/
	ChainShape(Body *body, const b2ChainShape &c);

	virtual ~ChainShape();

	/**
	 * Establish connectivity to a vertex that follows
	 * the last vertex. Fails if called on a loop.
	 * @param x The x-coordinate of the vertex.
	 * @param y The y-coordinate of the vertex.
	 **/
	void setNextVertex(float x, float y);

	/**
	 * Establish connectivity to a vertex that precedes
	 * the first vertex. Fails if called on a loop.
	 * @param x The x-coordinate of the vertex.
	 * @param y The y-coordinate of the vertex.
	 **/
	void setPreviousVertex(float x, float y);

	/**
	 * Gets the vertex that follows the last vertex.
	 **/
	b2Vec2 getNextVertex() const;

	/**
	 * Gets the vertex that precedes the first vertex.
	 **/
	b2Vec2 getPreviousVertex() const;

	/**
	 * Returns a child EdgeShape.
	 * @param index The index of the child shape.
	 * @returns The specified child.
	 **/
	EdgeShape *getChildEdge(int index) const;

	/**
	 * Returns the number of vertices in the shape.
	 * @returns The number of vertices in the shape.
	 **/
	int getVertexCount() const;

	/**
	 * Returns the vertex at the given index.
	 * @param index The index of the vertex.
	 * @returns The specified vertex.
	 **/
	b2Vec2 getPoint(int index) const;

	/**
	 * Returns all of the vertices.
	 * @returns The vertices the shape comprises.
	 **/
	const b2Vec2 *getPoints() const;

};

} // box2d
} // physics
} // love

#endif // LOVE_PHYSICS_BOX2D_CHAIN_SHAPE_H
