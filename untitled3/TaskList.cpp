//
// Created by Ghait Elghouate on 2019-03-02.
//

#include "TaskList.h"

vector<Task *> TaskList::getTaskList() {
    return taskList;
}

void TaskList::setTaskList(vector<Task *> taskList) {
    TaskList::taskList = taskList;
}
int TaskList::getTaskListSize(){
    return this->taskList.size();
}
