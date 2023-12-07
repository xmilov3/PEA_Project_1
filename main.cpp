#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <set>
#include <chrono>

// README
// ============================================================================================================
// Ze względu na przygotowanie do pracy komercyjnej wszystkie zmienne zostały napisane w języku angielskim.
// Jedynie komentarze oraz UI/UX jest w języku polskim aby ułatwić pracę oceniającemu.
// Indeks studenta: 264164
// W celu monitorowania zmian projekt jest dostępny na platformie Github.
// Program został napisany w Środowisku CLion, na MacBook Pro M2.
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
    auto begin = std::chrono::high_resolution_clock::now();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Nie mozna otworzyc pliku.\n";
        exit(EXIT_FAILURE);
    }

    file >> graph.E >> graph.V;

    delete[] graph.edges; // Zwolnienie pamieci grafu
    graph.edges = new Edge[graph.E];

    for (int i=0; i < graph.E; ++i) {
        file >> graph.edges[i].beginning >> graph.edges[i].end >> graph.edges[i].weight;
    }

    file.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
}

// Funkcja generująca graf
void generate_random_graph(Graph& graph, int density, int vertice) {
    auto begin = std::chrono::high_resolution_clock::now();
    srand(static_cast<unsigned int>(time(nullptr)));

    graph.V = vertice;
    graph.E = (graph.V * (graph.V-1) * density) / 200;

    delete[] graph.edges; // Zwolnienie pamięci grafu
    graph.edges = new Edge[graph.E];

    for (int i = 0; i < graph.E; ++i) {
        graph.edges[i].beginning = rand() % graph.V;
        graph.edges[i].end = rand() % graph.V;
        graph.edges[i].weight = rand() % 10 + 1; // Losowa waga krawędzi z przedziału 1-10
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
}




// Funkcja wyświetlająca graf
void display_graph(const Graph& graph) {
    auto begin = std::chrono::high_resolution_clock::now();
    std::cout << "Liczba wierzcholkow: " << graph.V << "\n";
    std::cout << "Gestosc: " << static_cast<double>(graph.E) / (graph.V * (graph.V - 1) / 2) * 100 << "%" << "\n";
    std::cout << "Liczba krawedzi: " << graph.E << "\n";
    std::cout << "Opis krawedzi:\n";
    for (int i = 0; i < graph.E; ++i) {
        std::cout << "Poczatek: " << graph.edges[i].beginning << ", Koniec: " << graph.edges[i].end
                  << ", Waga/przepustowosc: " << graph.edges[i].weight << "\n";
    }

    // Funkcja wyświetlająca graf macierzowo
    std::cout << "\nGraf macierzowy:\n";
    for (int i = 0; i < graph.V; ++i) {
        for (int j = 0; j < graph.V; ++j) {
            int weight = 0;
            for (int k = 0; k < graph.E; ++k) {
                if (graph.edges[k].beginning == i && graph.edges[k].end == j) {
                    weight = graph.edges[k].weight;
                    break;
                }
            }
            std::cout << weight << " ";
        }
        std::cout << "\n";
    }

    // Funkcja wyświetlająca graf listowo
    std::cout << "\nGraf listowy:\n";
    for (int i = 0; i < graph.V; ++i) {
        std::cout << "Wierzchołek " << i << ": ";
        for (int j = 0; j < graph.E; ++j) {
            if (graph.edges[j].beginning == i) {
                std::cout << "(" << graph.edges[j].end << ", " << graph.edges[j].weight << ") ";
            }
        }
        std::cout << "\n";
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
}



// Funkcja zapisująca graf do pliku tekstowego
void save_graph(const Graph& graph, const std::string& filename) {
    auto begin = std::chrono::high_resolution_clock::now();
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Nie mozna otworzyc pliku do zapisu.\n";
        return;
    }

    // Zapisz liczbę krawędzi i wierzchołków w pierwszym wierszu
    file << graph.E << " " << graph.V << "\n";

    // Zapisz opis każdej krawędzi w kolejnych wierszach
    for (int i = 0; i < graph.E; ++i) {
        file << graph.edges[i].beginning << " " << graph.edges[i].end << " " << graph.edges[i].weight << "\n";
    }
    std::cout << "Graf zostal zapisany w pliku tekstowym pod nazwa " << filename <<".\n";
    file.close();
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
}

// Funkcja wykonująca algorytm Dijkstry dla reprezentacji macierzowej
void dijkstry_matrix(const Graph& graph, int start_vertex, int end_vertex) {
    auto begin = std::chrono::high_resolution_clock::now();
    int V = graph.V;
    int* distance = new int[V]; // Tablica przechowująca najkrótsze odległości do wierzchołków
    bool* visited = new bool[V]; // Tablica przechowująca informację o odwiedzeniu wierzchołka

    // Inicjalizacja tablic
    for (int i = 0; i < V; ++i) {
        distance[i] = INT_MAX;
        visited[i] = false;
    }

    distance[start_vertex] = 0; // Odległość do samego siebie wynosi 0

    // Algorytm Dijkstry
    for (int count = 0; count < V - 1; ++count) {
        int min_distance = INT_MAX;
        int min_vertex = -1; // Dodanie -1 jako wartości początkowej

        // Znajdź wierzchołek o najmniejszej aktualnej odległości
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && distance[v] <= min_distance) {
                min_distance = distance[v];
                min_vertex = v;
            }
        }

        if (min_vertex == -1) // Dodanie warunku sprawdzającego, czy znaleziono wierzchołek
            break;

        visited[min_vertex] = true;

        // Zaktualizuj odległości sąsiadów wybranego wierzchołka
        for (int v = 0; v < V; ++v) {
            if (!visited[v] && graph.edges[min_vertex * V + v].weight != 0 && distance[min_vertex] != INT_MAX &&
                distance[min_vertex] + graph.edges[min_vertex * V + v].weight < distance[v]) {
                distance[v] = distance[min_vertex] + graph.edges[min_vertex * V + v].weight;
            }
        }
    }

    // Wyświetl wyniki
    std::cout << "Najkrotsza odleglosc z wierzcholka " << start_vertex << " do " << end_vertex << " wynosi: "
              << distance[end_vertex] << "\n";

    // Zwolnij pamięć
    delete[] distance;
    delete[] visited;
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
}

