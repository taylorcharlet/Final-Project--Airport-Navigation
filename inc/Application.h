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

    ArrayList<Vertex*> cities;
    Graph g;

    void initData();
    void initInterface();

public:
    Application();

};

#endif