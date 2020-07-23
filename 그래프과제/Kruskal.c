
// MST - Kruskal 알고리즘으로 구현

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define MAX_WEIGHT 100 // 최대 가중치는 100으로 설정
#define MAX_VERTICES 205 // 정점의 최대 개수는 205개로 설정
#define MAX_EDGES (MAX_VERTICES * (MAX_VERTICES - 1) / 2) // 최대 간선 개수
#define NUM_EDGE(X) (((X) * ((X) - 1)) / 2) // 정점이 X개일 때 최대 간선 개수
#define INF 987654321 // 무한한 가중치를 표현함

// 전역에 선언했기 때문에 false 또는 0으로 초기화됨
int parent[MAX_VERTICES]; // 부모 정점의 index를 저장 및 갱신
double TimeRepository[21]; // 실행시간 누적 값을 저장

// ===== Union Find 알고리즘 =====
// 부모 정보를 저장하는 parent 배열을 초기화한다.
void initParent(int nVtx) {
	// 정점의 개수 만큼만 배열을 사용한다.
	// 본인이 조상 노드이거나 단독 노드일 경우 -1 부모값을 가진다.
	for (int i = 0; i < nVtx; ++i) {
		parent[i] = -1;
	}
}

// 매개변수로 받은 curr의 조상 노드를 찾아서 반환한다.
int findParent(int curr) {
	// 본인이 -1일 경우 본인이 조상 노드이거나 단독 노드이다.
	if (parent[curr] == -1) {
		return curr;
	}
	// 부모 값 -1을 가진 노드를 찾아서 반환한다.
	while (parent[curr] != -1) {
		curr = parent[curr];
	}
	return curr;
}

// 부모 노드의 번호를 합친다.
// 서로 포함하지 않는 두 그래프를 a 노드와 b 노드를 기준으로 연결한다.
void unionParent(int a, int b) {
	// 두 노드의 조상 번호를 findParent() 함수를 사용해서 반환받는다.
	a = findParent(a);
	b = findParent(b);
	// 만약 두 노드의 조상이 다르다면,
	// (서로 다른 그래프에 속해있다면)
	if (a != b) {
		// a노드의 부모를 b로 설정한다.
		// 반대로 설정해도 무관하다.
		parent[a] = b;
	}
}
// =========================
// 간선 하나가 저장하는 데이터
typedef struct Edge {
	// (시작 정점, 끝 정점, 가중치)
	int start, end, weight;
} Edge;

// 그래프의 데이터.
typedef struct Graph {
	int nEdge; // 간선의 개수
	Edge edges[MAX_EDGES]; // 간선의 개수만큼 배열에 저장
} Graph;

// 그래프의 정보를 초기화한다. 정점의 개수를 매개변수로 받는다.
void initGraph(Graph* g, int nVtx) {
	g->nEdge = 0; // 최초 그래프는 간선의 개수가 0개이다.
	for (int i = 0; i < NUM_EDGE(nVtx); ++i) { // 정점 개수에 따른 최대 간선의 개수만큼만 반복한다.
		g->edges[i].start = 0; // 시작 정점의 값을 0으로 초기화
		g->edges[i].end = 0; // 끝 정점의 값을 0으로 초기화
		g->edges[i].weight = INF; // 가중치는 갈 수 없음을 뜻하는 INF값으로 초기화
	}
}

// 간선을 추가하는 함수.
void insertEdge(Graph* g, int start, int end, int weight) {
	g->edges[g->nEdge].start = start; // 시작 정점 번호 저장
	g->edges[g->nEdge].end = end; // 끝 정점 번호 저장
	g->edges[g->nEdge].weight = weight; // 가중치 값 저장
	g->nEdge++; // 다음 간선이 입력될 index를 만들어주고, 실질적인 간선의 개수를 저장
}

// qsort 함수를 사용하기 위한 비교함수
// 반드시 int형 반환과 const void* 타입 매개변수 2개가 필요하다.
int compare(const void* a, const void* b) {
	Edge* x = (Edge*)a;
	Edge* y = (Edge*)b;
	return (x->weight - y->weight); // 오름차순 정렬
}

// kruskal 알고리즘을 수행하는 함수
void kruskal(Graph* g, int nVtx) {
	initParent(nVtx); // 부모 배열을 초기화 한다.
	qsort(g->edges, g->nEdge, sizeof(Edge), compare); // 빠른 정렬 함수

	for (int i = 0; i < g->nEdge; ++i) {
		Edge e = g->edges[i]; // 간선 하나(가장 작은 가중치를 가진)를 구조체 변수 e에 대입
		int u = findParent(e.start); // 간선의 시작 정점의 부모를 반환
		int v = findParent(e.end); // 간선의 끝 정점의 부모를 반환

		// u와 v값이 다르다는 것은 부모가 다르다는 것.
		// 즉, 서로 다른 그래프에 속해있으며, 연결해도 사이클이 발생하지 않는다.
		if (u != v) { unionParent(u, v); }
	}
}

// 랜덤 값으로 그래프의 가중치 발생시키는 함수
void randomCompleteGraph(Graph* g, int nVtx) {
	// 무향(양방향) 그래프이기 때문에 한쪽으로만 연결하는 간선을 추가해도 양쪽이 연결되었다고 본다.
	// "시작 정점의 번호 < 끝 정점의 번호" 라는 규칙을 만들어서 간선을 추가했다.
	// 랜덤 가중치 값의 범위는 1~100으로 설정했다.
	for (int i = 0; i < nVtx; ++i) {
		for (int j = i; j < nVtx; ++j) {
			if (i == j) continue; // 자체 정점(시작 정점과 끝 정점이 같은)은 간선을 추가할 수 없다.
			insertEdge(g, i, j, rand() % MAX_WEIGHT + 1);
		}
	}
}

int main() {
	srand(time(NULL));
	Graph g;

	// 조금 더 정밀한 시간 측정을 위해서 선언
	LARGE_INTEGER BeginTime, EndTime, Frequency;
	double CurrentTimeResult;
	printf("\n * Kruskal 알고리즘 연산을 수행하겠습니다.\n\n");
	// 신뢰성 있는 데이터를 추출하기 위해 1,000번 반복 후 평균 값 구함.
	for (int i = 1; i <= 1000; ++i) {
		if (i % 100 == 0) {
			printf(" * %3d%% 연산 완료했습니다.\n", i / 10);
		}
		for (int testcase = 5; testcase <= 205; testcase += 10) {
			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&BeginTime); // 시간 측정 시작

			initGraph(&g, testcase); // 그래프 초기화
			randomCompleteGraph(&g, testcase); // 랜덤 그래프 생성
			kruskal(&g, testcase); // Kruskal 알고리즘 실행

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