
// 최단 경로 - Floyd 알고리즘으로 구현

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // 최대 가중치는 100으로 설정
#define MAX_VERTICES 205 // 정점의 최대 개수는 205개로 설정
#define INF 987654321 // 무한한 가중치를 표현함

double TimeRepository[21]; // 실행시간 누적 값을 저장

typedef struct Graph {
	int nVtx; // 정점의 개수
	int weight[MAX_VERTICES][MAX_VERTICES]; // 가중치
} Graph;

// 그래프 연산에 관련되는 변수에 초기값을 부여한다.
// 다음 test case에 영향을 미치지 않게 하기위해서이다.
void initGraph(Graph* g, int nVtx) {
	for (int i = 0; i < nVtx; ++i) {
		// i부터 초기화하는 이유는 완전 그래프 가중치 행렬 특성 상
		// (n, n) 으로 이어지는 선을 중심으로 대칭되는 값들을 가지고 있기 때문이다.
		// 따라서 아래와 같이 작성하는 것이 시간복잡도 측면에서 유리하다고 볼 수 있다.
		for (int j = i; j < nVtx; ++j) {
			if (i == j) { g->weight[i][j] = 0; } // 자기 자신으로의 가중치는 0이다.
			else { g->weight[i][j] = g->weight[j][i] = INF; } // 선대칭적으로 INF값을 대입
		}
	}
}

// nVtx * nVtx 크기의 인접 행렬에 랜덤 값으로 가중치 부여
void randomCompleteGraph(Graph* g, int nVtx) {
	for (int row = 0; row < nVtx; ++row) {
		for (int cal = 0; cal < nVtx; ++cal) {
			// 자체 간선의 가중치는 0이다. || 가중치가 INF가 아니라면 이미 가중치가 부여되었다.
			if (row == cal || g->weight[row][cal] != INF || g->weight[cal][row] != INF) continue;
			// 무향 그래프이기 때문에 대칭적으로 똑같은 값을 대입한다. (1 ~ MAX_WEIGHT)
			g->weight[row][cal] = g->weight[cal][row] = rand() % MAX_WEIGHT + 1;
		}
	}
}

// Floyd 알고리즘을 수행
void floyd(Graph* g, int nVtx) {
	for (int via = 0; via < nVtx; ++via) { // 거쳐갈 노드
		for (int i = 0; i < nVtx; ++i) { // i번째 노드로부터
			for (int j = 0; j < nVtx; ++j) { // j번째 노드로 가는
				// via번째 정점을 거쳐서 갈 경우 더 적은 비용이 드는 경우
				// 가중치 그래프(인접 행렬)를 적은 비용으로 갱신해준다.
				if (g->weight[i][j] > g->weight[i][via] + g->weight[via][j])
					g->weight[i][j] = g->weight[i][via] + g->weight[via][j];
			}
		}
	}
}

int main() {
	srand(time(NULL));
	Graph g;
	
	// 조금 더 정밀한 시간 측정을 위해서 선언
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double CurrentTimeResult;
	printf("\n * Floyd 알고리즘 연산을 수행하겠습니다.\n\n");
	// 신뢰성 있는 데이터를 추출하기 위해 100번 반복 후 평균 값 구함.
	for (int i = 1; i <= 100; ++i) {
		if (i % 10 == 0) {
			printf(" * %3d%% 연산 완료했습니다.\n", i);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // 시간 측정 시작

			g.nVtx = testcase; // 정점의 개수

			initGraph(&g, g.nVtx); // 그래프를 초기화한다
			randomCompleteGraph(&g, g.nVtx); // 그래프에 랜덤 가중치 값 부여
			floyd(&g, g.nVtx); // Floyd 알고리즘 수행

			QueryPerformanceCounter(&EndTime); // 시간 측정 종료
			CurrentTimeResult = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
			TimeRepository[(testcase - 5) / 10] += CurrentTimeResult;
		}
	}
	printf("============================================================================\n");
	for (int i = 0; i < 21; ++i) {
		printf(" * 정점의 개수가 %3d개일 때, 평균 실행시간은 %lf 입니다.\n", i * 10 + 5, TimeRepository[i] / 100);
	}

	return 0;
}