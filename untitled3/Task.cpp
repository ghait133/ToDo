//
// Created by Ghait Elghouate on 2019-03-02.
//

#include "Task.h"
#include "TaskList.h"

Task::Task(const string &name, const string &note, int id) : name(name), note(note), id(id) {
    this->name  = name;
    this-> note = note;
    this->id = id;
    this->creationDate = getDate();
}
const string &Task::getName() const {
    return name;
}

const string &Task::getNote() const {
    return note;
}

const string &Task::getCreationDate() const {
    return creationDate;
}

const string &Task::getEditDate() const {
    return editDate;
}

bool Task::isDone() const {
    return done;
}

int Task::getId() const {
    return id;
}

void Task::setName(const string &name) {
    Task::name = name;
    this->editDate = getDate();
}

void Task::setNote(const string &note) {
    Task::note = note;
    this->editDate = getDate();
}

void Task::setDone(bool done) {
    Task::done = done;
    this->editDate = getDate();
}
string Task::getDate() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80," %m-%d-%y um %H:%M.",timeinfo);

    return buffer;
}

void Task::setCreationDate(const string &creationDate) {
    Task::creationDate = creationDate;
}

void Task::setEditDate(const string &editDate) {
    Task::editDate = editDate;
}

void Task::setId(int id) {
    Task::id = id;
}

Task::Task(const string &name, const string &note, const string &creationDate, const string &editDate, bool done,
           int id) : name(name), note(note), creationDate(creationDate), editDate(editDate), done(done), id(id) {}
