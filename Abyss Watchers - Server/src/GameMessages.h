#pragma once

#include <MessageIdentifiers.h>

enum GameMessages {
	ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1,
	ID_SERVER_CLIENT_ID = ID_USER_PACKET_ENUM + 2,
	ID_CLIENT_INPUT_RECEIVED = ID_USER_PACKET_ENUM + 3,
	ID_SERVER_GAME_DATA = ID_USER_PACKET_ENUM + 4,
	ID_SERVER_ASSIGNED_PLAYER = ID_USER_PACKET_ENUM + 5,
	ID_SERVER_ASSIGNED_SPECTATOR = ID_USER_PACKET_ENUM + 6,
};