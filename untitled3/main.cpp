#include <iostream>
#include <curses.h>
#include <menu.h>
#include <form.h>

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

void creatTaskinput(){
     FORM *form;
     FIELD *fields[5];
     WINDOW *win_body, *win_form;
    int ch;

    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);

    win_body = newwin(24, 80, 15, 50);
    box(win_body, 0, 0);
    win_form = derwin(win_body, 20, 78, 3, 1);
    box(win_form, 0, 0);
    mvwprintw(win_body, 1, 2, "Press F1 to quit and F2 to print fields content");

    fields[0] = new_field(1, 10, 0, 0, 0, 0);
    fields[1] = new_field(1, 40, 0, 15, 0, 0);
    fields[2] = new_field(1, 10, 2, 0, 0, 0);
    fields[3] = new_field(1, 40, 2, 15, 0, 0);
    fields[4] = NULL;

    set_field_buffer(fields[0], 0, "label1");
    set_field_buffer(fields[1], 0, "val1");
    set_field_buffer(fields[2], 0, "label2");
    set_field_buffer(fields[3], 0, "val2");

    set_field_opts(fields[0], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[1], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_opts(fields[2], O_VISIBLE | O_PUBLIC | O_AUTOSKIP);
    set_field_opts(fields[3], O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    set_field_back(fields[1], A_UNDERLINE);
    set_field_back(fields[3], A_UNDERLINE);

    form = new_form(fields);
    set_form_win(form, win_form);
    set_form_sub(form, derwin(win_form, 18, 76, 1, 1));
    post_form(form);

    refresh();
    wrefresh(win_body);
    wrefresh(win_form);
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

                // Delete the char before cursor
            case KEY_BACKSPACE:
            case 127:
                form_driver(form, REQ_DEL_PREV);
                break;

                // Delete the char under the cursor
            case KEY_DC:
                form_driver(form, REQ_DEL_CHAR);
                break;

            default:
                form_driver(form, ch);
                break;
        }


        wrefresh(win_form);
        if ( ch == 97){
            break;
        }else if (ch == KEY_F(2)){

            printw("%s", trim_whitespaces(field_buffer(fields[1], 0)));
            printw("%s", trim_whitespaces(field_buffer(fields[3], 0)));

            refresh();
            pos_form_cursor(form);
        }
    }

    printw("jetzt muss er weg1");
    unpost_form(form);
    free_form(form);
    free_field(fields[0]);
    free_field(fields[1]);
    free_field(fields[2]);
    free_field(fields[3]);
    delwin(win_form);
    werase(win_form);
    wrefresh(win_form);
    delwin(win_body);
    werase(win_body);
    wrefresh(win_body);
    endwin();

}

void taskmenu(){
    int ch;
    initscr();
    //atexit(quit);
    //clear();
    noecho();
    //curs_set(0);
    cbreak();
    nl();

    WINDOW *taskmenu =newwin(12, 40, 20, 35);

    string menu_item [3]={"Bearbeiten","Löchen","istFertig"};

    box(taskmenu, 0, 0);
    mvwprintw(taskmenu, 1, 5, "***** Testmenü *****");
    mvwprintw(taskmenu,9, 2, "Programm mittels Menü oder F1-Funk- ");
    mvwprintw(taskmenu,10,2,"tionstaste beenden");
    printw("hier is der menu");
    refresh();
    wrefresh(taskmenu);
    keypad(taskmenu, TRUE);
    free(taskmenu);

    endwin();
}

void taskTabel(){
    int ch;
    initscr();
    noecho();
    cbreak();

    int xmax, ymax;
    getmaxyx(stdscr, ymax,xmax);
    WINDOW * tasktabel = newwin(ymax-7,xmax-3,5,1);
    box(tasktabel,0,0);
    refresh();
    wrefresh(tasktabel);
    keypad(tasktabel, TRUE);

    printw(" world  ");

    free(tasktabel);

    endwin();
}
void hauptmenu(){
    int ch = 0;
    initscr();
    noecho();
    cbreak();

    WINDOW * menu = newwin(4,40,1,1);
    box(menu,0,0);
    refresh();
    wrefresh(menu);
    keypad(menu,true);
    string menuItem[2]={"Erstelle","Verlassen"};
    int heighlight = 0;
    do{
        for (int i = 0; i < 2; i++) {
            if (i == heighlight) {
                wattron(menu, A_REVERSE);
            }
            mvwprintw(menu,2, (i *20 )+ 3, menuItem[i].c_str());
            wattroff(menu, A_REVERSE);
        }
        ch = wgetch(menu);
        switch (ch) {
            case KEY_LEFT:
                heighlight--;
                if (heighlight == -1){
                    heighlight = 0;
                }
                break;
            case KEY_RIGHT:
                heighlight++;
                if (heighlight == 2){
                    heighlight = 1;
                }
                break;
            default:
                break;
        }
        refresh();
        wrefresh(menu);
        if ( ch == 10){
            if ( menuItem[heighlight].compare("Erstelle") == 0){
                creatTaskinput();
                wrefresh(menu);
                refresh();
                //mvprintw(15, 50, menuItem[heighlight].c_str());
            } else if (menuItem[heighlight].compare("Verlassen") == 0){
                break;
            }
        }

    }while(1);
    refresh();
    wrefresh(menu);
    endwin();

}
char mygetch(){
    int ch = getch();
    while (getch() != '\n');
    return ch;
}
int main() {
    int ch = 0;
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

    taskTabel();
    ch = getch();
    do{

        if ( ch == 97){
            hauptmenu();
            break;
        }if(ch == 103){
            break;
        }
    }while (1);


    //taskmenu();

    refresh();
    wrefresh(mainwin);

    getch();
    free(mainwin);
    endwin();


    return 0;
}