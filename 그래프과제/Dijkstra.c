
// �ִ� ��� - Dijkstra �˰������� ����

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // �ִ� ����ġ�� 100���� ����
#define MAX_VERTICES 205 // ������ �ִ� ������ 205���� ����
#define INF 987654321 // ������ ����ġ�� ǥ����

typedef struct Graph {
	int nVtx; // ������ ����
	int weight[MAX_VERTICES][MAX_VERTICES]; // ����ġ
} Graph;

bool visit[MAX_VERTICES]; // �湮 ������ true
int distance[MAX_VERTICES]; // ���� �������κ��� �ִ� �Ÿ� ����
double TimeRepository[21]; // ����ð� ���� ���� ����

// �׷��� ���꿡 ���õǴ� ������ �ʱⰪ�� �ο��Ѵ�.
// ���� test case�� ������ ��ġ�� �ʰ� �ϱ����ؼ��̴�.
void initGraph(Graph* g, int nVtx) {
	for (int i = 0; i < nVtx; ++i) {
		// i���� �ʱ�ȭ�ϴ� ������ ���� �׷��� ����ġ ��� Ư�� ��
		// (n, n) ���� �̾����� ���� �߽����� ��Ī�Ǵ� ������ ������ �ֱ� �����̴�.
		// ���� �Ʒ��� ���� �ۼ��ϴ� ���� �ð����⵵ ���鿡�� �����ϴٰ� �� �� �ִ�.
		for (int j = i; j < nVtx; ++j) {
			if (i == j) { g->weight[i][j] = 0; } // �ڱ� �ڽ������� ����ġ�� 0�̴�.
			else { g->weight[i][j] = g->weight[j][i] = INF; } // ����Ī������ INF���� ����
		}
		visit[i] = false; // false���� �׷����� ���Ե��� �ʾ����� �ǹ��Ѵ�.
		distance[i] = INF; // �ʱ⿡�� �ƹ� ������ �Ÿ��� �� �� ���� ������ INF������ �ʱ�ȭ�Ѵ�.
	}
}

int getMinVertex(int nVtx) {
	// �ּ� ����ġ, �ּ� ����ġ�� ������ index �� �ʱ�ȭ�Ѵ�.
	int minWeight = INF, minIndex = -1;
	for (int i = 0; i < nVtx; ++i) { // ������ ������ŭ �ݺ�
		// �湮���� �ʾ�����, minWeight�� �����ϸ� distance �迭�� �ּҰ��� ã�Ƴ�
		if (!visit[i] && distance[i] < minWeight) {
			minWeight = distance[i]; // ����
			minIndex = i; // ����
		}
	}
	return minIndex; // ���� ���� ����ġ�� ������ ������ ��ȣ�� ��ȯ
}

void dijkstra(Graph* g, int start) {
	for (int i = 0; i < g->nVtx; ++i) {
		// ���� start �������� �� ���������� �Ÿ��� ����
		distance[i] = g->weight[start][i];
	}
	visit[start] = true; // ���� ��忡 �湮��
	distance[start] = 0; // ���� ������ �Ÿ��� 0�̴�.
	for (int i = 0; i < g->nVtx; ++i) { // ������ ������ŭ �ݺ�
		int u = getMinVertex(g->nVtx); // ���� ���� ����ġ�� ������ ������ ��ȣ�� ��ȯ����
		visit[u] = true; // ������ �湮 ǥ��
		for (int v = 0; v < g->nVtx; ++v) {
			// �湮���� �ʾ����� &&
			// ���� �������� ���� �������� �Ÿ� + v�� �������� �Ÿ� < distance �迭�� ����� v�� �������� �Ÿ�
			// ������ ������ ��� ������ ���ش�.
			if (!visit[v] && distance[u] + g->weight[u][v] < distance[v])
				distance[v] = distance[u] + g->weight[u][v];
		}
	}
}

// nVtx * nVtx ũ���� ���� ��Ŀ� ���� ������ ����ġ �ο�
void randomCompleteGraph(Graph* g, int nVtx) {
	for (int row = 0; row < nVtx; ++row) {
		for (int cal = 0; cal < nVtx; ++cal) {
			// ��ü ������ ����ġ�� 0�̴�. || ����ġ�� INF�� �ƴ϶�� �̹� ����ġ�� �ο��Ǿ���.
			if (row == cal || g->weight[row][cal] != INF || g->weight[cal][row] != INF) continue;
			// ���� �׷����̱� ������ ��Ī������ �Ȱ��� ���� �����Ѵ�. (1 ~ MAX_WEIGHT)
			g->weight[row][cal] = g->weight[cal][row] = rand() % MAX_WEIGHT + 1;
		}
	}
}

int main() {
	srand(time(NULL));
	Graph g;

	// ���� �� ������ �ð� ������ ���ؼ� ����
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double CurrentTimeResult;
	printf("\n * Dijkstra �˰��� ������ �����ϰڽ��ϴ�.\n\n");
	// �ŷڼ� �ִ� �����͸� �����ϱ� ���� 1,000�� �ݺ� �� ��� �� ����.
	for (int i = 1; i <= 1000; ++i) {
		if (i % 100 == 0) {
			printf(" * %3d%% ���� �Ϸ��߽��ϴ�.\n", i / 10);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // �ð� ���� ����

			g.nVtx = testcase; // ������ ����
			
			initGraph(&g, g.nVtx); // �׷��� ������ �ʱ�ȭ
			randomCompleteGraph(&g, g.nVtx); // ���� ����ġ�� ������ �׷��� ����
			// 0�� �������� ��� �������� ���� �ִ� ��θ� ����
			dijkstra(&g, 0); 

			QueryPerformanceCounter(&EndTime); // �ð� ���� ����
			CurrentTimeResult = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
			TimeRepository[(testcase - 5) / 10] += CurrentTimeResult;
		}
	}
	printf("============================================================================\n");
	for (int i = 0; i < 21; ++i) {
		printf(" * ������ ������ %3d���� ��, ��� ����ð��� %lf �Դϴ�. => x %3d => %lf\n",
			i * 10 + 5, TimeRepository[i] / 1000, i * 10 + 5, ((TimeRepository[i] / 1000) * (double)((i * 10) + 5)));
	}

	return 0;
}