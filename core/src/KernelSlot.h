#ifndef _KERNELSLOT_H
#define _KERNELSLOT_H

#include <common/Strategix_Forward.h>

namespace strategix
{

class KernelSlot
{
	string configPath;
	string mapsPath;
	
public:
	KernelSlot(string configPath, string mapsPath) : configPath(move(configPath)), mapsPath(move(mapsPath)) {}
	virtual ~KernelSlot() = default;
	
	string GetConfigPath() const { return configPath; }
	string GetMapsPath() const { return mapsPath; }
};

}
#endif