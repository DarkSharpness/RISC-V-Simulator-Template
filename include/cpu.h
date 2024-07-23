#include "module.h"
#include <memory>
#include <random>
#include <vector>

namespace dark {

class CPU {
private:
	std::vector<std::unique_ptr<ModuleBase>> modules;

public:
	unsigned long long cycles = 0;

private:
	void sync_all() {
		for (auto &module: modules)
			module->sync();
	}

public:
	void add_module(std::unique_ptr<ModuleBase> module) {
		modules.push_back(std::move(module));
	}
	void run_once() {
		++cycles;
		for (auto &module: modules)
			module->work();
		sync_all();
	}
	void run_once_shuffle() {
		static std::default_random_engine engine;
		std::vector<ModuleBase *> shuffled;
		shuffled.reserve(modules.size());
		for (auto &module: modules)
			shuffled.push_back(module.get());
		std::shuffle(shuffled.begin(), shuffled.end(), engine);

		++cycles;
		for (auto &module: shuffled)
			module->work();
		sync_all();
	}
	void run(unsigned long long max_cycles = 0, bool shuffle = false) {
		auto func = shuffle ? &CPU::run_once_shuffle : &CPU::run_once;
		while (max_cycles == 0 || cycles < max_cycles)
			(this->*func)();
	}
};

} // namespace dark
