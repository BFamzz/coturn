#include <mutex>
#include <unordered_map>
#include <utility>

// #include <App.h>

#include "headers/crow_all.h"
#include "headers/nlohmann/json.hpp"
#include "/usr/src/acl/lib_acl_cpp/include/acl_cpp/lib_acl.hpp"

int main(int argc, char *argv[])
{
	crow::SimpleApp app;
	std::mutex mtx;
	std::string uid;
	std::unordered_map<std::string, std::pair<crow::websocket::connection *, std::string>> activePeers;
	std::unordered_map<std::string, std::string> peerPairs;

	/* Connection to redis database */


	/*********************************** CHAT WEBSOCKET ******************************************/
	CROW_ROUTE(app, "/api/v1/hospitals/call")
		.websocket()
		.onaccept([&](const crow::request &req){
			std::lock_guard<std::mutex> _(mtx);

			/* Logging */
			std::cout << "websocket...onaccept()\n";

			std::string authToken = req.url_params.get("auth_token");
			if (authToken.empty())
			{
				return false;
			}
			/* Logging */
			std::cout << "auth_token is: " << authToken << "\n";

			/* Verify JWT here */

			uid = req.url_params.get("uid");
			/* Logging */
			std::cout << "uid is: " << uid << "\n";
			return true;
		})
		.onopen([&](crow::websocket::connection &conn){
			std::lock_guard<std::mutex> _(mtx);
			activePeers.insert(uid, make_pair(&conn, ""));

			/* Send unique turn-server credentials */
			

		})
		.onclose([&](crow::websocket::connection &conn, const std::string &reason) {
			std::lock_guard<std::mutex> _(mtx);

			/* Logging */
			std::cout << "websocket...onclose()\n";
			activePeers.erase(&conn);

		})
		.onmessage([&](crow::websocket::connection &conn, const std::string &data, bool is_binary) {
			std::lock_guard<std::mutex> _(mtx);
			if(is_binary) 
			{
				return;
			}

			nlohmann::json jsonMessage = nlohmann::json:parse(data);

			/* If user session is not null, forward the message to the other peer */
			std::unordered_map<std::string, std::pair<crow::websocket::connection *, 
				std::string>>::const_iterator currentPeer = activePeers.find(jsonMessage["caller"]);

			if (currentPeer == activePeers.end())
			{
				conn.close();
				return;
			}

			/* Check if the session is not empty or status == 'session' */
			if (!currentPeer->second->second.empty())
			{
				std::unordered_map<std::string, std::string>::const_iterator otherPeerId = 
					peerPairs.find(jsonMessage["caller"]);
				std::unordered_map<std::string, std::pair<crow::websocket::connection *, 
				std::string>>::const_iterator otherPeer = activePeers.find(otherPeerId->second);

				if (!otherPeer->second->second.empty())
				{
					currentPeer->second->first->send_text(data);
					return;
				}
				else
				{
					currentPeer->second->first->close();
					return;
				}
			}

			/* Check message */
			else if (jsonMessage["purpose"] == "INITIATE_CALL")
			{
				std::unordered_map<std::string, std::pair<crow::websocket::connection *>>::const_iterator otherPeer
					= activePeers.find(jsonMessage["recipient"]);

				/* Get other peer status from Redis jsonMessage["recipient"] */
				if (otherPeer.status != "session" && currentPeer.status != "session")
				{
					currentPeer->second->second->send_text("BUSY");

					/* Close websocket connection */
					activePeers.erase([message[uid]]);
					return;
				}
				currentPeer->second->second->send_text("CALL_INITIATED");

				/* Register session */
				peerPairs.insert({jsonMessage["caller"], jsonMessage["recipient"]});
				currentPeer->second->second = "session";
				peerPairs.insert({jsonMessage["recipient"], jsonMessage["caller"]});
				otherPeer->second->second = "session";
			}

			/* Get the recipient of the call */
			/* */
		});

	char* port = getenv("PORT");
	uint16_t iPort = static_cast<uint16_t>(port != NULL ? std::stoi(port) : 18080);

	std::cout << "PORT = " << iPort << "\n";

	app.port(iPort).multithreaded().run();
	return 0;
}

























/* ws->getUserData returns one of these */
// typedef struct {
// 	std::string uid,
// 	std::string session
// } PerSocketData;

// int main(int argc, char *argv[])
// {
// 	/* Handler for processing connection */
// 	uWS::App().ws<PerSocketData>("/call", {
// 		/* settings */
// 		.compression = uWS::DEDICATED_COMPRESSOR_3KB,
// 		.maxPayloadLength = 16 * 1024 * 1024,
// 		.idleTimeout = 10,
// 		.maxBackPressure = 1 * 1024 * 1024,
// 		/* Handlers */
// 		.open = [](auto *ws) {

// 		},
// 		.message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {

// 		},
// 		.drain = [](auto *ws) {
//             /* Check getBufferedAmount here */
//         },
//         .ping = [](auto *ws) {

//         },
//         .pong = [](auto *ws) {

//         },
//         .close = [](auto *ws, int code, std::string_view message) {
//             /* We automatically unsubscribe from any topic here */
//         }
// 	}).listen
// 	return 0;
// }