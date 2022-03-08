#include "pch.h"
#include "framework.h"

void Infomation::Execute()
{
	std::thread(&Infomation::upData, this).detach();
}

void Infomation::upData()
{
	const auto ModuleBase = reinterpret_cast<uintptr_t>(GetModuleHandle("PlantsVsZombies.exe"));

	while (true)
	{
		auto base = Memory::Read<uintptr_t>(ModuleBase + GameOffset::ModuleBase);
		base = Memory::Read<uintptr_t>(base + GameOffset::GameBase);

		const uintptr_t NumOfZombie = Memory::Read<int>(base + GameOffset::Offset::NumOfZombie);

		const auto ZombieBase = Memory::Read<uintptr_t>(base + GameOffset::ZombieBase);

		std::vector<std::shared_ptr<Zombie>> tempList;
		for (size_t i = 0; i < NumOfZombie; i++)
		{
			tempList.emplace_back(
				std::make_shared<Zombie>(
					Memory::Read<Vector::vec2>(ZombieBase + GameOffset::Offset::ZombiePositionX - i * GameOffset::Offset::zombieOffset),
					Memory::Read<Vector::Health<int>>(ZombieBase + GameOffset::Offset::ZomBieHealth - i * GameOffset::Offset::zombieOffset)
					)
			);
		}
		ZombieList = tempList;
	}
}
