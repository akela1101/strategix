/* 
 * File:   TerrainPositioner.h
 * Author: Akela1101
 *
 * Created on 4 Апрель 2010 г., 13:53
 */

#ifndef _TERRAINPOSITIONER_H
#define	_TERRAINPOSITIONER_H

#include <Ogre.h>
#include <OgreCommon.h>
#include <map>
#include <string>


namespace Sample1
{
	using namespace std;
	using namespace Ogre;

	struct TerrainPositioner
	{
		string name; // Texture name
		short divs; // Number of divisions of picture's square
		std::map<string, short> positions; // Map[terrain name, position of terrain on picture]

		TerrainPositioner(const char fileName[]);

		// Coordinates of the part of picture with such name.
		const FloatRect GetTexRect(string terrName);
	};
}

#endif	/* _TERRAINPOSITIONER_H */
