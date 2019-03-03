//
// Created by Ghait Elghouate on 2019-03-02.
//

#ifndef UNTITLED3_TASKLIST_H
#define UNTITLED3_TASKLIST_H


#include <vector>
#include "Task.h"
using namespace std;
class TaskList {
private:
    vector<Task *> taskList;
public:
    void setTaskList(vector<Task *> taskList);

    vector<Task *> getTaskList();

    int getTaskListSize();

};


#endif //UNTITLED3_TASKLIST_H
