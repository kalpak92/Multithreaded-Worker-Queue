#include "workItem.hpp"

WorkItem::WorkItem(std::string message, int number) :
                message_(message), number_(number) {
}


std::string WorkItem::getMessage() {
    return message_;
}

int WorkItem::getNumber() {
    return number_;
}