#ifndef __MAIN_H__
#define __MAIN_H__

#define MAX_PAGES 100     // 最大的页数
#define MAX_PROCESSES 10  // 最大进程数
#define PAGE_SIZE 4       // 每页的大小

int mem[PAGE_SIZE * MAX_PAGES];  // 物理内存

struct page {
    int addr;  // 页的物理地址
    struct page* next;
};

struct {
    int size;
    struct page* head;
} freepage_list;  // 空闲页列表

struct {
    int num_pages;
    struct page* map[MAX_PAGES];   // 页映射，map[i].addr就是第 i 页的物理地址
} page_tables[MAX_PROCESSES + 1];  // table[i] 是进程 i 的页表

void init();

void free_page(struct page* page);
void free_pages(int process_id);

struct page* allocate_page();
void allocate_pages(int process_id, int num_pages);

void show_mem();

#endif  // __MAIN_H__