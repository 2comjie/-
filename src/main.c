#include "main.h"

#include <stdio.h>
#include <stdlib.h>

void init() {
    freepage_list.size = 0;
    freepage_list.head = (struct page*)malloc(sizeof(struct page));
    for (int i = 0; i < MAX_PAGES; i++) {
        struct page* p = (struct page*)malloc(sizeof(struct page));
        p->addr = i * PAGE_SIZE;
        free_page(p);
    }
    for (int i = 1; i <= MAX_PROCESSES; i++)
        page_tables[i].num_pages = 0;
}

// 释放页
void free_page(struct page* page) {
    // 插入到空闲页表
    page->next = freepage_list.head->next;
    freepage_list.head->next = page;
    freepage_list.size++;
    // 释放对应的物理内存
    for (int i = 0; i < PAGE_SIZE; i++)
        mem[page->addr + i] = 0;
}

void free_pages(int process_id) {
    for (int i = 0; i < page_tables[process_id].num_pages; i++)
        free_page(page_tables[process_id].map[i]);
    page_tables->num_pages = 0;
}

// 分配页
struct page* allocate_page() {
    struct page* p = freepage_list.head->next;
    freepage_list.head->next = p->next;
    freepage_list.size--;
    return p;
}

void allocate_pages(int process_id, int num_pages) {
    if (freepage_list.size < num_pages) {
        printf("空闲页面不够，分配失败\n");
        return;
    }

    page_tables[process_id].num_pages = num_pages;
    for (int i = 0; i < num_pages; i++) {
        page_tables[process_id].map[i] = allocate_page();
        // 设置分配的物理内存
        for (int j = 0; j < PAGE_SIZE; j++)
            mem[page_tables[process_id].map[i]->addr + j] = process_id;
    }
}

void show_mem() {
    // 每行打印十个页
    printf("\n");
    for (int i = 0; i < MAX_PAGES; i++) {
        for (int j = 0; j < PAGE_SIZE; j++) {
            printf("%d", mem[i * PAGE_SIZE + j]);
        }
        printf(" ");
        if ((i + 1) % 10 == 0)
            printf("\n");
    }
    printf("\n");
}

int main(void) {
    init();
    while (1) {
        printf("1. 分配页\n");
        printf("2. 释放页\n");
        printf("3. 打印页面分配情况\n");
        printf("4. 退出\n");
        printf("输入:");
        int input;
        scanf("%d", &input);
        if (input == 1) {
            printf("进程ID:");
            int id;
            scanf("%d", &id);
            if (id <= 0 || id > MAX_PROCESSES) {
                printf("id不合法\n");
                continue;
            }
            printf("页面数:");
            int num;
            scanf("%d", &num);
            allocate_pages(id, num);
        } else if (input == 2) {
            printf("进程ID:");
            int id;
            scanf("%d", &id);
            if (id <= 0 || id > MAX_PROCESSES || page_tables[id].num_pages == 0) {
                printf("id不合法\n");
                continue;
            }
            free_pages(id);
        } else if (input == 3) {
            show_mem();
        } else if (input == 4) {
            break;
        } else {
            printf("输入错误\n");
        }
    }
    return 0;
}