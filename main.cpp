#include <iostream>
#include "graph.cpp"
#include "dijkstra.cpp"
#include "bfs.cpp"

int main() {
    Graph movieGraph;
    movieGraph.buildGraphFromCSV("actor-movie-data.csv");
    unordered_map<int, string> actorNames = movieGraph.getActorNames();
    unordered_map<int, string> movieNames = movieGraph.getMovieNames();

    string startActorName = "", targetActorName = "";
    while (startActorName != "q") {
        cout << "Enter the name of the starting actor (input 'q' to exit): " << endl;
        getline(cin, startActorName);
        if (startActorName == "q") continue;
        cout << "Enter the name of the target actor: " << endl;
        getline(cin, targetActorName);
        cout << endl;
        // Assuming that getActorNames returns a map where key is the actor ID and value is the actor name
        auto& actorNames = movieGraph.getActorNames();
        int startActorId = -1, targetActorId = -1;

        // Find the actor IDs corresponding to the entered names
        for (const auto& pair : actorNames) {
            if (pair.second == startActorName) {
                startActorId = pair.first;
            }
            if (pair.second == targetActorName) {
                targetActorId = pair.first;
            }
        }

        if (startActorId == -1 || targetActorId == -1) {
            cout << "Actor name(s) not found in the dataset." << endl;
            continue;
        }
        
        int option = 1;
        cout << "Which algorithm would you like to use?\nInput 1 or 2.\n1) Dijkstra's (paths are dependent upon movie popularity)\n2) Breadth-first search (no weights)\n";
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        vector<int> path;
        if (option == 1) {
            path = dijkstra(movieGraph, startActorId, targetActorId);
        } else {
            path = bfs(movieGraph, startActorId, targetActorId);
        }

        if (path.empty()) {
            cout << "\nNo path exists between " << startActorName << " and " << targetActorName << endl;
            return 1;
        }

        // Output the shortest path
        cout << "\nThe shortest path from " << startActorName << " to " << targetActorName << " is:" << endl << endl;
        int n = 0;
        for (auto it = path.begin(); it != path.end() - 1; it++) {
            if (n % 2 == 0) {
                cout << actorNames.at(*it) << " <--{";
            }
            else {
                cout << movieNames.at(*it) << "}--> ";
            }
            n++;
        }
        cout << actorNames.at(*(path.end() - 1)) << endl << endl;
    }
    return 0;
}
