
// �ִ� ��� - Floyd �˰������� ����

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // �ִ� ����ġ�� 100���� ����
#define MAX_VERTICES 205 // ������ �ִ� ������ 205���� ����
#define INF 987654321 // ������ ����ġ�� ǥ����

double TimeRepository[21]; // ����ð� ���� ���� ����

typedef struct Graph {
	int nVtx; // ������ ����
	int weight[MAX_VERTICES][MAX_VERTICES]; // ����ġ
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

// Floyd �˰����� ����
void floyd(Graph* g, int nVtx) {
	for (int via = 0; via < nVtx; ++via) { // ���İ� ���
		for (int i = 0; i < nVtx; ++i) { // i��° ���κ���
			for (int j = 0; j < nVtx; ++j) { // j��° ���� ����
				// via��° ������ ���ļ� �� ��� �� ���� ����� ��� ���
				// ����ġ �׷���(���� ���)�� ���� ������� �������ش�.
				if (g->weight[i][j] > g->weight[i][via] + g->weight[via][j])
					g->weight[i][j] = g->weight[i][via] + g->weight[via][j];
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
	printf("\n * Floyd �˰��� ������ �����ϰڽ��ϴ�.\n\n");
	// �ŷڼ� �ִ� �����͸� �����ϱ� ���� 100�� �ݺ� �� ��� �� ����.
	for (int i = 1; i <= 100; ++i) {
		if (i % 10 == 0) {
			printf(" * %3d%% ���� �Ϸ��߽��ϴ�.\n", i);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // �ð� ���� ����

			g.nVtx = testcase; // ������ ����

			initGraph(&g, g.nVtx); // �׷����� �ʱ�ȭ�Ѵ�
			randomCompleteGraph(&g, g.nVtx); // �׷����� ���� ����ġ �� �ο�
			floyd(&g, g.nVtx); // Floyd �˰��� ����

			QueryPerformanceCounter(&EndTime); // �ð� ���� ����
			CurrentTimeResult = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
			TimeRepository[(testcase - 5) / 10] += CurrentTimeResult;
		}
	}
	printf("============================================================================\n");
	for (int i = 0; i < 21; ++i) {
		printf(" * ������ ������ %3d���� ��, ��� ����ð��� %lf �Դϴ�.\n", i * 10 + 5, TimeRepository[i] / 100);
	}

	return 0;
}