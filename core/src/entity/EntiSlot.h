#ifndef _ENTISLOT_H
#define    _ENTISLOT_H

#include "feature/Interfaces.h"

#include "common/Strategix_Forward.h"


namespace strategix
{
class EntiSlot : public ITick, public IMove, public ICollect, public IHealth, public IAttack
{
	const string name;
	
public:
	EntiSlot(const string& name) : name(name) {}
	virtual ~EntiSlot() = 0;
	EntiSlot(const EntiSlot& _c) = delete;
	EntiSlot& operator=(const EntiSlot& _c) = delete;
	
	const string& GetName() const { return name; }
};
}

#endif    /* _ENTISLOT_H */

