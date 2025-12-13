#include <Application.h>
#include <FL/Enumerations.H>
#include <FL/Fl_Scroll.H>
#include <bobcat_ui/bobcat_ui.h>
#include <bobcat_ui/all.h>
#include <stdexcept>
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
        path = timeGraph.ucs2(timeCities[startIndex], timeCities[destIndex]);
    } else if (pref->value() == 1) {
        path = timeGraph.bfs(timeCities[startIndex], timeCities[destIndex]);
        cout << "1" << endl;
    } else if (pref->value() == 2) {
        path = timeGraph.ucs(timeCities[startIndex], timeCities[destIndex]);
        cout << "2" << endl;
    }

    system("clear");
    if (path) {
        cout << "We found a path" << endl;
        Waypoint *temp = path;
        Stack<Waypoint*> stops;
        Stack<Vertex*> stopsAgain;
        int y = results->y() + 10;
        while (temp != nullptr) {
            stops.push(temp);
            stopsAgain.push(temp->vertex);
            temp = temp->parent;
        }
        
        int i = 1;
        int numStops = stops.size();
        while(!stops.isEmpty()) {
            std::string s;
            Waypoint *curr = stops.pop();
            if (curr->parent != nullptr){
                if(pref->value() == 1) {
                    s = "    Number of Stops: " + to_string(i);
                }else if (pref->value() == 0){
                    s = "    Flight Cost: $" + to_string(curr->weight2);
                }else{
                    s = "    Flight Time: " + to_string(curr->weight) + " Hours";
                }
                results->add(new TextBox(40, y, 300, 25,s));
                y += 40;
                i++;
            }
            results->add(new TextBox(40, y, 300, 25, curr->vertex->data));
            y += 40;
             
            //cout << curr->vertex->data << " " << curr->partialCost << endl;
            //cout << curr->vertex->data << " " << curr->partialCost2 << endl;

            stats->label("Total Flight Time: " + to_string(curr->partialCost) + "    Total Cost: " + to_string(curr->partialCost2) + "    Total Stops: " + to_string(numStops - 1));

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
         timeCities.append(new Vertex(city));
         //costCities.append(new Vertex(city));
    }

    vertices.close();

    for (int i = 0; i < timeCities.size(); i++) {
        timeGraph.addVertex(timeCities[i]);
        //costGraph.addVertex(costCities[i]);
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
        
            if(city1 > timeCities.size() || city1 < 0){
                throw std::logic_error("City 1 does not exist");
            }
        }  
        if(std::getline(ss, item, ',')) {
            city2 = stoi(item);

            if(city2 > timeCities.size() || city2 < 0){
                throw std::logic_error("City 2 does not exist");
            }

            if(city2 == city1){
                throw std::logic_error("Cannot connect to the same city");
            }
        }
        if(std::getline(ss, item, ',')) {
            duration = stoi(item);

            if(duration < 0) {
                throw std::logic_error("Cannot have a negative flight time");
            }
        }
        if(std::getline(ss, item, ',')) {
            price = stoi(item);

            if(price < 0) {
                throw std::logic_error("Cannot have a negative price");
            }
        }

        if(isConnected(city1, city2)) {
            throw std::logic_error("Duplicate connection already exist");
        }

        timeGraph.addEdge(timeCities[city1], timeCities[city2], duration, price);
        //costGraph.addEdge(costCities[city1], costCities[city2], price);
        
    }    

    edges.close();

}

bool Application::isConnected(int c1, int c2){
    Vertex* v1 = timeCities[c1];
    Vertex* v2 = timeCities[c2];

    for(int i = 0; i < v1->edgeList.size(); i++) {
        if(v1->edgeList[i]->to == v2) {
            return true;
        }
    }
    return false;
}

void Application::initInterface() {
    window = new Window(100, 100, 800, 250, "Flight Planner");

    start = new Dropdown(20, 30, 360, 25, "Departure");
    end = new Dropdown(20, 80, 360, 25, "Destination");
    pref = new Dropdown(20, 130, 360, 25, "Preference");

    for (int i = 0; i < timeCities.size(); i++) {
        start->add(timeCities[i]->data);
        end->add(timeCities[i]->data);
    }

    pref->add("Cheapest Price");
    pref->add("Fewest Stops");
    pref->add("Shortest Travel Time");

    search = new Button(20, 170, 360, 25, "Search");
    ON_CLICK(search, Application::handleClick);

    stats = new TextBox(20, 210, 760, 25, "Total Flight Time:  Total Cost:  Number of Stops:  ");
    stats->align(FL_ALIGN_CENTER);

    results = new Fl_Scroll(420, 30, 360, 170, "Results");
    results->align(FL_ALIGN_TOP_LEFT);
    results->box(FL_THIN_UP_BOX);

    //stats = new TextBox(20, 200, 760, 25, "Total Flight Time:  Total Cost:  Number of Stops:  ");

    window->show();
}