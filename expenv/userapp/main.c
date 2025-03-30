/*
 * vim: filetype=c:fenc=utf-8:ts=4:et:sw=4:sts=4
 */
#include <inttypes.h>
#include <stddef.h>
#include <math.h>
#include <stdio.h>
#include <sys/mman.h>
#include <syscall.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "graphics.h"
#define MAX_SECTIONS 10
#define ARRAY_SIZE 100

extern void *tlsf_create_with_pool(void* mem, size_t bytes);
extern void *g_heap;
int random_numbers[10];


/**
 * GCC insists on __main
 *    http://gcc.gnu.org/onlinedocs/gccint/Collect2.html
 */
void __main()
{
    size_t heap_size = 32*1024*1024;
    void  *heap_base = mmap(NULL, heap_size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANON, -1, 0);
	g_heap = tlsf_create_with_pool(heap_base, heap_size);
}




void simple_delay(int loop_count) {
    volatile int i;
    for (i = 0; i < loop_count * 10000; i++);
}

// 绘制数组当前状态
void draw_array(int* array, int size, int section_id, int total_sections) {
    int screen_width = g_graphic_dev.XResolution;
    int screen_height = g_graphic_dev.YResolution;
    int section_height = screen_height / total_sections;
    int y_start = section_id * section_height;
    int y_end = y_start + section_height - 1;
    int bar_width = screen_width / size;
    
    // 清除该区域
    COLORREF black = RGB(0, 0, 0);
    
    // 绘制分隔线
    line(0, y_start, screen_width, y_start, RGB(128, 128, 128));
    
    // 清除区域
    int x;
    for (x = 0; x < screen_width; x++) {
        int y;
        for (y = y_start + 1; y < y_end; y++) {
            setPixel(x, y, black);
        }
    }

    int i;
    for (i = 0; i < size; i++) {
        int bar_height = (section_height - 2) * array[i] / size;
        int x_start = i * bar_width;
  
        COLORREF color;
        switch (section_id % 6) {
            case 0: color = RGB(255, 0, 0); break;   
            case 1: color = RGB(0, 255, 0); break;    
            case 2: color = RGB(0, 0, 255); break;    
            case 3: color = RGB(255, 255, 0); break;  
            case 4: color = RGB(0, 255, 255); break;  
            case 5: color = RGB(255, 0, 255); break;  
            default: color = RGB(255, 255, 255);      
        }
        int x;
        for (x = x_start; x < x_start + bar_width && x < screen_width; x++) {
            int y;
            for (y = 0; y < bar_height && y < section_height - 2; y++) {
                setPixel(x, y_end - y, color);
            }
        }
    }
    
    simple_delay(50);  
}

/* void q_sort(int *p, int l, int r) {
    if (l >= r - 1) return;    
    
    int i = l, j = r - 1;
    int pivot = p[l];          
    while (i < j) {
        while (i < j && p[j] > pivot) j--;   
        p[i] = p[j];                          
        while (i < j && p[i] <= pivot) i++;   
        p[j] = p[i];                        
    }
    
    p[i] = pivot;              
    q_sort(p, l, i);         
    q_sort(p, i + 1, r);     
}


void selection_sort(int arr[], int n) {
    int i;
    for (i = 0; i < n - 1; i++) {
        int min_idx = i;     
        int j;
        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;
        }
    }
}


void bubble_sort_basic(int arr[], int n) {
    int i,j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
               
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
} */


 /* void tsk_q(void *pv)
 {  
    printf("This is task foo with tid=%d\r\n", task_getid()); 
    q_sort(random_numbers,0,9);
    int i;
    for(i = 0;i < 10;i++){
        printf("%d ",random_numbers[i]);
    };
    printf("\n");
    task_exit(0);
 }
 
 void tsk_s(void *pv)
 {  
    printf("This is task foo with tid=%d\r\n", task_getid()); 
    selection_sort(random_numbers,10);
    int i;
    for(i = 0;i < 10;i++){
        printf("%d ",random_numbers[i]);
    };
    printf("\n");
    task_exit(0);
 }

 void tsk_b(void *pv)
 {  
    printf("This is task foo with tid=%d\r\n", task_getid()); 
    bubble_sort_basic(random_numbers,10);
    int i;
    for(i = 0;i < 10;i++){
        printf("%d ",random_numbers[i]);
    };
    printf("\n");
    task_exit(0);
 } */
 
