#ifndef APPLICATION_H
#define APPLICATION_H

#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/all.h>
#include <Graph.h>
#include <FL/Fl_Scroll.H>
#include <iostream>
#include <fstream>



class Application : public bobcat::Application_ {
    bobcat::Window *window;
    bobcat::Dropdown *start;
    bobcat::Dropdown *end;
    bobcat::Dropdown *pref;
    bobcat::Button *search;
    bobcat::TextBox *stats;

    Fl_Scroll *results;

    ArrayList<Vertex*> timeCities;
    ArrayList<Vertex*> costCities;
    Graph timeGraph;
    Graph costGraph;

    void initData();
    void initInterface();

    void handleClick(bobcat::Widget *sender);
    
public:
    Application();

};

#endif