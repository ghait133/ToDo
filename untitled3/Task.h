//
// Created by Ghait Elghouate on 2019-03-02.
//
#include <iostream>
#include <string>

#ifndef UNTITLED3_TASK_H
#define UNTITLED3_TASK_H
using namespace std;

class Task {
private:
    string name;
    string note ="";
    string creationDate = "";
    string editDate="";
    bool done = false;
    int id;
public:
    Task(const string &name, const string &note, const string &creationDate, const string &editDate, bool done, int id);

public:
    void setCreationDate(const string &creationDate);

    void setEditDate(const string &editDate);

    void setId(int id);

public:
    Task(const string &name, const string &note, int id);

    const string &getName() const;

    const string &getNote() const;

    const string &getCreationDate() const;

    const string &getEditDate() const;

    bool isDone() const;

    int getId() const;

    void setName(const string &name);

    void setNote(const string &note);

    void setDone(bool done);

    string getDate();
};


#endif //UNTITLED3_TASK_H

