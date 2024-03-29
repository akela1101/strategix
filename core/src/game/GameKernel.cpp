#include <boost/filesystem.hpp>
#include <random>

#include "../../strx/EntityInfo.hpp"
#include "../../strx/Kernel.hpp"
#include "../../strx/Map.hpp"
#include "../../strx/MapObject.hpp"
#include "../../strx/Message.hpp"
#include "../../strx/TechTree.hpp"
#include "../entity/EntityKernel.hpp"
#include "../game/PlayerKernel.hpp"
#include "../kernel/ConfigManager.hpp"

#include "GameKernel.hpp"

namespace strx
{
GameKernel::GameKernel(const string& mapName)
{
	string mapsPath = ConfigManager::GetMapsPath();
	if (mapsPath.empty()) nya_throw << "Configuration should be loaded before adding game.";

	LoadMap(mapName);
}

s_p<EntityKernel> GameKernel::GetEntity(IdType id) const
{
	auto i = entities.find(id);
	return i == entities.end() ? nullptr : i->second;
}

void GameKernel::ReceiveMessage(s_p<Message> message, PlayerId playerId)
{
	switch (message->GetType())
	{
		case Message::Type::PLAYER: AddPlayer(move(message), playerId); break;
		case Message::Type::JOIN: HandleJoin(playerId); break;
		default:
			auto&& command = dp_cast<CommandMessage>(message);
			if (!command) nya_throw << "Unknown message: " << message->GetType().c_str();

			auto i = entities.find(command->id);
			if (i == entities.end()) nya_throw << "Entity with id %d does not exist."s % command->id;

			EntityKernel* entity = i->second.get();
			entity->ReceiveMessage(move(command));
	}
}

void GameKernel::Tick(float seconds)
{
	for (auto& entity : entities | nya::map_values) { entity->Tick(seconds); }

	for (IdType entityId : removedEntities)
	{
		auto i = entities.find(entityId);
		if (i == entities.end()) nya_throw << "Trying to destroy entity %d one more time."s % entityId;

		i->second->GetPlayer().EntityRemoved(entityId);
		entities.erase(i);
	}
	removedEntities.clear();
}

void GameKernel::AddEntity(MapEntity* mapEntity)
{
	auto iSpotId = spotIds.find(mapEntity->ownerSpot);
	if (iSpotId == spotIds.end()) return;

	auto& player = *players.at(iSpotId->second);
	auto& info = player.GetTechTree().GetNode(mapEntity->name);
	int objectId = mapEntity->id;

	auto entity = make_s<EntityKernel>(*this, player, objectId, info, mapEntity->coord);
	player.EntityAdded(entity);
	entities.emplace(objectId, move(entity));
}

void GameKernel::RemoveEntity(IdType entityId)
{
	removedEntities.push_back(entityId);
}

s_p<MapMessage> GameKernel::CreateMapMessage(int playerSpot)
{
	// @#~ should return map AND fog-of-war mask
	ignore = playerSpot;
	return make_s<MapMessage>(map);
}

void GameKernel::LoadMap(const string& mapName)
{
	string mapsPath = ConfigManager::GetMapsPath();
	string fileName = boost::str("%s.map"s % mapName);
	string path = (boost::filesystem::path(mapsPath) / fileName).string();

	map.reset(new Map(path));
}

void GameKernel::AddPlayer(s_p<Message> message, PlayerId playerId)
{
	trace_log << "add player: " << playerId;

	auto playerMessage = sp_cast<PlayerMessage>(message);
	if (playerMessage->type == PlayerType::AI)
	{
		playerId = Kernel::GetNextPlayerId();  // id for AI
	}

	int& playerSpot = playerMessage->spot;
	if (!playerSpot)
	{
		for (auto spot : map->GetPlayerSpots())
		{
			if (!nya_in(spot, spotIds))
			{
				spotIds.emplace(spot, playerId);
				playerSpot = spot;
				break;
			}
		}
		if (!playerSpot) nya_throw << "Map is already full and cannot allow one more: " << playerId;
	}
	else
	{
		const auto& spots = map->GetPlayerSpots();
		if (find(nya_all(spots), playerSpot) == spots.end()) nya_throw << "There's no map spot: " << playerSpot;

		if (nya_in(playerSpot, spotIds))  // replace is not supported yet
		{
			nya_throw << "Trying to add same player twice [%d], name: %s"s % playerSpot % playerMessage->name;
		}
	}

	if (playerMessage->race.empty())
	{
		auto races = Kernel::GetRaceNames();
		std::sample(nya_all(races), &playerMessage->race, 1, std::mt19937(std::time(0)));
	}

	if (playerMessage->name.empty()) playerMessage->name = ("Player%d"s % playerId).str();

	spotIds.emplace(playerSpot, playerId);
	plannedPlayers.push_back(playerMessage);

	auto humanMessage = make_s<PlayerMessage>(*playerMessage);
	Kernel::SendMessageOne(move(playerMessage), playerId);

	humanMessage->type = PlayerType::HUMAN;
	Kernel::SendMessageAll(move(humanMessage));
}

void GameKernel::HandleJoin(PlayerId playerId)
{
	trace_log << "player joined: " << playerId;
	joinedPlayers.insert(playerId);

	// wait all human players joined
	for (const auto& plannedPlayer : plannedPlayers)
	{
		PlayerId id = spotIds.at(plannedPlayer->spot);
		if (plannedPlayer->type == PlayerType::HUMAN && !nya_in(id, joinedPlayers)) return;
	}
	Start();
}

void GameKernel::Start()
{
	trace_log << "game is started";

	for (const auto& playerMessage : plannedPlayers)
	{
		PlayerId id = spotIds.at(playerMessage->spot);

		// map
		auto&& mapMessage = CreateMapMessage(playerMessage->spot);
		if (playerMessage->type == PlayerType::SELF) Kernel::SendMessageOne(move(mapMessage), id);

		// player
		auto player = make_u<PlayerKernel>(*this, id, *playerMessage, *map);
		players.emplace(id, move(player));
	}
	plannedPlayers.clear();

	// entities
	for (int y : boost::irange(0, map->GetLength()))
	{
		for (int x : boost::irange(0, map->GetWidth()))
		{
			auto object = map->GetCell(x, y).object.get();
			if (auto mapEntity = dynamic_cast<MapEntity*>(object)) AddEntity(mapEntity);
		}
	}
	Kernel::SendMessageAll(make_s<EmptyMessage>(Message::Type::START));
}

}  // namespace strx
