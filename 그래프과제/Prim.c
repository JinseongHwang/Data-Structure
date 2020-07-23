
// MST - Prim �˰������� ����

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // �ִ� ����ġ�� 100���� ����
#define MAX_VERTICES 205 // ������ �ִ� ������ 205���� ����
#define INF 987654321 // ������ ����ġ�� ǥ����

// ������ �����߱� ������ false �Ǵ� 0���� �ʱ�ȭ��
bool selected[MAX_VERTICES];
int distance[MAX_VERTICES];
double TimeRepository[21]; // ����ð� ���� ���� ����

typedef struct Graph {
	int nVtx; // ������ ����
	int weight[MAX_VERTICES][MAX_VERTICES]; // ���� ���� ����ġ ���� ����
} Graph;

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
		selected[i] = false; // false���� �׷����� ���Ե��� �ʾ����� �ǹ��Ѵ�.
		distance[i] = INF; // �ʱ⿡�� �ƹ� ������ �Ÿ��� �� �� ���� ������ INF������ �ʱ�ȭ�Ѵ�.
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

int getMinVertex(int nVtx) {
	int minVertex = 0, minDistance = INF;
	for (int i = 0; i < nVtx; ++i) {
		// ���� Ʈ���� ���ԵǾ� ���� ������ && ���� ���� ����ġ�� ������ ��� ��ȣ�� ��ȯ
		// ������ ����� �迭���� �ּҰ��� ã�� �˰���� �ٸ� ���� ����
		if (!selected[i] && distance[i] < minDistance) {
			// ���� ���� �Ÿ� ���� minDistance�� ����
			// ���� �ݺ� �� �� ���� ���� �������� �����ϴ� ����
			minDistance = distance[i]; 
			minVertex = i; // minVertex���� ���� ���� ���� ���� ���� ��ȣ�� ��� ���ŵ�
		}
	}
	// �ᱹ ���� ���� ���� ���� ���� ��ȣ�� ��ȯ�Ѵ�.
	return minVertex;
}

void prim(Graph* g, int start) {
	distance[start] = 0; // �ڱ� �ڽ������� ����ġ�� 0�̴�.
	for (int i = 0; i < g->nVtx; ++i) { // ������ ������ŭ �ݺ�
		int minVertex = getMinVertex(g->nVtx); // �� �� �ִ� ���� �� ���� ����� ������ ��ȣ�� ��ȯ����
		selected[minVertex] = true; // ���� ����� ������ �׷����� ���Խ�Ų��.
		// ��ΰ� �������� ���� ��� ����ó��: �������� ��Ȳ�̶�� �߻����� ����
		if (distance[minVertex] == INF) return;

		// ���� �� �� ���� ��ΰ� ���Ӱ� �߰ߵǾ� INF���� ��� �Ÿ��� �ٲ��ִ� ���� �Ǵ�,
		// ������ ����Ǿ� �ִ� ��� �Ÿ����� ����� �Ÿ��� �߰ߵǾ� �� ������ �����ϴ� ����
		for (int currVertex = 0; currVertex < g->nVtx; ++currVertex) {
			// ���� �����ٰ� ���õ� �������κ��� �� �� �ִ� �ٸ� ������� �Ÿ� ��
			if (g->weight[minVertex][currVertex] != INF) {
				// ���� Ʈ���� ���ԵǾ� ���� ������ && ���� �Ÿ��� ������ ����� �Ÿ����� �����ٸ�
				if (!selected[currVertex] && g->weight[minVertex][currVertex] < distance[currVertex])
					distance[currVertex] = g->weight[minVertex][currVertex]; // �����Ѵ�.
			}
		}
	}
}

int main() {
	srand(time(NULL));
	Graph g;
	
	// ���� �� ������ �ð� ������ ���ؼ� ����
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double CurrentTimeResult;
	printf("\n * Prim �˰��� ������ �����ϰڽ��ϴ�.\n\n");
	// �ŷڼ� �ִ� �����͸� �����ϱ� ���� 1,000�� �ݺ� �� ��� �� ����.
	for (int i = 1; i <= 1000; ++i) {
		if (i % 100 == 0) {
			printf(" * %3d%% ���� �Ϸ��߽��ϴ�.\n", i / 10);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // �ð� ���� ����

			g.nVtx = testcase; // ������ ����
			initGraph(&g, g.nVtx); // �׷��� ���� �ʱ�ȭ
			randomCompleteGraph(&g, g.nVtx); // ���� ����ġ�� ������ �׷��� ����
			// �� ��° �Ű������� ���� ���� ����̴�.
			// �ƹ� ��峪 ���� ������� ������ ��ǥ�� 0�� �־���.
			prim(&g, 0);
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