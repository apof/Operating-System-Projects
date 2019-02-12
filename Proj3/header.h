#include <semaphore.h>



struct Table {
   int status;
   int space;
   int serve;
   int pay;
   int waiter_id;
   sem_t busy;
   sem_t table;
   int money;
};


struct shared_mem {
    Table* table;
    int table_space;
    int max_bar_space;
    int bar_counter;
    int waiter_request;
    int customer_request;
    int door_answer;
    int group_size;
    int table_answer;
    int bar2;
    int bar4;
    int bar6;
    int bar8;
    int end;
};

struct statistic {
    int total_money;
    int groups_left;
    int groups_left_after_bar;
    int groups_serviced;
};

