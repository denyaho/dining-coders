#include "codexion.h"

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

int find_heap(t_heap *hp, t_coder *coder)
{
    int index;

    index = 0;
    while (index < hp->size)
    {
        if (hp->requests[index].coder == coder)
            return index;
        index++;
    }
    return -1; // or some other error value
}

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