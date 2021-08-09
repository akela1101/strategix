#pragma once

#include "NetworkCommon.h"
#include <Strategix_fwd.h>
#include <nya/invoker.hpp>

namespace strx
{
class Client : public nya::static_invoker<Client, boost::asio::io_context>
{
	friend class GameSlot;
	Client() = delete;

public:
	static void StartSession(GameSlot* game);
	static void StopSession();

	static void SendMessageOne(s_p<Message> message);
	static void ReceiveMessage(s_p<Message> message, PlayerId id);
};
}  // namespace strx
