#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <iomanip>

using namespace std;

// The movie struct contains a unique movie ID and the movie's weight (quantative importance)
struct Movie {
    int movie_id;
    double weight;
    Movie(int id, double wt) : movie_id(id), weight(wt) {}
};

// An edge represents the movie that connects the two actors
struct Edge {
    shared_ptr<Movie> movie; // shared pointers for unique movies; used for optimization
    int actor_id;
    Edge(int actorId, shared_ptr<Movie> mov) : actor_id(actorId), movie(move(mov)) {}
};

class Graph {
private:
    unordered_map<int, vector<Edge>> adjList;
    unordered_map<int, string> actorNames; // to convert from actorId to actor name.
    unordered_map<int, string> movieNames; // to convert from movieId to movie title.
    unordered_map<int, shared_ptr<Movie>> movies; // to convert from movieId to movie struct.

public:
    void addActor(int actor_id, const string &name) {
        actorNames[actor_id] = name;
    }

    void addEdge(int actor_id_1, int actor_id_2, shared_ptr<Movie> movie, string movie_title) {
        adjList[actor_id_1].emplace_back(actor_id_2, movie);
        adjList[actor_id_2].emplace_back(actor_id_1, movie);
        movieNames[movie->movie_id] = movie_title;
    }

    // This function parses a single line in the CSV file.
    void parseCSVLine(const string& line, string fields[8]) {
        istringstream s(line);
        string field;
        field.reserve(100);  // pre allocate memory for efficiency
        int fieldIndex = 0;  // index to track the current field

        // the following logic is necessary for certain entries that include movie/actor names that have embedded double quotes
        while (s.good() && fieldIndex < 8) {
            if (s.peek() == '"') {
                s.ignore(); // Skip initial quote
                field.clear(); // Clear the field to start fresh
                while (true) {
                    char ch = s.get();
                    if (s.eof() || (ch == '"' && s.peek() != '"')) {
                        break; // end of the quoted field
                    } else if (ch == '"' && s.peek() == '"') {
                        s.ignore(); // skip one of the double quotes
                    }
                    field += ch; // add the character to the field
                }

                if (s.peek() == ',') s.ignore(); // Skip following comma
            } else {
                getline(s, field, ','); // read non-quoted field
            }
            fields[fieldIndex++] = move(field); // move the field to the array
        }

        // Fill any remaining fields with empty strings
        while (fieldIndex < 8) {
            fields[fieldIndex++] = "";
        }
    }

    void buildGraphFromCSV(const string& filename) {
        cout << "Building the graph of over 6 million actor-movie connections. Please wait..." << endl;

        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }

        string line;
        getline(file, line); // Skip the header line
        int n = 0;
        
        // initialize the eight relevant data which will be extracted each iteration
        int actor_id_1 = 0;
        string actor_name_1 = "";
        int actor_id_2 = 0;
        string actor_name_2 = "";
        int movie_id = 0;
        string movie_title = "";
        double movie_popularity = 0;
        double movie_vote_average = 0;

        while (getline(file, line)) {
            // dynamically display the progress of the graph
            n++;
            if (n % 50000 == 0) {
                cout << "\r" << fixed << setprecision(2) << (static_cast<double>(n) / 6200000) * 100.0 << "\% done...";
                cout.flush(); // Ensure the output is displayed immediately
            }
            string fields[8];
            parseCSVLine(line, fields);

            actor_id_1 = stoi(fields[0]);
            actor_name_1 = fields[1];
            actor_id_2 = stoi(fields[2]);
            actor_name_2 = fields[3];
            movie_id = stoi(fields[4]);
            movie_title = fields[5];
            movie_popularity = stod(fields[6]);
            movie_vote_average = stod(fields[7]);

            double weight = 100 * (1 / movie_popularity) + 25 * (1 / movie_vote_average);
            addActor(actor_id_1, actor_name_1);
            addActor(actor_id_2, actor_name_2);
            auto& movie = movies[movie_id];
            if (!movie) movie = make_shared<Movie>(movie_id, weight);
            addEdge(actor_id_1, actor_id_2, movie, movie_title);
        }
        cout << "\r" << fixed << setprecision(2) << 100.00 << "\% done..." << endl;
    }

    const unordered_map<int, string>& getActorNames() const { return actorNames; }

    const unordered_map<int, string>& getMovieNames() const { return movieNames; }

    const unordered_map<int, vector<Edge>>& getAdjList() const { return adjList; }

    int findMovieId(int actor_id_1, int actor_id_2) const {
        for (auto e : adjList.at(actor_id_1)) {
            if (e.actor_id == actor_id_2) return e.movie->movie_id;
        }
        return -1;
    }
};
