#include <stdint.h>
#include <stdio.h>
#include <ncurses.h>
#include <form.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/unistd.h> 
int spells(int h,int w){
  WINDOW * term = newwin(3,20,(h/2)-1,(w/2)-10);
  cbreak();
  noecho();
  keypad(stdscr,TRUE);


    if ( start_color() == ERR || !has_colors() || !can_change_color() ) {
        delwin(term);
        endwin();
        refresh();
        fputs("Could not use colors.", stderr);
        exit(EXIT_FAILURE);
    }

  init_pair(1, COLOR_WHITE, COLOR_BLACK);
    wbkgd(term, COLOR_PAIR(1));


    /*  Draw box  */

    box(term, 0, 0);
    wrefresh(term);
  int ch;
  int x = 1,y = 2;
  wmove(term,x,y);
  wrefresh(term);
  char * buffer;
  while (1) {
    
    ch = getch();
    if(ch == KEY_ENTER){
      if(strcmp(buffer,"stop") == 0){
	break;
      }

    }
    else if (ch == KEY_BACKSPACE && y >= 2){
      buffer--;
      *buffer = '\0';
      wmove(term,x,--y);
      waddch(term,' ');
    }
    else if(y < 18 && y >= 2){

      waddch(term,ch);
    *buffer = ch;
    buffer++;
    *buffer = '\0';
    y++;
   
      
    }
    wrefresh(term);
     }
	endwin();
	return 0;

}
int main(int argc, char *argv[])
{
  printf("this is going to be my game . . . \n");

  initscr();
  cbreak();
  noecho();
  keypad(stdscr,TRUE);
  int h,w;
  int x = 5;
  int y = 5;
  getmaxyx(stdscr,h,w);
  WINDOW * win = newwin(h,w,0,0);
  wmove(win,x,y);
  wrefresh(win);
while (1) {
  int val = getch();
  switch (val) {
  case KEY_DOWN: {
    x++;
    break;
  }
  case KEY_UP : {
    x--;
    break;
  }
  case KEY_RIGHT : {
    y++;
    break;
  }
  case KEY_LEFT : {
    y--;
    break;
  }
  case 't' : {
    spells(h,w);
    

    break;
  }
default:
    break;
  }

  wmove(win,x,y);
  wrefresh(win);

 }
  
  endwin();    


  return 0;
}