void main(void *pv)
{
    printf("task #%d: I'm the first user task(pv=0x%08x)!\r\n",
            task_getid(), pv);

       
       /*  uint32_t seed = (uint32_t)time(NULL);
        srand(seed);
        int i;
        for (i = 0; i < 10; i++) { 
            random_numbers[i] = rand();
                   
        }
        unsigned int  stack_size = 1024*1024; 

        unsigned char *stack_q; 
        stack_q = (unsigned char *)malloc(stack_size );   

        unsigned char *stack_s; 
        stack_s = (unsigned char *)malloc(stack_size ); 
        unsigned char *stack_b;
        stack_b = (unsigned char *)malloc(stack_size ); 
        int tid_q;    
        tid_q = task_create(stack_q+stack_size, &tsk_q, (void *)0); 
        int tid_s;    
        tid_s = task_create(stack_s+stack_size, &tsk_s, (void *)0); 
        int tid_b;    
        tid_b = task_create(stack_b+stack_size, &tsk_b, (void *)0);  */
        

        /* init_graphic(0x143);
    
    
        int num_sections = 4;  
        if (num_sections > MAX_SECTIONS) {
            num_sections = MAX_SECTIONS;
        }

        int arrays[MAX_SECTIONS][ARRAY_SIZE];
        

        unsigned int seed = 123456;
        int s;
        for (s = 0; s < num_sections; s++) {
            int i;
            for (i = 0; i < ARRAY_SIZE; i++) {
                seed = seed * 1103515245 + 12345;
                arrays[s][i] = ((seed / 65536) % 32768) % ARRAY_SIZE + 1;  // 1到ARRAY_SIZE之间的随机数
            }
        }
        
   
        int y;
        for (y = 0; y < g_graphic_dev.YResolution; y++) {
            int x;
            for ( x = 0; x < g_graphic_dev.XResolution; x++) {
                setPixel(x, y, RGB(0, 0, 0));
            }
        }
        
        int sorting_complete[MAX_SECTIONS] = {0};
        int all_complete = 0;

        int i_pos[MAX_SECTIONS] = {0};
        int j_pos[MAX_SECTIONS] = {0};
        

       
        for (s = 0; s < num_sections; s++) {
            draw_array(arrays[s], ARRAY_SIZE, s, num_sections);
        }
        

        while (!all_complete) {
            all_complete = 1;
            int s;
            for (s = 0; s < num_sections; s++) {
                if (!sorting_complete[s]) {
                    all_complete = 0;
                    
                
                    if (j_pos[s] < ARRAY_SIZE - i_pos[s] - 1) {
                        if (arrays[s][j_pos[s]] > arrays[s][j_pos[s] + 1]) {
                          
                            int temp = arrays[s][j_pos[s]];
                            arrays[s][j_pos[s]] = arrays[s][j_pos[s] + 1];
                            arrays[s][j_pos[s] + 1] = temp;
                     
                            draw_array(arrays[s], ARRAY_SIZE, s, num_sections);
                        }
                        j_pos[s]++;
                    } else {
                        j_pos[s] = 0;
                        i_pos[s]++;
                        
                        if (i_pos[s] >= ARRAY_SIZE - 1) {
                            sorting_complete[s] = 1;
                   
                            draw_array(arrays[s], ARRAY_SIZE, s, num_sections);
                        }
                    }
                }
            }
        }
  
        simple_delay(1000);
 
        exit_graphic(); */



        time_t t1, t2;

        t1 = time(&t2);

        printf("t1: %ld\n", t1);
        printf("t2: %ld\n", t2);
        if (t1 == t2) {
            printf("successful: t1 equals t2.\n");
        }   
        else {
            printf("failed: t1 does not equal t2.\n");
        }



    while(1)
        ;
    task_exit(0);
}

