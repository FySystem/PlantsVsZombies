#pragma once

namespace GameOffset
{
	//模块基地址偏移
	constexpr uintptr_t ModuleBase = 0x2a9ec0;

	//基地址偏移
	constexpr uintptr_t GameBase = 0x768;

	//僵尸基地址
	constexpr uintptr_t ZombieBase = 0x90;

	namespace Offset
	{
		//阳光偏移
		constexpr uintptr_t Sum = 0x5560;

		//坐标偏移
		constexpr uintptr_t ZombiePositionX = 0xB0C;

		//血量偏移
		constexpr uintptr_t ZomBieHealth = 0xBA8;

		//总僵尸数量
		constexpr uintptr_t NumOfZombie = 0xA0;

		//僵尸间隔偏移 
		constexpr uintptr_t zombieOffset = 0x15c;
	}
}

class Zombie
{
public:
	Zombie(const Vector::vec2& n_location, const Vector::Health<int>& n_health)
		:location(n_location), health(n_health)
	{}

	//坐标
	Vector::vec2 location;

	//血量
	Vector::Health<int> health;
};

class Infomation : public Singleton<Infomation>
{
public:
	void Execute();

	//僵尸列表
	std::vector<std::shared_ptr<Zombie>> ZombieList;
private:
	void upData();
};

