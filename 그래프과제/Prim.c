
// MST - Prim 알고리즘으로 구현

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // 최대 가중치는 100으로 설정
#define MAX_VERTICES 205 // 정점의 최대 개수는 205개로 설정
#define INF 987654321 // 무한한 가중치를 표현함

// 전역에 선언했기 때문에 false 또는 0으로 초기화됨
bool selected[MAX_VERTICES];
int distance[MAX_VERTICES];
double TimeRepository[21]; // 실행시간 누적 값을 저장

typedef struct Graph {
	int nVtx; // 정점의 개수
	int weight[MAX_VERTICES][MAX_VERTICES]; // 정점 간의 가중치 값을 저장
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
		selected[i] = false; // false값이 그래프에 포함되지 않았음을 의미한다.
		distance[i] = INF; // 초기에는 아무 정점의 거리도 알 수 없기 때문에 INF값으로 초기화한다.
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

int getMinVertex(int nVtx) {
	int minVertex = 0, minDistance = INF;
	for (int i = 0; i < nVtx; ++i) {
		// 아직 트리에 포함되어 있지 않으며 && 가장 작은 가중치를 가지는 노드 번호를 반환
		// 정수가 저장된 배열에서 최소값을 찾는 알고리즘과 다를 것이 없음
		if (!selected[i] && distance[i] < minDistance) {
			// 가장 작은 거리 값을 minDistance에 대입
			// 다음 반복 시 더 작은 값이 들어오도록 유도하는 역할
			minDistance = distance[i]; 
			minVertex = i; // minVertex에는 가장 작은 값을 가진 정점 번호로 계속 갱신됨
		}
	}
	// 결국 가장 작은 값을 가진 정점 번호를 반환한다.
	return minVertex;
}

void prim(Graph* g, int start) {
	distance[start] = 0; // 자기 자신으로의 가중치는 0이다.
	for (int i = 0; i < g->nVtx; ++i) { // 정점의 개수만큼 반복
		int minVertex = getMinVertex(g->nVtx); // 갈 수 있는 정점 중 가장 가까운 정점의 번호를 반환받음
		selected[minVertex] = true; // 가장 가까운 정점을 그래프에 포함시킨다.
		// 경로가 존재하지 않을 경우 에러처리: 정상적인 상황이라면 발생하지 않음
		if (distance[minVertex] == INF) return;

		// 직접 갈 수 없던 경로가 새롭게 발견되어 INF에서 상수 거리로 바꿔주는 과정 또는,
		// 기존에 저장되어 있던 상수 거리보다 가까운 거리가 발견되어 그 정보를 갱신하는 과정
		for (int currVertex = 0; currVertex < g->nVtx; ++currVertex) {
			// 가장 가깝다고 선택된 정점으로부터 갈 수 있는 다른 정점들과 거리 비교
			if (g->weight[minVertex][currVertex] != INF) {
				// 아직 트리에 포함되어 있지 않으며 && 현재 거리가 기존에 저장된 거리보다 가깝다면
				if (!selected[currVertex] && g->weight[minVertex][currVertex] < distance[currVertex])
					distance[currVertex] = g->weight[minVertex][currVertex]; // 갱신한다.
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
	printf("\n * Prim 알고리즘 연산을 수행하겠습니다.\n\n");
	// 신뢰성 있는 데이터를 추출하기 위해 1,000번 반복 후 평균 값 구함.
	for (int i = 1; i <= 1000; ++i) {
		if (i % 100 == 0) {
			printf(" * %3d%% 연산 완료했습니다.\n", i / 10);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // 시간 측정 시작

			g.nVtx = testcase; // 정점의 개수
			initGraph(&g, g.nVtx); // 그래프 정보 초기화
			randomCompleteGraph(&g, g.nVtx); // 랜덤 가중치를 가지는 그래프 생성
			// 두 번째 매개변수는 연산 시작 노드이다.
			// 아무 노드나 들어가도 상관없기 때문에 대표로 0을 넣었다.
			prim(&g, 0);
			QueryPerformanceCounter(&EndTime); // 시간 측정 종료
			CurrentTimeResult = (double)(EndTime.QuadPart - BeginTime.QuadPart) / Frequency.QuadPart;
			TimeRepository[(testcase - 5) / 10] += CurrentTimeResult;
		}
	}
	printf("============================================================================\n");
	for (int i = 0; i < 21; ++i) {
		printf(" * 정점의 개수가 %3d개일 때, 평균 실행시간은 %lf 입니다.\n", i * 10 + 5, TimeRepository[i] / 1000);
	}

	return 0;
}