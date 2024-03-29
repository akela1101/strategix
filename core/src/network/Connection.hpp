#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <mutex>
#include <nya/invoker.hpp>

#include "../../strx/NetworkCommon.hpp"
#include "../../strx/strx_fwd.hpp"


namespace strx
{
class Connection final : boost::noncopyable
{
public:
	using ReceiveCallback = function<void(s_p<Message>, ConnectionId)>;
	using ClosedCallback = function<void(ConnectionId id)>;

private:
	ConnectionId id;
	tcp::socket socket;
	mutable mutex readMutex;
	const ReceiveCallback receiveCallback;
	const ClosedCallback closedCallback;

	string writeBuffer;
	string readBuffer;
	int expectedSize;

public:
	Connection(tcp::socket socket, ReceiveCallback receiveCallback, ClosedCallback closedCallback = {});
	~Connection();

	ConnectionId GetId() const { return id; }
	void Write(s_p<Message> message);

private:
	void Read();
	bool LockedRead(boost::system::error_code ec) noexcept;
	bool OneRead(boost::system::error_code ec);
};
}  // namespace strx
