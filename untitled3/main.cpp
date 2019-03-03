#include <iostream>
#include <curses.h>
#include <menu.h>
#include <form.h>
#include <vector>
#include <zconf.h>
#include <fstream>
#include <sstream>
#include "TaskList.h"

using namespace std;
static char* trim_whitespaces(char *str)
{
    char *end;

    // trim leading space
    while(isspace(*str))
        str++;

    if(*str == 0) // all spaces?
        return str;

    // trim trailing space
    end = str + strnlen(str, 128) - 1;

    while(end > str && isspace(*end))
        end--;

    // write new null terminator
    *(end+1) = '\0';

    return str;
}

string * creatTaskinput(){

     FORM *form;
     FIELD *fields[5];
     WINDOW *win_body, *win_form;
    int ch;

    initscr();
    noecho();
    cbreak();
    curs_set(1);
    keypad(stdscr, TRUE);
    int xmax, ymax;
    getmaxyx(stdscr, ymax,xmax);
    win_body = newwin(16, 60, ymax/2 - 6, xmax/2 - 30);
    box(win_body, 0, 0);
    win_form = derwin(win_body, 12, 58, 3, 1);
    box(win_form, 0, 0);
    mvwprintw(win_body, 1, 2, "Fenster mittels F1-Funktionstaste beenden");
    mvwprintw(win_body, 2, 2, "Bestätigen und speichern mit Enter");

    fields[0] = new_field(1, 10, 0, 0, 0, 0);
    fields[1] = new_field(1, 40, 0, 15, 0, 0);
    fields[2] = new_field(1, 10, 2, 0, 0, 0);
    fields[3] = new_field(1, 40, 2, 15, 0, 0);
    fields[4] = NULL;

    set_field_buffer(fields[0], 0, "Titel");
    set_field_buffer(fields[1], 0, "");
    set_field_buffer(fields[2], 0, "Bemerkung");
    set_field_buffer(fields[3], 0, "");

    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[3], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    set_field_back(fields[1], A_UNDERLINE);
    set_field_back(fields[3], A_UNDERLINE);

    form = new_form(fields);
    set_form_win(form, win_form);
    set_form_sub(form, derwin(win_form, 10, 56, 1, 1));
    post_form(form);

    refresh();
    touchwin(win_form);
    wrefresh(win_body);
    wrefresh(win_form);
    string * taskData = new string [2];
    while (1){
        ch = getch();
        switch (ch) {
            case KEY_DOWN:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;

            case KEY_UP:
                form_driver(form, REQ_NEXT_FIELD);
                form_driver(form, REQ_END_LINE);
                break;

            case KEY_LEFT:
                form_driver(form, REQ_PREV_CHAR);
                break;

            case KEY_RIGHT:
                form_driver(form, REQ_NEXT_CHAR);
                break;

            case KEY_BACKSPACE:
            case 127:
                form_driver(form, REQ_DEL_PREV);
                break;

            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;

            default:
                form_driver(form, ch);
                break;
        }


        wrefresh(win_form);
        if ( ch == KEY_F(1)){
            taskData = NULL;
            break;
        }else if (ch == 10){
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_PREV_FIELD);

            taskData[0] = (trim_whitespaces(field_buffer(fields[1], 0)));
            taskData[1] = (trim_whitespaces(field_buffer(fields[3], 0)));

            if (strcmp("",(trim_whitespaces(field_buffer(fields[1], 0)))) == 0){
                mvwprintw(win_form, 8, 2, "%s","Ein Titel für die Aufgabe ist Erforderlich");
                mvwprintw(win_form, 9, 2, "%s","zum abbrechen F1 drucken");
            } else{
                break;
            }

        }
    }

    unpost_form(form);

    free_field(fields[0]);
    free_field(fields[1]);
    free_field(fields[2]);
    free_field(fields[3]);
    free_form(form);



    werase(win_form);
    werase(win_body);
    delwin(win_form);
    delwin(win_body);

    endwin();
    return taskData;
}

