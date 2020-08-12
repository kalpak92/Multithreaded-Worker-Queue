#ifndef WORKITEM_HPP
#define WORKITEM_HPP

#include <string>

class WorkItem {
public:
    WorkItem(std::string message, int number);

    std::string getMessage();
    int getNumber();

private:
    std::string message_;
    int number_;
};

#endif