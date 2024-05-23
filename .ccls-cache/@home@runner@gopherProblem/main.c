#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX 202 // Máximo número de nós no grafo (100 gophers + 100 buracos + source + sink)

int n, m, s, v;
int capacity[MAX][MAX], residualCapacity[MAX][MAX];
int parent[MAX];
bool visited[MAX];

bool bfs(int source, int sink) {
    memset(visited, 0, sizeof(visited));
    int queue[MAX], front = 0, rear = 0;
    queue[rear++] = source;
    visited[source] = true;
    parent[source] = -1;

    while (front < rear) {
        int current = queue[front++];

        for (int next = 0; next <= sink; ++next) {
            if (!visited[next] && residualCapacity[current][next] > 0) {
                queue[rear++] = next;
                visited[next] = true;
                parent[next] = current;
                if (next == sink) return true;
            }
        }
    }
    return false;
}

int fordFulkerson(int source, int sink) {
    int maxFlow = 0;

    memcpy(residualCapacity, capacity, sizeof(capacity));

    while (bfs(source, sink)) {
        int pathFlow = __INT_MAX__;

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = (pathFlow < residualCapacity[u][v]) ? pathFlow : residualCapacity[u][v];
        }

        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualCapacity[u][v] -= pathFlow;
            residualCapacity[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }

    return maxFlow;
}

int main() {
    while (scanf("%d %d %d %d", &n, &m, &s, &v) == 4) {
        double gophers[n][2], holes[m][2];

        for (int i = 0; i < n; ++i)
            scanf("%lf %lf", &gophers[i][0], &gophers[i][1]);

        for (int i = 0; i < m; ++i)
            scanf("%lf %lf", &holes[i][0], &holes[i][1]);

        memset(capacity, 0, sizeof(capacity));
        int source = 0, sink = n + m + 1;

        for (int i = 0; i < n; ++i)
            capacity[source][i + 1] = 1;

        for (int i = 0; i < m; ++i)
            capacity[n + 1 + i][sink] = 1;

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                double distance = (gophers[i][0] - holes[j][0]) * (gophers[i][0] - holes[j][0])
                                + (gophers[i][1] - holes[j][1]) * (gophers[i][1] - holes[j][1]);
                if (distance <= (s * v) * (s * v)) {
                    capacity[i + 1][n + 1 + j] = 1;
                }
            }
        }

        int maxFlow = fordFulkerson(source, sink);
        printf("%d\n", n - maxFlow);
    }

    return 0;
}