string * taskmenu( TaskList *pList, int id){
    string * taskData;
    int ch;
    initscr();
    noecho();
    curs_set(0);
    cbreak();
    nl();
    int xmax, ymax;
    getmaxyx(stdscr, ymax,xmax);
    WINDOW *taskmenu =newwin(12, 40, ymax/2 - 6, xmax/2 - 20);


    box(taskmenu, 0, 0);
    mvwprintw(taskmenu, 1, 3, "*********** Testmenu ************");
    mvwprintw(taskmenu,9, 2, "Fenster mittels F1-Funktionstaste");
    mvwprintw(taskmenu,10,2,"beenden");
    wrefresh(taskmenu);
    keypad(taskmenu, TRUE);
    string menu_item [3]={"Bearbeiten","Loeschen","istFertig"};
    int heighlight = 0;
    while (1){
        for (int i= 0 ; i < 3; i++) {
            if (i == heighlight) {
                wattron(taskmenu, A_REVERSE);
            }
            mvwprintw(taskmenu, i + 4, 15, menu_item[i].c_str());
            wattroff(taskmenu, A_REVERSE);
        }
        ch = wgetch(taskmenu);
        switch (ch){
            case  259:
                heighlight--;
                if (heighlight == -1)
                    heighlight = 0;
                break;
            case 258:
                heighlight++;
                if  (heighlight == 3)
                    heighlight =2;
                break;
            default:
                break;
        }
        refresh();
        wrefresh(taskmenu);

        if (ch == 10){
            if ( menu_item[heighlight].compare("Bearbeiten") == 0){
                string * taskData = creatTaskinput();
                if (taskData  != NULL){
                    vector<Task*> taskList = pList->getTaskList();
                    for (int i = 0; i < taskList.size();i++){
                        if (taskList[i]->getId() == id){
                            taskList[i]->setName(taskData[0]);
                            taskList[i]->setNote(taskData[1]);
                            pList->setTaskList(taskList);
                        }
                    }
                }
                werase(taskmenu);
                wrefresh(taskmenu);
                endwin();
                break;
            } else if (menu_item[heighlight].compare("Loeschen") == 0){
                vector<Task*> taskList = pList->getTaskList();
                for (int i = 0; i < taskList.size();i++) {
                    if (taskList[i]->getId() == id) {
                        taskList.erase(taskList.begin() + i);
                        pList->setTaskList(taskList);
                    }
                }
                break;
            } else if (menu_item[heighlight].compare("istFertig") == 0){
                vector<Task*> taskList = pList->getTaskList();
                for (int i = 0; i < taskList.size();i++){
                    if (taskList[i]->getId() == id){
                        bool x = true;
                        taskList[i]->setDone(x);
                        pList->setTaskList(taskList);
                    }
                }
                refresh();
                wrefresh(taskmenu);
                break;
            }
        }else if(ch == KEY_F(1)){
            break;
        }

    }

    refresh();
    werase(taskmenu);
    delwin(taskmenu);
    endwin();
    return taskData;
}


char *repeat(char c, int w) {
    char *retval =(char*) malloc(w);
    if (retval == NULL) {
        return retval;
    }
    memset(retval, c, w);
    retval[w] = '\0';
    return retval   ;
}
int getColumnSize(int prozendwert){
    int size = 0;
    int xmax = getmaxx(stdscr) - 8;
    size = prozendwert * xmax / 100;
    return size;
}

