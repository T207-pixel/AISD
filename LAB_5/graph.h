#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#define INF UINT_MAX

struct s_edge;

typedef struct s_node {
    int x;
    int y;
    char* name;
    int is_fired; // for BFS, Dijkstra
    unsigned int dist; // for Dijkstra
    struct s_node* parent; // for Dijkstra shortest path reconstruction
    struct s_edge* head;
    struct s_node* next;
}               t_node;

typedef struct s_edge {
    t_node* to;
    unsigned int weight;
    struct s_edge* next;
}               t_edge;

typedef struct s_graph {
    unsigned int n;
    unsigned int m;
    t_node* head;
}               t_graph;

t_graph* generate_random_graph(unsigned int n, unsigned int m);
void dump_graph_to_file(t_graph* g, const char* filename);
int load_graph_from_file(t_graph** g, const char* filename);
void delete_graph(t_graph** g);
void print_graph(t_graph* g);
int get_three_shortest_paths(t_graph* g);
int dijkstra(t_graph* g, const char* start_name, const char* end_name);
int bfs(t_graph* g, const char* start_name, const char* end_name);
void delete_node(t_graph* g, const char* name);
void delete_edge_by_name(t_graph* g, const char* from_name, const char* to_name);
int add_new_edge(t_graph* g, const char* from_name, const char* to_name);
int add_new_node(t_graph* g, int x, int y, const char* name);