// Funkcja wykonująca algorytm Dijkstry dla reprezentacji listowej
void dijkstry_list(const Graph& graph, int start_vertex, int end_vertex) {
    auto begin = std::chrono::high_resolution_clock::now();
    int V = graph.V;
    std::set<std::pair<int, int> > vertices_to_visit; // Zbiór par (odległość, wierzchołek)
    std::vector<int> distance(V, INT_MAX); // Wektor przechowujący najkrótsze odległości do wierzchołków

    // Inicjalizacja odległości
    distance[start_vertex] = 0;

    // Wstaw wszystkie wierzchołki do zbioru
    for (int i = 0; i < V; ++i) {
        vertices_to_visit.insert(std::make_pair(distance[i], i));
    }

    // Algorytm Dijkstry
    while (!vertices_to_visit.empty()) {
        int current_vertex = vertices_to_visit.begin()->second;
        vertices_to_visit.erase(vertices_to_visit.begin());

        // Zaktualizuj odległości sąsiadów wybranego wierzchołka
        for (int i = 0; i < graph.E; ++i) {
            int neighbor = graph.edges[i].end;
            int weight = graph.edges[i].weight;

            if (graph.edges[i].beginning == current_vertex && distance[current_vertex] != INT_MAX &&
                distance[current_vertex] + weight < distance[neighbor]) {
                vertices_to_visit.erase(std::make_pair(distance[neighbor], neighbor));
                distance[neighbor] = distance[current_vertex] + weight;
                vertices_to_visit.insert(std::make_pair(distance[neighbor], neighbor));
            }
        }
    }

    // Wyświetl wyniki
    std::cout << "Najkrotsza odleglosc z wierzcholka " << start_vertex << " do " << end_vertex << " wynosi: "
              << distance[end_vertex] << "\n";
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    std::cout << "Operacja trwala: " << elapsed.count() << " mikrosekund.\n";
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
        std::cout<<"3. Wyświetlanie grafu (macierzowo oraz listowo)\n";
        std::cout<<"4. Zapisz graf do pliku\n";
        std::cout<<"5. Uruchom algorytm Dijkstry dla wczytanego/wygenerowanego grafu\n";
        std::cout<<"6. Zakoncz program\n";
        std::cout<<"==================================\n";
        std::cin>>choice;

        switch(choice){
            case 1:

            std::cout << "Wybrano wczytanie grafu z pliksu tekstowego\n";
            std::cout << "Wpisz nazwe pliku (z rozszerzeniem '.txt')\n";
            std::cin >> filename;
            load_graph_from_file(filename, myGraph);
            std::cout << "Graf zostal wczytany pomyslnie\n";
            break;

            case 2:
                int density;
                int vertice;
            std::cout << "Wybrano generowanie losowego grafu\n";
            std::cout << "Wpisz gestosc grafu (wystarczy sama liczba, nie trzeba pisac znaku '%')\n";
            std::cin >> density;
            std::cout << "Wpisz liczbe wierzcholkow grafu\n";
                std::cin >> vertice;
            generate_random_graph(myGraph, density, vertice);
            std::cout << "Graf zostal wygenerowany pomyslnie\n";
            break;

            case 3:
                std::cout << "Wybrano wyswietlenie grafu\n";
                display_graph(myGraph);
                break;

            case 4:
            std::cout << "Wybrano zapisanie grafu do pliku\n";
            std::cout << "Napisz nazwe pliku (z rozszerzeniem '.txt')\n";
            std::cin >> filename;
            save_graph(myGraph, filename);
            break;

            case 5:
                int first_vertice;
                int last_vertice;
            std::cout << "Wybrano uruchomienie algorytmu Dijsktry (listowo/macierzowo)\n";
            std::cout << "Podaj wierzcholek poczatkowy\n";
            std::cin >> first_vertice;
            std::cout << "Podaj wierzcholek koncowy\n";
            std::cin >> last_vertice;
            dijkstry_matrix(myGraph, first_vertice, last_vertice);
            dijkstry_list(myGraph, first_vertice, last_vertice);
            break;

            case 6:
                return 0;
        }
    }

}
