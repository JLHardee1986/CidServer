#include <iostream>
#include <cid_net.h>

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

class CustomServer : public cid::net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : cid::net::server_interface<CustomMsgTypes>(nPort)
	{

	}
protected:
	virtual bool OnClientConnect(std::shared_ptr<cid::net::connection<CustomMsgTypes> > client)
	{
		cid::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// called when a client apppears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<cid::net::connection<CustomMsgTypes> > client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	virtual void OnMessage(std::shared_ptr<cid::net::connection<CustomMsgTypes> > client, cid::net::message<CustomMsgTypes>& msg)
	{
		cid::net::message<CustomMsgTypes> msgAll{};

		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerPing:
			std::cout << "[" << client->GetID() << "]: Server Ping\n";

			// simple bounce message back to client
			client->Send(msg);

			break;
		case CustomMsgTypes::ServerDeny:
			break;
		case CustomMsgTypes::MessageAll:
			std::cout << "[" << client->GetID() << "]: Message All\n";

			msgAll.header.id = CustomMsgTypes::ServerMessage;
			msgAll << client->GetID();
			MessageAllClients(msgAll, client);

			break;
		case CustomMsgTypes::ServerMessage:
			break;
		default:
			break;
		}
	}



};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);

	}
	return 0;
}