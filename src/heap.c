#include "includes/codexion.h"

#define LIMIT 255

typedef struct t_heap
{
    int size;
    int limit;
    long *data;
} s_heap;

s_heap *make_heap(int n)
{
    s_heap *hp = malloc(sizeof(s_heap));
    if (hp != NULL)
    {
        hp->size = 0;
        hp->limit = LIMIT;
        hp->data = malloc(sizeof(long) * n);
        if (hp->data == NULL)
        {
            free(hp);
            return NULL;
        }
    }
    return hp;
}

void delete_heap(s_heap *hp)
{
    free(hp->data);
    free(hp);
}

bool is_full(s_heap *hp)
{
    if (hp->size == hp->limit)
    {
        return true;
    }
    return false;
}

bool is_empty(s_heap *hp)
{
    if (hp->size == 0)
    {
        return true;
    }
    return false;
}

bool push(s_heap *hp, long d)
{
    if (is_full(hp))
    {
        return false;
    }
    hp->data[hp->size + 1] = d;

    int tail_index = hp->size + 1;
    while (tail_index > 0)
    {
        int parent_index = (tail_index - 1 ) / 2;
        if (hp->data[parent_index] >= d) {
            return true;
        }
        hp->data[tail_index] = hp->data[parent_index];
        tail_index = parent_index;
    }
    hp->data[tail_index] = d;
    return true;
}

bool pop(s_heap *hp)
{
    if (is_empty(hp))
    {
        return false;
    }

    long d = hp->data[hp->size];
    
}
