#include "codexion.h"

#define LIMIT 2

t_heap *make_heap(int n)
{
    t_heap *hp = malloc(sizeof(t_heap));
    if (hp != NULL)
    {
        hp->size = 0;
        hp->limit = n;
        hp->requests = malloc(sizeof(t_heap_request) * n);
        if (hp->requests == NULL)
        {
            free(hp);
            return NULL;
        }
    }
    return hp;
}

void delete_heap(t_heap *hp)
{
    free(hp->requests);
    free(hp);
}

bool heap_is_full(t_heap *hp)
{
    if (hp->size == hp->limit)
    {
        return true;
    }
    return false;
}

bool heap_is_empty(t_heap *hp)
{
    if (hp->size == 0)
    {
        return true;
    }
    return false;
}

t_heap_request find_heap(t_heap *hp, t_coder *coder)
{
    int index;

    index = 0;
    while (index < hp->size)
    {
        if (hp->requests[index].coder == coder)
            return hp->requests[index];
        index++;
    }
    return (t_heap_request){-1, -1, NULL}; // or some other error value
}

void heap_push(t_heap *hp, t_heap_request req)
{
    int tail_index;
    int parent_index;

    printf("Current heap size: %d\n", hp->size);
    if (heap_is_full(hp))
        return;
    printf("Pushing request with key: %ld\n", req.key);
    tail_index = hp->size;
    hp->size++;
    while (tail_index > 0)
    {
        parent_index = (tail_index - 1 ) / 2;
        if (hp->requests[parent_index].key <= req.key) {
            break;
        }
        hp->requests[tail_index] = hp->requests[parent_index];
        tail_index = parent_index;
    }
    hp->requests[tail_index] = req;
    return;
}

void schedule_heap(t_coder *coder)
{
    t_params *param;
    t_heap_request req;
    param = coder->param;
    
    pthread_mutex_lock(&param->table_lock);
    if (param->scheduler == FIFO) {
        req.key = param->seq++;
        req.coder = coder;
        heap_push(param->wait_heap, req);
    } else {
        req.key = coder->last_compile_start + param->time_to_burnout;
        req.seq = param->seq++;
        req.coder = coder;
        printf("Scheduling request with key: %ld\n", req.key);
        heap_push(param->wait_heap, req);
    }
    pthread_mutex_unlock(&param->table_lock);
}



t_heap_request heap_top(t_heap *hp)
{
    if (heap_is_empty(hp))
        return (t_heap_request){-1, -1, NULL}; // or some other error value
    return hp->requests[0];
}

long heap_pop_back(t_heap *hp)
{
    if (heap_is_empty(hp))
        return -1;
    long tail_data = hp->requests[hp->size - 1].key;
    hp->size--;
    return tail_data;
}

bool heap_pop(t_heap *hp)
{
    int index;
    int child1;
    int child2;

    if (heap_is_empty(hp))
        return false;
    long tail_data = hp->requests[hp->size - 1].key;
    heap_pop_back(hp);
    index = 0;
    while (index * 2 + 1 < hp->size)
    {
        child1 = index * 2 + 1;
        child2 = index * 2 + 2;
        if (child2 < hp->size && hp->requests[child2].key > hp->requests[child1].key)
            child1 = child2;
        if (hp->requests[child1].key >= tail_data)
            break;
        hp->requests[index] = hp->requests[child1];
        index = child1;
    }
    hp->requests[index] = hp->requests[hp->size];
    return true;
}