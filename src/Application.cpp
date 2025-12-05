#include <Application.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/all.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace bobcat;
using namespace std;

Application::Application() {
    // App's constructor
    initData();
    initInterface();
}

void Application::initData() {
    std::ifstream file ("./assets/vertices.csv");
    std::string city;

    if(!file.is_open()) {
        throw std::logic_error("Error opening file!");
        return;
    }

    while(std::getline(file, city)) {
         cities.append(new Vertex(city)); 
    }

    file.close();

    // for (int i = 0; i < cities.size(); i++) {
    //     g.addVertex(cities[i]);
    // }
}

void Application::initInterface() {
    window = new Window(100, 100, 400, 400, "Flight Planner");

    start = new Dropdown(20, 40, 360, 25, "Starting Point");
    end = new Dropdown(20, 100, 360, 25, "Destination");

    for (int i = 0; i < cities.size(); i++) {
        start->add(cities[i]->data);
        end->add(cities[i]->data);
    }

    // search = new Button(20, 150, 360, 25, "Search");
    // ON_CLICK(search, Application::handleClick);

    // results = new Fl_Scroll(20, 200, 360, 180, "Results");
    // results->align(FL_ALIGN_TOP_LEFT);
    // results->box(FL_THIN_UP_BOX);

    window->show();
}