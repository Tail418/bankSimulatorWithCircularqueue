#include <stdio.h>
#include <stdlib.h>
#include <time.h> // srand 함수를 위해 추가

// ===== 원형 큐(Circular Queue) 구현 =====
#define MAX_QUEUE_SIZE 5

// 고객 정보를 저장할 구조체 정의
typedef struct {
	int id;             // 고객 ID
	int arrival_time;   // 도착 시간
	int service_time;   // 서비스 소요 시간
} element;

// 큐를 관리할 구조체 정의
typedef struct {
	element data[MAX_QUEUE_SIZE];
	int front, rear;
} Queue;

// 에러 메시지 출력 함수
void error(char* message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}

// 큐 초기화 함수
void init_queue(Queue* q)
{
	q->front = q->rear = 0;
}

// 큐가 비어있는지 확인하는 함수
int is_empty(Queue* q)
{
	return (q->front == q->rear);
}

// 큐가 꽉 찼는지 확인하는 함수
int is_full(Queue* q)
{
	return ((q->rear + 1) % MAX_QUEUE_SIZE == q->front);
}

// 큐에 데이터를 추가하는 함수 (Enqueue)
void enqueue(Queue* q, element item)
{
	if (is_full(q))
		error("큐가 포화상태입니다.");
	q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
	q->data[q->rear] = item;
}

// 큐에서 데이터를 꺼내는 함수 (Dequeue)
element dequeue(Queue* q)
{
	if (is_empty(q))
		error("큐가 공백상태입니다.");
	q->front = (q->front + 1) % MAX_QUEUE_SIZE;
	return q->data[q->front];
}

// ===== 메인 프로그램 (은행 창구 시뮬레이션) =====
int main(void)
{
	int minutes = 60;       // 시뮬레이션 총 시간 (60분)
	int total_wait = 0;     // 전체 고객의 총 대기 시간
	int total_customers = 0;// 총 방문 고객 수
	int service_time = 0;   // 현재 서비스 중인 고객의 남은 시간
	int service_customer_id = -1; // 현재 서비스 중인 고객의 ID

	Queue q;
	init_queue(&q);

	srand(time(NULL)); // 매번 다른 난수를 생성하기 위해 초기화

	// 시뮬레이션 시작 (1분 단위로 진행)
	for (int clock = 0; clock < minutes; clock++) {
		printf("현재시간=%d분\n", clock);

		// 30% 확률로 새로운 고객 도착
		if ((rand() % 10) < 3) {
			element customer;
			customer.id = total_customers++;
			customer.arrival_time = clock;
			customer.service_time = rand() % 3 + 1; // 서비스 시간은 1~3분 사이로 무작위 설정
			
            if (!is_full(&q)) {
                enqueue(&q, customer);
			    printf(" -> 고객 %d이 %d분에 도착했습니다. (서비스 필요 시간: %d분)\n",
				    customer.id, customer.arrival_time, customer.service_time);
            } else {
                printf(" -> 줄이 꽉 차서 고객 %d이 돌아갔습니다.\n", total_customers - 1);
            }
		}

		// 현재 서비스 중인 고객이 있는지 확인
		if (service_time > 0) {
			printf("   ... 고객 %d 서비스 처리 중입니다.\n", service_customer_id);
			service_time--; // 남은 서비스 시간 1분 감소
		}
		// 서비스 중인 고객이 없고, 대기 줄에 고객이 있다면 새로운 서비스 시작
		else {
			if (!is_empty(&q)) {
				element customer = dequeue(&q);
				service_customer_id = customer.id;
				service_time = customer.service_time - 1; // 서비스를 1분 진행했으므로 1 빼줌
				int wait_time = clock - customer.arrival_time;
				total_wait += wait_time;
				printf(" => 대기하던 고객 %d이(가) 서비스를 시작합니다. (대기 시간: %d분)\n",
					customer.id, wait_time);
			}
		}
	}

	printf("\n========================================\n");
	printf("시뮬레이션 종료\n");
	printf("총 대기 시간: %d분\n", total_wait);
	printf("총 방문 고객: %d명\n", total_customers);
	if (total_customers > 0) {
		printf("평균 대기 시간: %.2f분\n", (double)total_wait / total_customers);
	}
	printf("========================================\n");
	return 0;
}