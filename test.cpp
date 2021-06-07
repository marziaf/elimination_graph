#include <cstdlib>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <stdio.h>
#include "lexp.h"
#ifndef GRAPH_H
#include "graph.h"
#endif

void print_graph(std::vector<Node> &G);

// Perfect elimination graph
std::vector<Node> get_perfect_elimination_graph()
{
    std::vector<Node> graph;
    const int n = 10;
    graph.resize(n);
    for (int i = 0; i < n; ++i)
        graph[i] = {.id = (char)('1' + i)};

    graph[0].adj = {&graph[2],
                    &graph[4]};
    graph[1].adj = {&graph[5],
                    &graph[6]};
    graph[2].adj = {&graph[0],
                    &graph[4],
                    &graph[8]};
    graph[3].adj = {&graph[2],
                    &graph[4],
                    &graph[8]};
    graph[4].adj = {&graph[0],
                    &graph[2],
                    &graph[3],
                    &graph[7],
                    &graph[8]};
    graph[5].adj = {&graph[1],
                    &graph[6],
                    &graph[9]};
    graph[6].adj = {&graph[1],
                    &graph[5],
                    &graph[9]};
    graph[7].adj = {&graph[4],
                    &graph[8],
                    &graph[9]};
    graph[8].adj = {&graph[2],
                    &graph[3],
                    &graph[4],
                    &graph[7],
                    &graph[9]};
    graph[9].adj = {&graph[5],
                    &graph[6],
                    &graph[7],
                    &graph[8]};

    return graph;
}

// get simple non triangulated
std::vector<Node> get_nontriang_graph()
{
    std::vector<Node> graph;
    const int n = 6;
    graph.resize(n);
    for (int i = 0; i < n; ++i)
        graph[i] = {.id = (char)('1' + i)};

    graph[0].adj = {&graph[2],
                    &graph[3]};
    graph[1].adj = {&graph[2],
                    &graph[4]};
    graph[2].adj = {&graph[0],
                    &graph[1],
                    &graph[5]};
    graph[3].adj = {&graph[0],
                    &graph[5]};
    graph[4].adj = {&graph[1],
                    &graph[5]};
    return graph;
}

std::vector<Node> get_list_graph()
{
    std::vector<Node> graph;
    const int n = 3;
    graph.resize(n);
    for (int i = 0; i < n; ++i)
        graph[i] = {.id = (char)('a' + i)};
    graph[0].adj = {&graph[1]};
    graph[1].adj = {&graph[0],
                    &graph[2]};
    graph[2].adj = {&graph[1]};
    return graph;
}

// Returns a random graph of n nodes and e edges maximum
std::vector<Node>
get_random_graph(int n, int e)
{
    std::vector<Node> graph;
    // create the nodes
    for (int i = 0; i < n; ++i)
        graph.push_back((Node){.id = (char)('a' + i)});

    // create edges (max e)
    for (int i = 0; i < e; ++i)
    {
        int r1 = std::rand() % n;
        int r2 = std::rand() % n;
        while (r1 == r2)
            r2 = std::rand() % n;
        graph[r1].adj.insert(&graph[r2]);
        graph[r2].adj.insert(&graph[r1]);
    }
    return graph;
}

void print_graph(std::vector<Node> &G)
{
    for (Node node : G)
    {
        printf("node %c adj: {", node.id);
        for (Node *adj : node.adj)
        {
            printf(" %c ", adj->id);
        }
        printf("}\n");
    }
}

void print_order(order &ord)
{
    for (Node *node : ord.alpha)
        printf("%c\n", node->id);
}

int main()
{
    std::vector<Node> graph = get_list_graph();
    printf("The original graph is:\n");
    print_graph(graph);

    order ord = lexp_iter(graph);
    printf("\nThe order is:\n");
    print_order(ord);
}