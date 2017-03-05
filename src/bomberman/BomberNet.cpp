#include "BomberNet.h"

BomberNet BomberNet::m_instance = BomberNet();

BomberNet& BomberNet::Instance() {
	return m_instance;
}

BomberNet::BomberNet() {
}

BomberNet::~BomberNet() {
}
