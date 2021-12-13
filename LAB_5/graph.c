#include "graph.h"

t_graph* create_empty_graph() {
    t_graph* g = (t_graph*)malloc(sizeof(t_graph));
    if (!g) return NULL;
    g -> n = 0;
    g -> m = 0;
    g -> head = NULL;
    return g;
}

int add_new_node(t_graph* g, int x, int y, const char* name) {
    t_node* v = (t_node*)malloc(sizeof(t_node));
    if (!v) return -1;
    v -> x = x;
    v -> y = y;
    v -> name = strdup(name);
    v -> is_fired = 0;
    v -> dist = INF;
    v -> parent = NULL;
    if (!(v -> name)) return -1;
    v -> head = NULL;
    v -> next = g -> head;
    g -> head = v;
    ++(g -> n);
    return 0;
}

t_node* find_node_by_name(t_graph* g, const char* name) {
    if (!g) return NULL;
    t_node* tmp = g -> head;
    while (tmp) {
        if (strcmp(tmp -> name, name) == 0)
            return tmp;
        tmp = tmp -> next;
    }
    return NULL;
}

unsigned int get_distance(t_node* from, t_node* to) {
    int x1 = from -> x;
    int x2 = to -> x;
    int y1 = from -> y;
    int y2 = to -> y;
    unsigned int res = sqrt(1. * ((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
    return res;
}

int add_new_edge(t_graph* g, const char* from_name, const char* to_name) {
    if (!g) return -1;
    t_node* from = find_node_by_name(g, from_name);
    t_node* to = find_node_by_name(g, to_name);
    if (!from || !to) return -1;
    t_edge* edge = (t_edge*)malloc(sizeof(t_edge));
    if (!edge) return -1;
    edge -> to = to;
    edge -> weight = get_distance(from, to);
    edge -> next = from -> head;
    from -> head = edge;
    ++(g -> m);
    return 0;
}

void delete_edge(t_graph* g, t_node* from, t_node* to) {
    if (!from || !to) return ;
    t_edge* tmp = from -> head;
    if (!tmp) return ;
    if (tmp -> to == to) {
        from -> head = tmp -> next;
        free(tmp);
        --(g -> m);
        return ;
    }
    while (tmp -> next) {
        if (tmp -> next -> to == to) {
            t_edge* tmp2 = tmp -> next;
            tmp -> next = tmp -> next -> next;
            free(tmp2);
            --(g -> m);
            return ;
        }
        tmp = tmp -> next;
    }
}

void delete_edge_by_name(t_graph* g, const char* from_name, const char* to_name) {
    if (!g) return ;
    t_node* from = find_node_by_name(g, from_name);
    t_node* to = find_node_by_name(g, to_name);
    delete_edge(g, from, to);
}

void delete_node(t_graph* g, const char* name) {
    if (!g) return ;
    t_node* node = find_node_by_name(g, name);
    if (!node) return ;
    if (g -> head == node)
        g -> head = node -> next;
    t_node* tmp_node = g -> head;
    while (tmp_node) {
        delete_edge(g, tmp_node, node);
        if (tmp_node -> next == node)
            tmp_node -> next = node -> next;
        tmp_node = tmp_node -> next;

    }
    t_edge* tmp_edge = node -> head;
    while (tmp_edge) {
        node -> head = tmp_edge -> next;
        free(tmp_edge);
        tmp_edge = node -> head;
    }
    free(node -> name);
    free(node);
    --(g -> n);
}

void clear_nodes(t_graph* g) {
    if (!g) return;
    t_node* tmp = g -> head;
    while (tmp) {
        tmp -> is_fired = 0;
        tmp -> dist = INF;
        tmp -> parent = NULL;
        tmp = tmp -> next;
    }
}

int bfs(t_graph* g, const char* start_name, const char* end_name) {
    if (!g) return -3;
    t_node* start = find_node_by_name(g, start_name);
    t_node* end = find_node_by_name(g, end_name);
    if (!start || !end) return -2;
    t_node* queue[g -> n];
    for (unsigned int i = 0; i < g -> n; ++i)
        queue[i] = NULL;
    unsigned int first = 0;
    unsigned int last = 0;
    queue[first] = start;
    while (queue[first]) {
        queue[first] -> is_fired = 1;
        t_edge* tmp = queue[first] -> head;
        while (tmp) {
            if (tmp -> to == end) {
                clear_nodes(g);
                printf("path was found\n");
                return 0;
            }
            if (tmp -> to -> is_fired == 0) {
                ++last;
                queue[last] = tmp -> to;
            }
            tmp = tmp -> next;
        }
        ++first;
    }
    clear_nodes(g);
    printf("path doesn't exist\n");
    return -1;
}

int dijkstra(t_graph* g, const char* start_name, const char* end_name) {
    if (!g) return -3;
    t_node* start = find_node_by_name(g, start_name);
    t_node* end = find_node_by_name(g, end_name);
    if (!start || !end) return -2;
    unsigned int n = g -> n;
    size_t dist[n];
    t_node* nodes[n];
    t_node* tmp = g -> head;
    unsigned int i = 0;
    unsigned int start_iter, end_iter;
    while (tmp) {
        if (tmp == start)
            start_iter = i;
        else if (tmp == end)
            end_iter = i;
        nodes[i++] = tmp;
        tmp = tmp -> next;
    }
    start -> dist = 0;
	for (i = 0; i < n; ++i) {
		int v = -1;
		for (unsigned int j = 0; j < n; ++j)
			if (!(nodes[j] -> is_fired) && (v == -1 || (nodes[j] -> dist) < (nodes[v] -> dist)))
				v = j;
		if (nodes[v] -> dist == INF)
			break;
		nodes[v] -> is_fired = 1;
        t_edge* tmp_edge = nodes[v] -> head;
        while (tmp_edge) {
            unsigned int w = tmp_edge -> weight;
            t_node* to = tmp_edge -> to;
            if ((nodes[v] -> dist) + w < to -> dist) {
                to -> dist = (nodes[v] -> dist) + w;
                to -> parent = nodes[v];
            }
            tmp_edge = tmp_edge -> next;
        }
	}
    tmp = end;
    if (!(tmp -> parent)) {
        printf("path from %s to %s doesn't exist\n", start -> name, end -> name);
        return -1;
    }
    printf("shortest path was found:\n");
    char* path[n];
    i = 0;
    while(tmp != start && tmp -> parent) {
        path[i++] = tmp -> name;
        tmp = tmp -> parent;
    }
    printf("%s", start -> name);
    for (int j = i - 1; j >= 0; --j)
        printf(" -> %s", path[j]);
    printf("\n");
    clear_nodes(g);
}

int get_three_shortest_paths(t_graph* g) {
    if (!g) return -1;
    t_edge* shortest1 = NULL;
    t_node* from1 = NULL;
    unsigned int dist = INF;
    t_node* tmp = g -> head;
    while (tmp) {
        t_edge* tmp_edge = tmp -> head;
        while (tmp_edge) {
            if (tmp_edge -> weight < dist) {
                dist = tmp_edge -> weight;
                shortest1 = tmp_edge;
                from1 = tmp;
            }
            tmp_edge = tmp_edge -> next;
        }
        tmp = tmp -> next;
    }
    if (dist == INF) {
        printf("no more paths in graph\n");
        return 0;
    }
    printf("First path: %s -> %s, distance = %u\n", \
        from1 -> name, shortest1 -> to -> name, dist);
    t_edge* shortest2 = NULL;
    t_node* from2 = NULL;
    dist = INF;
    tmp = g -> head;
    while (tmp) {
        t_edge* tmp_edge = tmp -> head;
        while (tmp_edge) {
            if (tmp_edge -> weight < dist && tmp_edge != shortest1) {
                dist = tmp_edge -> weight;
                shortest2 = tmp_edge;
                from2 = tmp;
            }
            tmp_edge = tmp_edge -> next;
        }
        tmp = tmp -> next;
    }
    if (dist == INF) {
        printf("no more paths in graph\n");
        return 0;
    }
    printf("Second path: %s -> %s, distance = %u\n", \
        from2 -> name, shortest2 -> to -> name, dist);
    t_edge* shortest3 = NULL;
    t_node* from3 = NULL;
    dist = INF;
    tmp = g -> head;
    while (tmp) {
        t_edge* tmp_edge = tmp -> head;
        while (tmp_edge) {
            if (tmp_edge -> weight < dist && tmp_edge != shortest1 && \
                    tmp_edge != shortest2) {
                dist = tmp_edge -> weight;
                shortest3 = tmp_edge;
                from3 = tmp;
            }
            tmp_edge = tmp_edge -> next;
        }
        tmp = tmp -> next;
    }
    if (from1 == shortest2 -> to) {
        if (shortest2 -> weight + shortest1 -> weight < dist) {
            dist = shortest2 -> weight + shortest1 -> weight;
            printf("Third path: %s -> %s -> %s, distance = %u\n", \
                from2 -> name, from1 -> name, shortest1 -> to -> name, dist);
            return 0;
        }
    }
    else if (from2 == shortest1 -> to) {
        if (shortest2 -> weight + shortest1 -> weight < dist) {
            dist = shortest2 -> weight + shortest1 -> weight;
            printf("Third path: %s -> %s -> %s, distance = %u\n", \
                from1 -> name, from2 -> name, shortest2 -> to -> name, dist);
            return 0;
        }
    }
    if (dist == INF) {
        printf("no more paths in graph\n");
        return 0;
    }
    printf("Third path: %s -> %s, distance = %u\n", \
        from3 -> name, shortest3 -> to -> name, dist);
    return 0;
}

void print_graph(t_graph* g) {
    if (!g) return ;
    t_node* tmp = g -> head;
    printf("Vertex (x, y): Edge1 (weight1), Edge2 (weight2), ...\n");
    while (tmp) {
        printf("%s (%d, %d): ", tmp -> name, tmp -> x, tmp -> y);
        t_edge* tmp_edge = tmp -> head;
        while (tmp_edge) {
            printf("%s (%u), ", tmp_edge -> to -> name, tmp_edge -> weight);
            tmp_edge = tmp_edge -> next;
        }
        printf("\n");
        tmp = tmp -> next;
    }
}

void delete_graph(t_graph** g) {
    if (!g || !(*g)) return ;
    t_node* tmp = (*g) -> head;
    while (tmp) {
        t_edge* tmp_edge = tmp -> head;
        while (tmp_edge) {
            tmp -> head = tmp_edge -> next;
            free(tmp_edge);
            tmp_edge = tmp -> head;
        }
        (*g) -> head = tmp -> next;
        free(tmp -> name);
        free(tmp);
        tmp = (*g) -> head;
    }
    free(*g);
}

void gen_node_name(char* name, unsigned int k) {
    sprintf(name, "%u", k);
}

t_graph* generate_random_graph(unsigned int n, unsigned int m) {
    t_graph* g = create_empty_graph();
    char buf[10];
    for (unsigned int i = 0; i < n; ++i) {
        gen_node_name(buf, i);
        add_new_node(g, (rand() % n) - n / 2, (rand() % n) - n / 2, buf);
    }
    for (unsigned int i = 0; i< m; ++i) {
        int from = rand() % n;
        int to = rand() % n;
        while (from == to) {
            from = rand() % n;
            to = rand() % n;
        }
        char name1[10];
        char name2[10];
        sprintf(name1, "%d", from);
        sprintf(name2, "%d", to);
        add_new_edge(g, name1, name2);
    }
    return g;
}

void dump_graph_to_file(t_graph* g, const char* filename) {
    FILE* file;
    if (!g) return ;
    file = fopen(filename, "w");
    if (!file) {
        printf("file \"%s\" is inaccessible and could not be created\n", filename);
        return ;
    }
    t_node* tmp = g -> head;
    fprintf(file, "%u %u\n", g -> n, g -> m);
    while (tmp) {
        fprintf(file, "%d %d %s\n", tmp -> x, tmp -> y, tmp -> name);
        tmp = tmp -> next;
    }
    tmp = g -> head;
    while (tmp) {
        t_edge* tmp_edge = tmp -> head;
        while (tmp_edge) {
            fprintf(file, "%s %s\n", tmp -> name, tmp_edge -> to -> name);
            tmp_edge = tmp_edge -> next;
        }
        tmp = tmp -> next;
    }
    fclose(file);
}

int load_graph_from_file(t_graph** graph, const char* filename) {
    if (*graph) delete_graph(graph);
    *graph = create_empty_graph();
    t_graph* g = *graph;
    FILE* file;
    file = fopen(filename, "r");
    if (!file) {
        printf("file \"%s\" doesn't exist or is inaccessible\n", filename);
        return -1;
    }
    unsigned int n, m;
    if (fscanf(file, "%u %u", &n, &m) == 2) {
        for (unsigned int i = 0; i < n; ++i) {
            int x, y;
            char buf[100];
            if (fscanf(file, "%d %d %s", &x, &y, buf) != 3 || \
                    add_new_node(g, x, y, buf) < 0) {
                printf("incorrect file\n");
                fclose(file);
                return -1;
            }
        }
        for (unsigned int i = 0; i < m; ++i) {
            char buf1[100];
            char buf2[100];
            if (fscanf(file, "%s %s", buf1, buf2) != 2 || \
                    add_new_edge(g, buf1, buf2)) {
                printf("incorrect file\n");
                fclose(file);
                return -1;
            }
        }
        fclose(file);
        return 0;
    }
    printf("incorrect file\n");
    fclose(file);
    return -1;
}
