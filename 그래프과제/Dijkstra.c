
// 최단 경로 - Dijkstra 알고리즘으로 구현

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // 최대 가중치는 100으로 설정
#define MAX_VERTICES 205 // 정점의 최대 개수는 205개로 설정
#define INF 987654321 // 무한한 가중치를 표현함

typedef struct Graph {
	int nVtx; // 정점의 개수
	int weight[MAX_VERTICES][MAX_VERTICES]; // 가중치
} Graph;

bool visit[MAX_VERTICES]; // 방문 정점에 true
int distance[MAX_VERTICES]; // 시작 정점으로부터 최단 거리 저장
double TimeRepository[21]; // 실행시간 누적 값을 저장

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
		visit[i] = false; // false값이 그래프에 포함되지 않았음을 의미한다.
		distance[i] = INF; // 초기에는 아무 정점의 거리도 알 수 없기 때문에 INF값으로 초기화한다.
	}
}

int getMinVertex(int nVtx) {
	// 최소 가중치, 최소 가중치를 가지는 index 를 초기화한다.
	int minWeight = INF, minIndex = -1;
	for (int i = 0; i < nVtx; ++i) { // 정점의 개수만큼 반복
		// 방문하지 않았으며, minWeight를 갱신하며 distance 배열의 최소값을 찾아냄
		if (!visit[i] && distance[i] < minWeight) {
			minWeight = distance[i]; // 갱신
			minIndex = i; // 갱신
		}
	}
	return minIndex; // 가장 작은 가중치를 가지는 정점의 번호를 반환
}

void dijkstra(Graph* g, int start) {
	for (int i = 0; i < g->nVtx; ++i) {
		// 현재 start 정점에서 각 정점으로의 거리로 갱신
		distance[i] = g->weight[start][i];
	}
	visit[start] = true; // 시작 노드에 방문함
	distance[start] = 0; // 시작 노드로의 거리는 0이다.
	for (int i = 0; i < g->nVtx; ++i) { // 정점의 개수만큼 반복
		int u = getMinVertex(g->nVtx); // 가장 작은 가중치를 가지는 정점의 번호를 반환받음
		visit[u] = true; // 정점에 방문 표시
		for (int v = 0; v < g->nVtx; ++v) {
			// 방문하지 않았으며 &&
			// 시작 정점부터 현재 정점까지 거리 + v번 정점까지 거리 < distance 배열에 저장된 v번 정점까지 거리
			// 조건을 만족할 경우 갱신을 해준다.
			if (!visit[v] && distance[u] + g->weight[u][v] < distance[v])
				distance[v] = distance[u] + g->weight[u][v];
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

int main() {
	srand(time(NULL));
	Graph g;

	// 조금 더 정밀한 시간 측정을 위해서 선언
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double CurrentTimeResult;
	printf("\n * Dijkstra 알고리즘 연산을 수행하겠습니다.\n\n");
	// 신뢰성 있는 데이터를 추출하기 위해 1,000번 반복 후 평균 값 구함.
	for (int i = 1; i <= 1000; ++i) {
		if (i % 100 == 0) {
			printf(" * %3d%% 연산 완료했습니다.\n", i / 10);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // 시간 측정 시작

			g.nVtx = testcase; // 정점의 개수
			
			initGraph(&g, g.nVtx); // 그래프 데이터 초기화
			randomCompleteGraph(&g, g.nVtx); // 랜덤 가중치를 가지는 그래프 생성
			// 0번 정점에서 모든 정점으로 가는 최단 경로를 구함
			dijkstra(&g, 0); 

			QueryPerformanceCounter(&EndTime); // 시간 측정 종료
			CurrentTimeResult = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
			TimeRepository[(testcase - 5) / 10] += CurrentTimeResult;
		}
	}
	printf("============================================================================\n");
	for (int i = 0; i < 21; ++i) {
		printf(" * 정점의 개수가 %3d개일 때, 평균 실행시간은 %lf 입니다. => x %3d => %lf\n",
			i * 10 + 5, TimeRepository[i] / 1000, i * 10 + 5, ((TimeRepository[i] / 1000) * (double)((i * 10) + 5)));
	}

	return 0;
}