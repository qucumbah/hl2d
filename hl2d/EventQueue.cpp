#include "EventQueue.h"

int EventQueue::_precision;

steady_clock::time_point EventQueue::_start;
steady_clock::time_point EventQueue::_now;

map<int, EventQueue::actionInfo> EventQueue::_actions;
int EventQueue::_lastId;