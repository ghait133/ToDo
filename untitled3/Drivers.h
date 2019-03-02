//
// Created by Ghait Elghouate on 2019-03-01.
//

#ifndef UNTITLED3_DRIVERS_H
#define UNTITLED3_DRIVERS_H

#include <string>
#include <curses.h>

using namespace std;
class Drivers {
public:
    void hauptMenuDriver(int ch, WINDOW*menu ){
        keypad(menu,true);
        string menuItem[2]={"Erstelle","Verlassen"};
        int heighlight = 0;
        while(1){
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
                mvprintw(15, 50, menuItem[heighlight].c_str());
                wrefresh(menu);
                refresh();
                break;
            }

        }
    }
};


#endif //UNTITLED3_DRIVERS_H
