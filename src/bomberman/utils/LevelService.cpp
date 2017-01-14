#include "LevelService.h"

LevelService LevelService::m_instance = LevelService();

LevelService::LevelService() {
	fprintf(stderr, "Init LevelService system\n");
}

LevelService::~LevelService() {
	fprintf(stderr, "close LevelService system\n");
}

LevelService& LevelService::Instance() {
	return m_instance;
}
