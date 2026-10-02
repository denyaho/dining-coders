#include <includes/codexion.h>


static void set_dongles(t_params *param, int i)
{
    param->coders[i].left_dongle = &param->dongles[i];
    if (i == param->n_coders - 1)
        param->coders[i].right_dongle = &param->dongles[0];
    else
        param->coders[i].right_dongle = &param->dongles[i + 1];
}


int init_coders(t_params *param)
{
    int index;
    t_coder *coders;

    coders = malloc(sizeof(t_coder) * param->n_coders);
    if (!coders)
        return (1);
    param->coders = coders;
    index = 0;
    while (index < param->n_coders)
    {
        coders[index].id = index;
        coders[index].last_compile_time = 0;
        coders[index].compile_count = 0;
        coders[index].param = param;
        set_dongles(param, index);
        index++;
    }
    return (0);
}

void free_dongles(t_params *param)
{
    int index;
    
    index = 0;
    while (index < param->n_coders)
    {
        pthread_mutex_destroy(&param->dongles[index].dongle_lock);
        pthread_cond_destroy(&param->dongles[index].cond);
        index++;
    }
    free(param->dongles);
}

static int init_one_dongle(t_params *param, int index)
{
    t_dongle *dongle;

    dongle = &param->dongles[index];
    if (pthread_mutex_init(&dongle->dongle_lock, NULL) != 0)
        return (1);
    if (pthread_cond_init(&dongle->cond, NULL) != 0)
    {
        pthread_mutex_destroy(&dongle->dongle_lock);
        return (1);
    }
    dongle->id = index;
    dongle->in_use = 0;
    dongle->avaiable_at = 0;
    dongle->param = param;
    return (0);
}

static void destroy_dongles(t_params *param)
{
    int index;

    index = 0;
    while (index < param->n_coders)
    {
        pthread_mutex_destroy(&param->dongles[index].dongle_lock);
        pthread_cond_destroy(&param->dongles[index].cond);
        index++;
    }
}

int init_dongles(t_params *param)
{
    int index;
    t_dongle *dongles;

    dongles = malloc(sizeof(t_dongle) * param->n_coders);
    if (!dongles)
        return (1);
    param->dongles = dongles;
    index = 0;
    while (index < param->n_coders)
    {
        if (init_one_dongle(param, index) != 0)
        {
            destroy_dongles(param);
            free(dongles);
            param->dongles = NULL;
            return (1);
        }
        index++;
    }
    return (0);
}

int run_simulate(t_params *param)
{
    pthread_t thread_coders[param->n_coders];
    int index;

    if (init_dongles(param) != 0)
        return (1);
    if (init_coders(param) != 0)
        return (1);
    
    index = 0;
    while (index < param->n_coders)
    {
        pthread_create(&thread_coders[index], NULL, coder_run, &param->coders[index]);
        index++;
    }
    index = 0;
    while (index < param->n_coders)
    {
        pthread_join(thread_coders[index], NULL);
        index++;
    }



    return (0);
}