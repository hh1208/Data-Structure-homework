#include <stdio.h>

#define N 13   // 总共13条可通行路线

// 1. 路线表（线性表） 
typedef struct {
    char from;  // 起点
    char to;    // 终点
} Route;

Route routes[N] = {
    {'A', 'B'},  // 0
    {'A', 'C'},  // 1
    {'A', 'D'},  // 2
    {'B', 'A'},  // 3
    {'B', 'C'},  // 4
    {'B', 'D'},  // 5
    {'D', 'A'},  // 6
    {'D', 'B'},  // 7
    {'D', 'C'},  // 8
    {'E', 'A'},  // 9
    {'E', 'B'},  // 10
    {'E', 'C'},  // 11
    {'E', 'D'}   // 12
};

// 2. 冲突矩阵（13×13）
// 1 = 冲突（不能同时通行）  0 = 不冲突（可以同时通行）
// 根据图(b)的连线严格填写，对角线全为1，其余按连线填1
int conflict[N][N] = {
//    0  1  2  3  4  5  6  7  8  9 10 11 12
    { 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0 }, // 0: AB 连了 4,5,6,9
    { 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 0 }, // 1: AC 连了 5,6,7,9,10
    { 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0 }, // 2: AD 连了 9,10,11
    { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // 3: BA 没有连线
    { 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 }, // 4: BC 连了 0,7,10
    { 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0 }, // 5: BD 连了 0,1,6,10,11
    { 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0 }, // 6: DA 连了 0,1,5,10,11
    { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 }, // 7: DB 连了 1,4,11
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0 }, // 8: DC 没有连线
    { 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 }, // 9: EA 连了 0,1,2
    { 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 }, // 10: EB 连了 1,2,4,5,9
    { 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0 }, // 11: EC 连了 2,5,6,7
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }  // 12: ED 没有连线
};

//  3. 当前绿灯池（线性表） 
int current_phase[N];        // 当前这一波放行了哪些路线
int current_phase_count = 0; // 池子里现在有几条路线

//  4. 最终分组结果 
int phase_assignment[N];     // 每条路线属于第几波，初始 -1
int total_phases = 0;        // 总共用了几波

//  5. 判断：路线 id 能不能放进当前绿灯池？ 
int can_put_in(int id) {
    for (int k = 0; k < current_phase_count; k++) {
        if (conflict[id][current_phase[k]] == 1) {
            return 0;  // 和池子里某条路线冲突，不能放
        }
    }
    return 1;  // 和池子里所有路线都不冲突，可以放
}

//  6. 核心：多轮扫描分组（纯线性表逻辑） 
void divide_phases() {
    // 初始化：所有路线都还没分组
    for (int i = 0; i < N; i++) {
        phase_assignment[i] = -1;
    }

    int phase_id = 1;  // 从第1波开始

    // 只要还有路线没分组，就继续开新一轮
    while (1) {
        // 检查是否所有路线都分完组了
        int all_done = 1;
        for (int i = 0; i < N; i++) {
            if (phase_assignment[i] == -1) {
                all_done = 0;
                break;
            }
        }
        if (all_done) break;

        // 清空当前绿灯池，开始新一波
        current_phase_count = 0;

        // 从 0 扫描到 12，能放进池子的就放
        for (int i = 0; i < N; i++) {
            if (phase_assignment[i] != -1) continue;  // 已分组的跳过

            if (can_put_in(i)) {
                // 不冲突，放进绿灯池，标记这一波
                current_phase[current_phase_count++] = i;
                phase_assignment[i] = phase_id;
            }
            // 冲突的：什么都不做，直接跳过，留到下一轮
        }

        // 打印这一波的结果
        printf("===== 相位 %d 绿灯放行 =====\n", phase_id);
        for (int k = 0; k < current_phase_count; k++) {
            int id = current_phase[k];
            printf("  %c -> %c\n", routes[id].from, routes[id].to);
        }
        printf("\n");

        phase_id++;
    }

    total_phases = phase_id - 1;
}

//  7. 主函数 
int main() {
    printf("========== 五岔路口交通灯调度方案 ==========\n\n");

    divide_phases();

    printf("========== 总共需要 %d 个相位 ==========\n\n", total_phases);

    printf("各路线所属相位汇总：\n");
    for (int i = 0; i < N; i++) {
        printf("  路线 %c->%c  →  相位 %d\n",
               routes[i].from, routes[i].to, phase_assignment[i]);
    }

    return 0;
}