void taskTabel(TaskList *pList) {

    vector<Task *> taskList = pList->getTaskList();
    int ch;
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);



    int xmax, ymax;
    getmaxyx(stdscr, ymax,xmax);
    WINDOW * tasktabel = newwin(ymax-7,xmax-3,5,1);
    WINDOW * showNotWin = newwin(4,xmax-3 -61,1,62);
    WINDOW * newTaskDialog = newwin(4,60,1,1);
    box(tasktabel,0,0);
    box(showNotWin,0,0);
    box(newTaskDialog,0,0);

    //wbkgd(tasktabel,COLOR_PAIR(1));

    keypad(showNotWin, TRUE);
    keypad(tasktabel, TRUE);
    keypad(newTaskDialog, TRUE);

    refresh();
    wrefresh(tasktabel);
    wrefresh(showNotWin);
    wrefresh(newTaskDialog);
    mvwprintw(newTaskDialog,1,1,"Zum aufgaben Erstellen 'a'  drucken");
    mvwprintw(newTaskDialog,2,1,"Zum program verlassen F1 drucken");


    refresh();
    wrefresh(tasktabel);
    wrefresh(showNotWin);
    wrefresh(newTaskDialog);

    int heighlight = 0;
    while (1) {

        vector<Task *> taskList = pList->getTaskList();
        wclear(tasktabel);
        box(tasktabel, 0, 0);
        wattron(tasktabel,COLOR_PAIR(3));

        mvwprintw(tasktabel, 1, 1, "%*s|%*s|%*s|%*s|%*s|%*s"
                                                            , -getColumnSize(4), " Id"
                                                            , -getColumnSize(44)-2, " Titel"
                                                            ,-getColumnSize(5), " Note"
                                                            , -getColumnSize(7), " Status"
                                                            , -getColumnSize(20), " Erstellungsdatum"
                                                            ,-getColumnSize(20), "Aenderungsdatum"
                                                            );
        mvwprintw(tasktabel, 2, 1, repeat('-', xmax - 5), "\n");

        if (pList->getTaskListSize()!= 0) {
            for (int i = 0; i < taskList.size(); i++) {
                if (i == heighlight) {
                    wattron(tasktabel, A_REVERSE);
                }
                bool note = taskList[i]->getNote().compare("");

                if (taskList[i]->isDone()){
                    wattron(tasktabel,COLOR_PAIR(2));
                    mvwprintw(tasktabel, i + 3, 1, "%*d|%*s|%*s|%*s|%*s|%*s"
                            , -getColumnSize(4), taskList[i]->getId()
                            ,-getColumnSize(44)-2, taskList[i]->getName().c_str()
                            , -getColumnSize(5), note ? "   X" : "   -"
                            ,-getColumnSize(7), taskList[i]->isDone() ? "Erledigt" : "ist offen"
                            , -getColumnSize(20),taskList[i]->getCreationDate().c_str()
                            , -getColumnSize(20), taskList[i]->getEditDate().c_str()
                    );
                    wattron(tasktabel,COLOR_PAIR(3));
                } else{
                    wattron(tasktabel,COLOR_PAIR(1));
                    mvwprintw(tasktabel, i + 3, 1, "%*d|%*s|%*s|%*s|%*s|%*s"
                            , -getColumnSize(4), taskList[i]->getId()
                            ,-getColumnSize(44)-2, taskList[i]->getName().c_str()
                            , -getColumnSize(5), note ? "   X" : "   -"
                            ,-getColumnSize(7), taskList[i]->isDone() ? "Erledigt" : "ist offen"
                            , -getColumnSize(20),taskList[i]->getCreationDate().c_str()
                            , -getColumnSize(20), taskList[i]->getEditDate().c_str()
                    );
                    wattron(tasktabel,COLOR_PAIR(3));
                }

                wattroff(tasktabel, A_REVERSE);
            }
            ch = wgetch(tasktabel);
            switch (ch) {
                case 259:
                    heighlight--;
                    if (heighlight == -1)
                        heighlight = 0;
                    wclear(showNotWin);
                    box(showNotWin, 0, 0);
                    mvwprintw(showNotWin, 1, 1, taskList[heighlight]->getNote().c_str());
                    refresh();
                    wrefresh(showNotWin);

                    break;
                case 258:
                    heighlight++;
                    if (heighlight == taskList.size())
                        heighlight = taskList.size() - 1;
                    wclear(showNotWin);
                    box(showNotWin, 0, 0);
                    mvwprintw(showNotWin, 1, 1, taskList[heighlight]->getNote().c_str());
                    refresh();
                    wrefresh(showNotWin);

                    break;
                default:
                    break;
            }

            if (ch == 10) {
                taskmenu(pList, taskList[heighlight]->getId());

                if (pList->getTaskList().size() != taskList.size()) {
                    heighlight--;
                    if (heighlight == -1)
                        heighlight = 0;
                }

                curs_set(0);

            } else if (ch == 97) {
                string *taskData = creatTaskinput();
                if(taskData != NULL){
                    Task *t = new Task(taskData[0], taskData[1], taskList.size() + 1);
                    taskList.push_back(t);
                    pList->setTaskList(taskList);
                }

            } else if (ch == KEY_F(1)) {
                break;
            }
        } else{
            mvwprintw(tasktabel,4,1,"Ihre ToDo List ist noch leer, 'a' drucken um neue Aufgaben zu erstellen" );
            ch = wgetch(tasktabel);
            if(ch ==97){
                string *taskData = creatTaskinput();
                if (taskData != NULL){
                    Task *t = new Task(taskData[0], taskData[1], taskList.size() + 1);
                    taskList.push_back(t);
                    pList->setTaskList(taskList);
                }

            }else if (ch == KEY_F(1)) {
                break;
            }
        }
    }

    werase(tasktabel);
    delwin(tasktabel);
    endwin();
}

int main() {
    TaskList *l = new TaskList();
    vector <Task *> localList1;
    ifstream myfile ("example.csv");
    std::string                line;

    while ( getline (myfile,line) )
    {
        std::stringstream          lineStream(line);
        std::string                cell;
        std::vector<std::string>   result;
        while(std::getline(lineStream,cell,'|'))
        {
            result.push_back(cell);
        }
        // This checks for a trailing comma with no data after it.
        if (!lineStream && cell.empty())
        {
            // If there was a trailing comma then add an empty element.
            result.push_back("");
        }
        if (result.size()!=0){
            Task * t = new Task (result[1],result[2],result[3],result[4],stoi(result[5]),stoi(result[0]));
            localList1.push_back(t);
        }

    }
    l->setTaskList(localList1);
    myfile.close();

    initscr();
    noecho();
    cbreak();
    int xmax, ymax;
    getmaxyx(stdscr, ymax,xmax);
    WINDOW * mainwin = newwin(ymax-1,xmax-1,0,0);
    box(mainwin,0,0);
    refresh();
    wrefresh(mainwin);
    keypad(mainwin, TRUE);
    taskTabel(l);

    localList1 = l->getTaskList();
    std::ofstream file;
    file.open ("example.csv");
    for (int i = 0; i<localList1.size();i++){

        file << localList1[i]->getId()<<"|"
             << localList1[i]->getName() <<"|"
             << localList1[i]->getNote() <<"|"
             << localList1[i]->getCreationDate()<<"|"
             << localList1[i]->getEditDate()<<"|"
             << localList1[i]->isDone()<<endl;
    }
    file.close();
    refresh();
    wrefresh(mainwin);

    werase(mainwin);
    delwin(mainwin);
    endwin();


    return 0;
}