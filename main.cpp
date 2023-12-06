#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

// README
// ============================================================================================================
// Ze względu na przygotowanie do pracy komercyjnej wszystkie zmienne zostały napisane w języku angielskim.
// Jedynie komentarze oraz tekst wyświetlany w konsoli jest w języku polskim aby ułatwić pracę oceniającemu.
// Indeks studenta: 264164
// W celu monitorowania zmian projekt jest dostępny na platformie Github.
// https://github.com/xmilov3/PEA_Project_1
// ============================================================================================================

// Zmienne
std::string filename; // Nazwa pliku tekstowego

// Struktura krawędzi grafu
struct Edge {
    int beginning;
    int end;
    int weight;
};

// Struktura reprezentująca graf
struct Graph {
    int V; // Liczba wierzchołków
    int E; // Liczba krawędzi
    Edge* edges;

    // Konstruktor grafu
    Graph(int vertices = 0, int edgesCount = 0) : V(vertices), E(edgesCount) {
        edges = new Edge[edgesCount];
    }

    // Destruktor grafu
    ~Graph() {
        delete[] edges;
    }
};

// Funkcja wczytująca graf z pliku tekstowego
void load_graph_from_file(const std::string& filename, Graph& graph) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout<<"Nie mozna otworzyc pliku.\n";
        exit(EXIT_FAILURE);
    }

    file>>graph.E>>graph.V;

    delete[] graph.edges; // Zwolnienie pamięci grafu
    graph.edges = new Edge[graph.E];

    for (int i=0;i<graph.E;++i) {
        file>>graph.edges[i].beginning>>graph.edges[i].end>>graph.edges[i].weight;
    }

    file.close();
}

// Funkcja generujaca graf
void generate_random_graph(Graph& graph, int density) {
    srand(static_cast<unsigned int>(time(nullptr)));

    graph.V = rand()%10+5; // Random number of vertices from 5 to 14
    graph.E = (graph.V*(graph.V-1)*density)/200;

    delete[] graph.edges; // Deleting previous data
    graph.edges = new Edge[graph.E];

    for (int i=0;i<graph.E;++i) {
        graph.edges[i].beginning = rand()%graph.V;
        graph.edges[i].end = rand()%graph.V;
        graph.edges[i].weight = rand()%10+1; // Losowa waga krawędzi z przedziału 1-10
    }
}

// Funkcja wyświetlająca graf
void display_graph(const Graph& graph) {
    std::cout << "Liczba wierzcholkow: " << graph.V << "\n";
    std::cout << "ELiczba krawedzi: " << graph.E << "\n";

    std::cout << "Opis krawedzi:\n";
    for (int i = 0; i < graph.E; ++i) {
        std::cout << "Poczatek: " << graph.edges[i].beginning << ", End: "<<graph.edges[i].end
                  << ", Waga/przepustowosc: " << graph.edges[i].weight << "\n";
    }
}

int main(){
    Graph myGraph;
    int choice;
    while (true){
        std::cout<<"==================================\n";
        std::cout<<"               MENU               \n";
        std::cout<<"==================================\n";
        std::cout<<"1. Wczytaj graf z pliku tekstowego\n";
        std::cout<<"2. Generowanie losowego grafu\n";
        std::cout<<"3. Wyświetlanie grafu\n";
        std::cout<<"4. Zapisz graf do pliku\n";
        std::cout<<"5. Uruchom algorytm Dijkstry\n";
        std::cout<<"==================================\n";
        std::cin>>choice;

        switch(choice){
            case 1:
            std::cout<<"Wybrano wczytanie grafu z pliksu tekstowego\n";
            std::cout<<"Wpisz nazwe pliku (z rozszerzeniem np. '.txt'\n";
            std::cin>>filename;
            load_graph_from_file(filename, myGraph);
            std::cout<<"Graf zostal wczytany pomyslnie\n";
            break;

            case 2:
                int density;
            std::cout<<"Wybrano generowanie losowego grafu\n";
            std::cout<<"Wpisz gestosc grafu\n";
            std::cin>>density;
            generate_random_graph(myGraph, density);
            std::cout<<"Graf zostal wygenerowany pomyslnie\n";
            break;

            case 3:
                std::cout<<"Wybrano wyswietlenie grafu\n";
                display_graph(myGraph);
                break;

            case 4:
            std::cout<<"Wybrano zapisanie grafu do pliku\n";
            break;

            case 5:
            std::cout<<"Wybrano uruchomienie algorytmu Dijsktry\n";
            break;
        }
    }
    return 0;
}
