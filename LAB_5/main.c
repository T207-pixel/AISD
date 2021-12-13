#include "graph.h"

int main(int argc, char** argv) {
    t_graph* g = NULL;
    unsigned int n, m, weight;
    int x, y;
    char buf[100];
    char buf2[100];
    int option = 0;
    while (1) {
        printf("Choose option:\n1. Create random graph\n2. Add node\n");
        printf("3. Add edge\n4. Delete node\n5. Delete edge\n6. BFS\n");
        printf("7. Dijkstra\n8. 3 shortest paths\n");
        printf("9. Print graph\n10. Load graph from file\n11. Dump graph to file\n");
        printf("12. Exit\n");
        if (scanf("%d", &option) <= 0) {
            printf("incorrect option\n");
            break ;
        }
        if (option == 12)
            break ;
        if (option == 1) {
            printf("please specify n and m\n");
            if (scanf("%u %u", &n, &m) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            g = generate_random_graph(n, m);
        }
        else if (option == 2) {
            printf("please specify x, y, name\n");
            if (scanf("%d %d %s", &x, &y, buf) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            add_new_node(g, x, y, buf);
        }
        else if (option == 3) {
            printf("Please specify from and to\n");
            if (scanf("%s %s", buf, buf2) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            add_new_edge(g, buf, buf2);
        }
        else if (option == 4) {
            printf("please specify the name\n");
            if (scanf("%s", buf) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            delete_node(g, buf);
        }
        else if (option == 5) {
            printf("please specify from and to\n");
            if (scanf("%s %s", buf, buf2) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            delete_edge_by_name(g, buf, buf2);
        }
        else if (option == 6) {
            printf("Please specify from and to\n");
            if (scanf("%s %s", buf, buf2) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            bfs(g, buf, buf2);
        }
        else if (option == 7) {
            printf("Please specify from and to\n");
            if (scanf("%s %s", buf, buf2) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            if (dijkstra(g, buf, buf2) < -1)
                printf("one or two vertices were not found in graph\n");
        }
        else if (option == 8) {
            get_three_shortest_paths(g);
        }
        else if (option == 9) {
            print_graph(g);
        }
        else if (option == 10) {
            printf("Please specify the filename\n");
            if (scanf("%s", buf) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            if (load_graph_from_file(&g, buf) < 0)
                printf("couldn't load graph from file\n");
        }
        else if (option == 11) {
            printf("Please specify the filename\n");
            if (scanf("%s", buf) <= 0) {
                printf("incorrect input\n");
                break ;
            }
            dump_graph_to_file(g, buf);
        }
        else {
            printf("incorrect option!\n");
            break ;
        }
    }
    delete_graph(&g);
    return 0;
}
