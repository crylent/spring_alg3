#pragma once
#include <string>
#include <stdexcept>
#include <limits.h>

#define INF ULLONG_MAX/2

typedef size_t** Matrix;

class Graph
{
private:
	const int size;
	Matrix matrix;

public:
	Graph(int size): size(size) {
		matrix = new size_t*[size];
		for (int i = 0; i < size; i++) {
			matrix[i] = new size_t[size];
			for (int j = 0; j < size; j++) {
				matrix[i][j] = INF; // basically no way
			}
		}
	}

	void set_edge(int node1, int node2, size_t direct, size_t reverse = INF) {
		matrix[node1][node2] = direct;
		matrix[node2][node1] = reverse;
	}

	struct Node {
		Node* next = nullptr;
		int node = -1;
	};
	struct Path {
		Node* start = new Node;
		size_t length = 0;
	};

	Path* find_best(int start, int end) { // bellman-ford algorithm
		size_t* d = new size_t[size]; // distances
		int* p = new int[size]; // predecessors
		for (int i = 0; i < size; i++) {
			d[i] = INF;
		}
		d[start] = 0;
		for (int times = 0; times < size; times++) {
			for (int i = 0; i < size; i++) {
				for (int j = 0; j < size; j++) {
					if (matrix[i][j] == INF) continue;
					size_t new_dist = d[i] + matrix[i][j];
					if (new_dist < d[j]) {
						d[j] = new_dist;
						p[j] = i;
					}
				}
			}
		}

		Path* path = new Path;
		path->length = d[end];

		Node* begin = new Node;
		Node* curr = new Node;
		int i = end; // record path from end to begin
		while (i != start) {
			curr->node = i;
			begin = new Node;
			begin->next = curr;
			curr = begin;
			i = p[i];
		}
		begin->node = i;

		path->start = begin;
		return path;
	}

	~Graph() {
		for (int i = 0; i < size; i++) {
			delete[] matrix[i];
		}
		delete[] matrix;
	}
};

