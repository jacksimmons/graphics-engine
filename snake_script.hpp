#pragma once


namespace Tank
{
	class Player;
}
class SnakeScript
{
	friend class Tank::Player;
private:
	static void handleMovement();

};