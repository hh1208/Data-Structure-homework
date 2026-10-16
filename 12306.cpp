#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ================== 1. 订单结构体 ==================
typedef struct {
    int order_id;          // 订单号
    char name[50];         // 旅客姓名
    char id_card[20];      // 身份证号
} TicketOrder;

// ================== 2. 链表节点结构体 ==================
typedef struct Node {
    TicketOrder data;       // 节点里存的数据（一条订单）
    struct Node *next;      // 指向下一个节点的指针
} Node;

// ================== 3. 队列结构体（用链表实现） ==================
typedef struct {
    Node *front;  // 队头指针（指向最早排队的节点）
    Node *rear;   // 队尾指针（指向最后排队的节点）
} Queue;

// ================== 4. 初始化队列 ==================
void initQueue(Queue *q) {
    q->front = NULL;
    q->rear = NULL;
}

// ================== 5. 判断队列是否为空 ==================
int isEmpty(Queue *q) {
    return q->front == NULL;
}

// ================== 6. 入队：新增候补订单 ==================
void enqueue(Queue *q, TicketOrder order) {
    // 第一步：创建一个新节点
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("? 内存分配失败，无法添加新订单！\n");
        return;
    }
    newNode->data = order;   // 把订单数据放进节点里
    newNode->next = NULL;    // 新节点是队尾，后面没人了

    // 第二步：把新节点接到队尾
    if (isEmpty(q)) {
        // 如果队列是空的，新节点既是队头也是队尾
        q->front = newNode;
        q->rear = newNode;
    } else {
        // 如果队列不为空，把新节点接在原来的队尾后面
        q->rear->next = newNode;
        q->rear = newNode;   // 更新队尾指针
    }

    printf("? 订单 %d（%s）已加入候补队列。\n", order.order_id, order.name);
}

// ================== 7. 出队：从队头取出订单 ==================
TicketOrder dequeue(Queue *q) {
    // 取出队头节点的数据
    TicketOrder order = q->front->data;

    // 保存队头节点，准备释放内存
    Node *temp = q->front;

    // 队头指针后移
    q->front = q->front->next;

    // 如果出队后队列变成空的，队尾指针也要置空
    if (q->front == NULL) {
        q->rear = NULL;
    }

    // 释放刚才出队的节点内存（防止内存泄漏）
    free(temp);

    return order;
}

// ================== 8. 余票发放 ==================
void releaseTickets(Queue *q, int k) {
    printf("\n===== 本次释放余票 %d 张 =====\n", k);

    if (k <= 0) {
        printf("余票数量无效。\n");
        return;
    }

    int success_count = 0;

    while (k > 0 && !isEmpty(q)) {
        TicketOrder order = dequeue(q);
        printf("?? 候补成功！订单号：%d，姓名：%s，身份证：%s\n",
               order.order_id, order.name, order.id_card);
        k--;
        success_count++;
    }

    printf("\n本轮共成功出票 %d 张。\n", success_count);

    if (k > 0) {
        printf("?? 余票还剩 %d 张，但候补队列已空。\n", k);
    } else if (!isEmpty(q)) {
        printf("? 余票已用完，队列中还有人在等待。\n");
    } else {
        printf("?? 所有候补订单已处理完毕，队列为空。\n");
    }
}

// ================== 9. 打印当前队列 ==================
void printQueue(Queue *q) {
    if (isEmpty(q)) {
        printf("当前候补队列为空。\n");
        return;
    }
    printf("\n当前候补队列（从队头到队尾）：\n");
    Node *current = q->front;
    int index = 1;
    while (current != NULL) {
        printf("  [%d] 订单号：%d，姓名：%s，身份证：%s\n",
               index++,
               current->data.order_id,
               current->data.name,
               current->data.id_card);
        current = current->next;
    }
    printf("\n");
}

// ================== 10. 释放整个队列（程序退出前调用） ==================
void destroyQueue(Queue *q) {
    Node *current = q->front;
    while (current != NULL) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    q->front = NULL;
    q->rear = NULL;
}

// ================== 11. 主函数（菜单交互） ==================
int main() {
    Queue q;
    initQueue(&q);

    int choice;
    int order_counter = 1001;  // 订单号自动生成

    printf("========== 12306 候补购票系统（链表版） ==========\n");

    while (1) {
        printf("\n===== 主菜单 =====\n");
        printf("1. 新增候补订单\n");
        printf("2. 余票发放\n");
        printf("3. 查看当前候补队列\n");
        printf("4. 退出程序\n");
        printf("请选择操作（1-4）：");
        scanf("%d", &choice);

        if (choice == 1) {
            TicketOrder order;
            order.order_id = order_counter++;

            printf("请输入旅客姓名：");
            scanf("%s", order.name);
            printf("请输入身份证号：");
            scanf("%s", order.id_card);

            enqueue(&q, order);

        } else if (choice == 2) {
            int k;
            printf("请输入本次释放的余票数量：");
            scanf("%d", &k);
            releaseTickets(&q, k);

        } else if (choice == 3) {
            printQueue(&q);

        } else if (choice == 4) {
            printf("感谢使用，再见！\n");
            destroyQueue(&q);  // 释放所有节点内存
            break;

        } else {
            printf("? 输入无效，请重新选择。\n");
        }
    }

    return 0;
}
