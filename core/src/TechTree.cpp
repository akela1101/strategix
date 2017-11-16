#include "entity/EntiInfo.h"
#include "TechTree.h"


namespace strategix
{
using namespace std;


const EntiInfo& TechTree::GetNode(const string& name) const
{
	auto it = techMap.find(name);
	if (it == techMap.end())
	{
		throw_nya("Wrong Enti name: " + name);
	}
	return *it->second;
}

void TechTree::AddNode(u_p<EntiInfo> entiInfo)
{
	auto iter_ok = techMap.emplace(entiInfo->name, move(entiInfo));
	if (!iter_ok.second)
	{
		throw_nya("More than one EntiInfo with name: " + entiInfo->name);
	}
}

}
