#include "Rifle.h"

Rifle::Rifle(int ownerId) : Gun(ownerId) {

}

void Rifle::pullTrigger1() {
	cout << "pew" << endl;
}

void Rifle::releaseTrigger1() {
	cout << "whoosh" << endl;
}

void Rifle::pullTrigger2() {

}

void Rifle::releaseTrigger2() {

}
