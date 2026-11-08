#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <algorithm>
#include <climits>
#include <cctype>
#include <utility>
using namespace std;

const int INF = INT_MAX;
const int N = 20;                       // nodes A..T

// Priority queue supporting insert / getMin / decreaseKey
struct PQ {
    set<pair<int,int>> s;               // (key, node)
    void insert(int v, int key)       { s.insert({key, v}); }
    bool empty() const                { return s.empty(); }
    int  getMin() {                     // remove and return node with smallest key
        int v = s.begin()->second;
        s.erase(s.begin());
        return v;
    }
    void decreaseKey(int v, int oldKey, int newKey) {
        s.erase({oldKey, v});
        s.insert({newKey, v});
    }
};

vector<pair<int,int>> adj[N];           // adjacency list: (neighbor, weight)

void addEdge(char u, char v, int w) {   // undirected edge
    adj[u - 'A'].push_back({v - 'A', w});
    adj[v - 'A'].push_back({u - 'A', w});
}

// Ask the user for a node letter between A and T until a valid one is typed
int readNode(const string& prompt) {
    char c;
    while (true) {
        cout << prompt;
        if (!(cin >> c)) exit(0);       // input closed
        c = toupper(static_cast<unsigned char>(c));
        if (c >= 'A' && c < 'A' + N) return c - 'A';
        cout << "Invalid node. Enter a letter from A to T.\n";
    }
}

int main() {
    // (edge weight = DIST[node] - DIST[pred])
    addEdge('A','B',100);
    addEdge('A','C',300);
    addEdge('C','D',150);
    addEdge('A','E',450);
    addEdge('E','G',200);
    addEdge('G','F',100);
    addEdge('D','F',350);             
    addEdge('G','H',80);
    addEdge('G','I',200);
    addEdge('G','J',120);
    addEdge('J','K',100);
    addEdge('J','L',90);
    addEdge('L','M',100);
    addEdge('K','N',350);
    addEdge('M','P',150);
    addEdge('P','O',200);
    addEdge('O','Q',110);
    addEdge('P','R',200);
    addEdge('Q','S',80);
    addEdge('R','T',160);

    //  User input 
    int src = readNode("Enter source node (A-T): ");
    int dst = readNode("Enter destination node (A-T): ");

    // 2-4: initialise
    vector<int> dist(N, INF), pred(N, -1);
    // 5
    dist[src] = 0;

    // 6-7: insert every vertex into PQ
    PQ pq;
    for (int v = 0; v < N; v++) pq.insert(v, dist[v]);

    // 8: while PQ is not empty
    while (!pq.empty()) {
        int u = pq.getMin();                         // 9
        if (dist[u] == INF) break;                   // remaining nodes unreachable
        for (auto [v, w] : adj[u]) {                 // 10-11
            int newLen = dist[u] + w;                // 12
            if (newLen < dist[v]) {                  // 13
                pq.decreaseKey(v, dist[v], newLen);  // 14
                dist[v] = newLen;                    // 15
                pred[v] = u;                         // 16
            }
        }
    }

    // DIST / PRED table
    cout << "\nNode\tDIST\tPRED\n";
    for (int i = 0; i < N; i++) {
        cout << char('A' + i) << "\t";
        if (dist[i] == INF) cout << "INF\t-1\n";
        else cout << dist[i] << "\t"
                  << (pred[i] == -1 ? string("-1") : string(1, 'A' + pred[i])) << "\n";
    }

    // Shortest path src -> dst
    if (dist[dst] == INF) {
        cout << "\nNo path from " << char('A' + src) << " to " << char('A' + dst) << ".\n";
        return 0;
    }
    vector<int> path;
    for (int v = dst; v != -1; v = pred[v]) path.push_back(v);
    reverse(path.begin(), path.end());

    cout << "\nShortest path: ";
    for (size_t i = 0; i < path.size(); i++)
        cout << char('A' + path[i]) << (i + 1 < path.size() ? " - " : "");
    cout << "\nTotal distance: " << dist[dst] << "\n";
    return 0;
}
