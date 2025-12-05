#include <Application.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/all.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace bobcat;
using namespace std;

Application::Application() {
    // App's constructor
    initData();
    initInterface();
}

void Application::handleClick(bobcat::Widget *sender) {

    results->clear();
    window->redraw();

    int startIndex = start->value();
    int destIndex = end->value();

    Waypoint *path = g.ucs(cities[startIndex], cities[destIndex]);

    system("clear");

    if (path) {
        cout << "We found a path" << endl;
        Waypoint *temp = path;
        int y = results->y() + 10;
        while (temp != nullptr) {
            results->add(new TextBox(40, y, 300, 25, temp->vertex->data));
            y += 40;
            if (temp->parent != nullptr) {
                results->add(new TextBox(
                    40, y, 300, 25,
                    "    Flight time: " + to_string(temp->weight) + " hours"));
                y += 40;
            }
            cout << temp->vertex->data << " " << temp->partialCost << endl;
            temp = temp->parent;

            window->redraw();
        }
    } else {
        cout << "There is no path" << endl;
    }    
}

void Application::initData() {
    std::ifstream vertices ("./assets/vertices.csv");
    std::string city;

    if(!vertices.is_open()) {
        throw std::logic_error("Error opening file!");
        return;
    }

    while(std::getline(vertices, city)) {
         cities.append(new Vertex(city)); 
    }

    vertices.close();

    for (int i = 0; i < cities.size(); i++) {
        g.addVertex(cities[i]);
    }

    std::ifstream edges ("./assets/edges.csv");
    std::string line;
    int city1;
    int city2;
    int duration;
    int price;

    while(std::getline(edges, line)) {
        std::stringstream ss(line);
        std::string item;
        if(std::getline(ss, item, ',')) {
            city1 = stoi(item);
        }
        if(std::getline(ss, item, ',')) {
            city2 = stoi(item);
        }
        if(std::getline(ss, item, ',')) {
            duration = stoi(item);
        }
        if(std::getline(ss, item, ',')) {
            price = stoi(item);
        }

        g.addEdge(cities[city1], cities[city2], duration);
    }

    edges.close();

}

void Application::initInterface() {
    window = new Window(100, 100, 400, 400, "Flight Planner");

    start = new Dropdown(20, 40, 360, 25, "Starting Point");
    end = new Dropdown(20, 100, 360, 25, "Destination");

    for (int i = 0; i < cities.size(); i++) {
        start->add(cities[i]->data);
        end->add(cities[i]->data);
    }

    search = new Button(20, 150, 360, 25, "Search");
    ON_CLICK(search, Application::handleClick);

    results = new Fl_Scroll(20, 200, 360, 180, "Results");
    results->align(FL_ALIGN_TOP_LEFT);
    results->box(FL_THIN_UP_BOX);

    window->show();
}