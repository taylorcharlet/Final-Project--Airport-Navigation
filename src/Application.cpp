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
    Waypoint *path;

    if(pref->value() == 0) {
        path = costGraph.ucs(cities2[startIndex], cities2[destIndex]);
        cout << costGraph << endl;
    } else if (pref->value() == 1) {
        path = timeGraph.bfs(cities[startIndex], cities[destIndex]);
        cout << "1" << endl;
    } else if (pref->value() == 2) {
        path = timeGraph.ucs(cities[startIndex], cities[destIndex]);
        cout << "2" << endl;
    }

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
                    "    Flight cost: $" + to_string(temp->weight)));
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
         cities2.append(new Vertex(city));
    }

    vertices.close();

    for (int i = 0; i < cities.size(); i++) {
        timeGraph.addVertex(cities[i]);
        costGraph.addVertex(cities2[i]);
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

        timeGraph.addEdge(cities[city1], cities[city2], duration);
        costGraph.addEdge(cities2[city1], cities2[city2], price);
        
    }
    // for(int i = 0; i < costGraph.vertices.size(); i++){
    //     cout << costGraph.vertices[i]->edgeList << endl;
    // }
    

    edges.close();

}

void Application::initInterface() {
    window = new Window(100, 100, 400, 430, "Flight Planner");

    start = new Dropdown(20, 30, 360, 25, "Departure");
    end = new Dropdown(20, 80, 360, 25, "Destination");
    pref = new Dropdown(20, 130, 360, 25, "Preference");

    for (int i = 0; i < cities.size(); i++) {
        start->add(cities[i]->data);
        end->add(cities[i]->data);
    }

    pref->add("Cheapest Price");
    pref->add("Fewest Stops");
    pref->add("Shortest Travel Time");

    search = new Button(20, 170, 360, 25, "Search");
    ON_CLICK(search, Application::handleClick);

    results = new Fl_Scroll(20, 220, 360, 180, "Results");
    results->align(FL_ALIGN_TOP_LEFT);
    results->box(FL_THIN_UP_BOX);

    window->show();
}