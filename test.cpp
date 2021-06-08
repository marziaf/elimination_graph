#include <cstdlib>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <stdio.h>
#include <assert.h>
#include "lib/lex.h"
#include "lib/graph.h"
#include "lib/fill.h"

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
                    &graph[3],
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
    graph[5].adj = {&graph[2],
                    &graph[3],
                    &graph[4]};
    return graph;
}

std::vector<Node> get_list_graph()
{
    std::vector<Node> graph;
    const int n = 4;
    graph.resize(n);
    for (int i = 0; i < n; ++i)
        graph[i] = {.id = (char)('a' + i)};
    graph[0].adj = {&graph[1]};
    graph[1].adj = {&graph[0],
                    &graph[2]};
    graph[2].adj = {&graph[1],
                    &graph[3]};
    graph[3].adj = {&graph[2]};
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
    {
        printf("%c ", node->id);
    }
    printf("\n");
}

struct results
{
    std::string test_name;
    std::vector<Node> original_graph;
    order ord;
    std::vector<Node> fillin_graph;
    int added_edges;
};

int main()
{
    std::vector<results> tests;
    std::vector<Node> G1 = get_list_graph();
    order ord1 = lexp_iter(G1);
    auto fll1 = fill(G1, ord1);
    tests.push_back({"list", G1, ord1, fll1.first, fll1.second});
    std::vector<Node> G3 = get_perfect_elimination_graph();
    order ord3 = lexp_iter(G3);
    auto fll3 = fill(G3, ord3);
    tests.push_back({"triangulated", G3, ord3, fll3.first, fll3.second});
    std::vector<Node> G2 = get_nontriang_graph();
    order ord2 = lexp_iter(G2);
    auto fll2 = fill(G2, ord2);
    tests.push_back({"non-triangulated", G2, ord2, fll2.first, fll2.second});

    for (results test : tests)
    {
        printf("---------------------\n");
        std::cout << "TEST: " << test.test_name << std::endl;
        printf("\nThe original graph is:\n");
        print_graph(test.original_graph);
        printf("\nThe filled graph is:\n");
        print_graph(test.fillin_graph);
        printf("\n%d edges added\n", test.added_edges);
        printf("---------------------\n");
    }
}