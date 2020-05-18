// 원형 큐를 활용한 소개팅 주선 시뮬레이션 프로그램 구현

#pragma warning (disable : 4996) // 파일 내 불필요한 경고 해제
#include <stdio.h> // 표준입출력 헤더파일
#include <stdlib.h> // srand 함수를 사용하기 위함
#include <string.h> // 문자열 처리 함수를 사용하기 위함
#include <stdbool.h> // boolean type을 c언어에서 사용하기 위함
#include <time.h> // 시간값을 가져오기 위함

#define MAX_QUEUE_SIZE 10 // 남학생 큐와 여학생 큐에 들어갈 수 있는 노드의 개수
#define PRINT_LINE printf("\n=====================================================================\n") // 구분선 출력
enum { MALE, FEMALE }; // 가독성을 위해 열거형 사용 -> 남학생: 0, 여학생: 1

int count = 1; // 몇 번째 만남인지 출력하는 용도. 메모리 낭비를 방지하기 위해 전역변수로 1개만 선언함.
bool first_check = true; // 남학생 또는 여학생 둘 중 하나만 등록을 마쳤을 때 일괄 처리문 1번만 출력하게 하기 위함.

typedef struct {
	char* name[MAX_QUEUE_SIZE]; // 큐에 저장될 이름 배열
	int regist_time[MAX_QUEUE_SIZE]; // 등록한 시간을 저장
	int front, rear; // 큐의 데이터 위치를 가리키는 정보
	int can_meet; // 현재 만날 수 있는 사람의 수
} Meeting; // 타입을 Meeting으로 지정

void init_queue(Meeting* q); // 큐의 초기값을 지정 (초기화)
bool is_empty(Meeting* q); // 큐가 빈 상태면 true, 비지 않았다면 false를 return 한다.
bool is_full(Meeting* q); // 큐가 포화 상태면 true, 포화 상태가 아니라면 false를 return 한다.
void push(int t, Meeting* q, char* value); // push연산을 수행. (언제 등록했는지, 어떤 큐인지, 무슨 값을 넣을지)를 매개변수로 가짐.
void double_pop(int t, Meeting* q1, Meeting* q2); // pop연산을 수행하면서 두개의 큐의 front에 위치해있는 값들을 매칭(주선)시켜줌.
void already_exhausted(Meeting* q, int gender); // 이미 남학생 또는 여학생 중 하나의 큐가 등록을 마쳤을 경우 일괄 처리문을 출력함.

int main() {
	// 큐의 크기가 10이라면 들어갈 수 있는 사람의 수는 최대 (10 - 1) 9명이다. (1칸은 공백상태와 포화상태를 구분하기 위함)
	// 남학생 큐와 여학생 큐에 각각 9명씩 들어가면 큐 두 개를 합해서 최대 18명까지 들어갈 수 있다.
	int minutes = 18; // 최소값: 2, 최대값: 18
	int curr_pos[2] = { 0, }; // 현재 남학생과 여학생 이름 배열의 출력될 index number값을 저장.

	Meeting gender[2]; // gender[MALE]: 남성, gender[FEMALE]: 여성

	// 문제에서 주어진 조건: 남학생과 여학생의 이름은 미리 배열에 저장해 두고 사용.
	// 문제에서 주어진 조건: 학생의 성별은 무작위로 선택한다 -> 랜덤값을 통한 접근을 위해 남성은 [0][], 여성은 [1][] 으로 구성함.
	char* name[2][MAX_QUEUE_SIZE - 1] = { { "유재석", "강호동", "권지용", "싸이", "장동건", "김종국", "성시경", "윤민수", "강동원" },
						{ "아이유", "수지", "전지현", "송혜교", "이영애", "박신혜", "이효리", "이선희", "백지영" } };
	// 큐 2개의 front, rear, can_meet 값 초기화
	init_queue(&gender[MALE]);
	init_queue(&gender[FEMALE]);

	srand(time(NULL)); // 시간에 따른 난수 발생 조건
	// 문제에서 주어진 조건: 단위 시간 당 한 명의 학생이 등록할 수 있다.
	for (int clock = 0; clock < minutes; ++clock) {
		PRINT_LINE;
		int random = rand() % 2; // 성별 결정: int형 변수 random 에는 0 또는 1 이 무작위로 들어간다.
		if (curr_pos[MALE] == MAX_QUEUE_SIZE - 1) { // 모든 남학생이 등록을 마쳤을 경우
			random = 1; // random값은 1(여학생)으로 고정
			if (first_check) already_exhausted(&gender[MALE], MALE); // 일괄 처리 안내문 출력
		}
		else if (curr_pos[FEMALE] == MAX_QUEUE_SIZE - 1) {  // 모든 여학생이 등록을 마쳤을 경우
			random = 0; // random값은 0(남학생)으로 고정
			if (first_check) already_exhausted(&gender[FEMALE], FEMALE); // 일괄 처리 안내문 출력
		}
		printf("\n 현재시각 ==> %d\n", clock); // 현재 시각(임의의 단위 시간)을 출력

		char* tmp_gender = (random == 0 ? "남학생" : "여학생"); // random 값에 대한 성별 문자열 결정

		printf(" %s 한 명이 등록을 완료했습니다.\n", tmp_gender); // 성별 알림문 출력
		push(clock, &gender[random], name[random][curr_pos[random]]); // random값(무작위 성별)에 의존한 push 연산 수행

		if (gender[MALE].can_meet > 0 && gender[FEMALE].can_meet > 0) { // 현재 만날 수 있는 인원이 0보다 큰 양수일 경우(1 이상일 경우)
			double_pop(clock, &gender[MALE], &gender[FEMALE]); // 매칭(주선) 시도
		}

		curr_pos[random]++; // 다음사람 지칭
	}
	PRINT_LINE;
	return 0;
}

