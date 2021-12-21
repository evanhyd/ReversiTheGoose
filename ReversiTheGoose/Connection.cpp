#include "Connection.h"
#include <random>
#include <ctime>


Connection::Connection() : diff_weight_(0.0)
{
	this->weight_ = double(rand()) / double(RAND_MAX);
}
