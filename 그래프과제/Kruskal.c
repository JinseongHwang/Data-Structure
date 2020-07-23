
// MST - Kruskal �˰������� ����

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // �ִ� ����ġ�� 100���� ����
#define MAX_VERTICES 205 // ������ �ִ� ������ 205���� ����
#define MAX_EDGES (MAX_VERTICES * (MAX_VERTICES - 1) / 2) // �ִ� ���� ����
#define NUM_EDGE(X) (((X) * ((X) - 1)) / 2) // ������ X���� �� �ִ� ���� ����
#define INF 987654321 // ������ ����ġ�� ǥ����

// ������ �����߱� ������ false �Ǵ� 0���� �ʱ�ȭ��
int parent[MAX_VERTICES]; // �θ� ������ index�� ���� �� ����
double TimeRepository[21]; // ����ð� ���� ���� ����

// ===== Union Find �˰��� =====
// �θ� ������ �����ϴ� parent �迭�� �ʱ�ȭ�Ѵ�.
void initParent(int nVtx) {
	// ������ ���� ��ŭ�� �迭�� ����Ѵ�.
	// ������ ���� ����̰ų� �ܵ� ����� ��� -1 �θ��� ������.
	for (int i = 0; i < nVtx; ++i) {
		parent[i] = -1;
	}
}

// �Ű������� ���� curr�� ���� ��带 ã�Ƽ� ��ȯ�Ѵ�.
int findParent(int curr) {
	// ������ -1�� ��� ������ ���� ����̰ų� �ܵ� ����̴�.
	if (parent[curr] == -1) {
		return curr;
	}
	// �θ� �� -1�� ���� ��带 ã�Ƽ� ��ȯ�Ѵ�.
	while (parent[curr] != -1) {
		curr = parent[curr];
	}
	return curr;
}

// �θ� ����� ��ȣ�� ��ģ��.
// ���� �������� �ʴ� �� �׷����� a ���� b ��带 �������� �����Ѵ�.
void unionParent(int a, int b) {
	// �� ����� ���� ��ȣ�� findParent() �Լ��� ����ؼ� ��ȯ�޴´�.
	a = findParent(a);
	b = findParent(b);
	// ���� �� ����� ������ �ٸ��ٸ�,
	// (���� �ٸ� �׷����� �����ִٸ�)
	if (a != b) {
		// a����� �θ� b�� �����Ѵ�.
		// �ݴ�� �����ص� �����ϴ�.
		parent[a] = b;
	}
}
// =========================
// ���� �ϳ��� �����ϴ� ������
typedef struct Edge {
	// (���� ����, �� ����, ����ġ)
	int start, end, weight;
} Edge;

// �׷����� ������.
typedef struct Graph {
	int nEdge; // ������ ����
	Edge edges[MAX_EDGES]; // ������ ������ŭ �迭�� ����
} Graph;

// �׷����� ������ �ʱ�ȭ�Ѵ�. ������ ������ �Ű������� �޴´�.
void initGraph(Graph* g, int nVtx) {
	g->nEdge = 0; // ���� �׷����� ������ ������ 0���̴�.
	for (int i = 0; i < NUM_EDGE(nVtx); ++i) { // ���� ������ ���� �ִ� ������ ������ŭ�� �ݺ��Ѵ�.
		g->edges[i].start = 0; // ���� ������ ���� 0���� �ʱ�ȭ
		g->edges[i].end = 0; // �� ������ ���� 0���� �ʱ�ȭ
		g->edges[i].weight = INF; // ����ġ�� �� �� ������ ���ϴ� INF������ �ʱ�ȭ
	}
}

// ������ �߰��ϴ� �Լ�.
void insertEdge(Graph* g, int start, int end, int weight) {
	g->edges[g->nEdge].start = start; // ���� ���� ��ȣ ����
	g->edges[g->nEdge].end = end; // �� ���� ��ȣ ����
	g->edges[g->nEdge].weight = weight; // ����ġ �� ����
	g->nEdge++; // ���� ������ �Էµ� index�� ������ְ�, �������� ������ ������ ����
}

// qsort �Լ��� ����ϱ� ���� ���Լ�
// �ݵ�� int�� ��ȯ�� const void* Ÿ�� �Ű����� 2���� �ʿ��ϴ�.
int compare(const void* a, const void* b) {
	Edge* x = (Edge*)a;
	Edge* y = (Edge*)b;
	return (x->weight - y->weight); // �������� ����
}

// kruskal �˰����� �����ϴ� �Լ�
void kruskal(Graph* g, int nVtx) {
	initParent(nVtx); // �θ� �迭�� �ʱ�ȭ �Ѵ�.
	qsort(g->edges, g->nEdge, sizeof(Edge), compare); // ���� ���� �Լ�

	for (int i = 0; i < g->nEdge; ++i) {
		Edge e = g->edges[i]; // ���� �ϳ�(���� ���� ����ġ�� ����)�� ����ü ���� e�� ����
		int u = findParent(e.start); // ������ ���� ������ �θ� ��ȯ
		int v = findParent(e.end); // ������ �� ������ �θ� ��ȯ

		// u�� v���� �ٸ��ٴ� ���� �θ� �ٸ��ٴ� ��.
		// ��, ���� �ٸ� �׷����� ����������, �����ص� ����Ŭ�� �߻����� �ʴ´�.
		if (u != v) { unionParent(u, v); }
	}
}

// ���� ������ �׷����� ����ġ �߻���Ű�� �Լ�
void randomCompleteGraph(Graph* g, int nVtx) {
	// ����(�����) �׷����̱� ������ �������θ� �����ϴ� ������ �߰��ص� ������ ����Ǿ��ٰ� ����.
	// "���� ������ ��ȣ < �� ������ ��ȣ" ��� ��Ģ�� ���� ������ �߰��ߴ�.
	// ���� ����ġ ���� ������ 1~100���� �����ߴ�.
	for (int i = 0; i < nVtx; ++i) {
		for (int j = i; j < nVtx; ++j) {
			if (i == j) continue; // ��ü ����(���� ������ �� ������ ����)�� ������ �߰��� �� ����.
			insertEdge(g, i, j, rand() % MAX_WEIGHT + 1);
		}
	}
}

int main() {
	srand(time(NULL));
	Graph g;

	// ���� �� ������ �ð� ������ ���ؼ� ����
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double CurrentTimeResult;
	printf("\n * Kruskal �˰��� ������ �����ϰڽ��ϴ�.\n\n");
	// �ŷڼ� �ִ� �����͸� �����ϱ� ���� 1,000�� �ݺ� �� ��� �� ����.
	for (int i = 1; i <= 1000; ++i) {
		if (i % 100 == 0) {
			printf(" * %3d%% ���� �Ϸ��߽��ϴ�.\n", i / 10);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // �ð� ���� ����

			initGraph(&g, testcase); // �׷��� �ʱ�ȭ
			randomCompleteGraph(&g, testcase); // ���� �׷��� ����
			kruskal(&g, testcase); // Kruskal �˰��� ����

			QueryPerformanceCounter(&EndTime); // �ð� ���� ����
			CurrentTimeResult = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
			TimeRepository[(testcase - 5) / 10] += CurrentTimeResult;
		}
	}
	printf("============================================================================\n");
	for (int i = 0; i < 21; ++i) {
		printf(" * ������ ������ %3d���� ��, ��� ����ð��� %lf �Դϴ�.\n", i * 10 + 5, TimeRepository[i] / 1000);
	}

	return 0;
}