void init_queue(Meeting* q) { // 큐의 초기값을 지정 (초기화)
	q->front = 0;
	q->rear = 0;
	q->can_meet = 0;
}

bool is_empty(Meeting* q) { // 큐가 빈 상태면 true, 비지 않았다면 false를 return 한다.
	return (q->front == q->rear); // 큐의 front값과 rear값이 동일한 상태는 비어있는 상태이다.(원형큐의 정의)
}

bool is_full(Meeting* q) { // 큐가 포화 상태면 true, 포화 상태가 아니라면 false를 return 한다.
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front); // 큐의 (rear + 1) % MAX_QUEUE_SIZE의 값이 front값과 같다면 포화상태이다.(원형큐의 정의)
}

void push(int t, Meeting* q, char* value) { // push연산을 수행. (언제 등록했는지, 어떤 큐인지, 무슨 값을 넣을지)를 매개변수로 가짐.
	if (is_full(q)) { // 큐가 꽉 찼으면 push연산을 수행할 수 없기 때문에 에러 메시지 출력 후 return한다.
		printf(" 등록 명단이 꽉 찼습니다!!\n");
		return;
	}
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;  // rear값을 하나 올린다.
	q->regist_time[q->rear] = t; // push연산이 일어나는 시점의 시간을 저장
	q->name[q->rear] = strdup(value); // name 배열의 rear번째 칸에 입력값을 대입한다.
	
	q->can_meet++; // 만날 수 있는 사람 수를 하나 올린다.
}

void double_pop(int t, Meeting* q1, Meeting* q2) { // pop연산을 수행하면서 두개의 큐의 front에 위치해있는 값들을 매칭(주선)시켜줌.
	if (is_empty(q1) || is_empty(q2)) { // 큐가 비어있는데 매칭(주선)을 할 수 없다.
		printf(" 주선 할 사람이 없습니다!!\n");
		return;
	}
	q1->front = (q1->front + 1) % MAX_QUEUE_SIZE; // 첫번째 큐의 front값을 하나 올린다.
	q2->front = (q2->front + 1) % MAX_QUEUE_SIZE; // 두번째 큐의 front값을 하나 올린다.

	printf("\n ** %d 번째 만남 **\n", count); // 몇 번째 만남인지 알림문 출력
	// 만난이 성사된 시간, 남학생 이름, 여학생 이름을 대입 및 출력
	printf(" ** %d 분에 %s 님과 %s 님이 만나게 되었습니다!!\n", t, q1->name[q1->front], q2->name[q2->front]);
	printf(" ** 축하의 박수를 부탁드립니다!! 짝 짝 짝 \n\n"); // 응원메시지

	// 대기시간을 계산: 현재 시간에서 등록된 시간의 차를 구했다. 
	printf(" ** %s 님의 대기시간: %d 분\n", q1->name[q1->front], t - q1->regist_time[count]);
	printf(" ** %s 님의 대기시간: %d 분\n", q2->name[q2->front], t - q2->regist_time[count]);
	
	q1->can_meet--; q2->can_meet--; // 두 큐에서 만날 수 있는 사람 수를 하나씩 내린다: 매칭 완료됐기 때문.
	count++; // 다음 만남을 위해 만남 횟수를 증가
}

void already_exhausted(Meeting* q, int gender) { // 이미 남학생 또는 여학생 중 하나의 큐가 등록을 마쳤을 경우 일괄 처리문을 출력함.
	first_check = false; // 한 번만 출력하기 위함
	char* main_gender = (gender == 0 ? "남학생" : "여학생"); // 등록을 마친 성별
	char* sub_gender = (gender == 0 ? "여학생" : "남학생"); // 등록을 마치지 않은 성별
	PRINT_LINE;
	printf("\n >>> 모든 %s 은 등록이 마감되었기 때문에 남은 %s 을 일괄 등록합니다.>>>\n", main_gender, sub_gender);
	PRINT_LINE;
}
