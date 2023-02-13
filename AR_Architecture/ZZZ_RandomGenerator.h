#pragma once
#include <random>
#include <cstdint>

class ZZZ_RandomGenerator {
	std::mt19937 mt;
	std::random_device rd;
public:
	ZZZ_RandomGenerator() { mt.seed(rd()); }

	void seed() { mt.seed(rd()); }
	void seed(const std::uint_fast32_t seed_) { mt.seed(seed_); }

	std::uint_fast32_t operator()(){ return mt(); }

	//Random number generation between two values (INT)
	std::int_fast32_t operator()(const std::int_fast32_t min_, const std::int_fast32_t max_)
	{
		std::uniform_int_distribution<> uid((min_ <= max_) ? min_ : max_, (min_ <= max_) ? max_ : min_);
		return uid(mt);
	}
	//Random number generation between two values (INT)
	std::double_t operator()(const std::double_t min_, const std::double_t max_)
	{
		std::uniform_real_distribution<> uid((min_ <= max_) ? min_ : max_, (min_ <= max_) ? max_ : min_);
		return uid(mt);
	}
};
static thread_local ZZZ_RandomGenerator rnd;
