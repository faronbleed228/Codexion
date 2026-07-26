#include "codexion.h"

void	add_to_queue(t_dongle *dongle_struct, t_coder *coder_struct)
{
	pthread_mutex_lock(&coder_struct->coder_mutex);
	coder_struct->cur_status = WAITING;
	dongle_struct->queue[dongle_struct->queue_n] = coder_struct;
	dongle_struct->queue_n++;
	pthread_mutex_unlock(&coder_struct->coder_mutex);
}

void	fifo_queue(t_dongle *dongle_struct)
{
	if (dongle_struct->queue_n == 1)
	{
		dongle_struct->next_coder = dongle_struct->queue[0]->cod_num;
		dongle_struct->queue_n--;
	}
	else if (dongle_struct->queue_n == 2)
	{
		dongle_struct->next_coder = dongle_struct->queue[0]->cod_num;
		dongle_struct->queue[0] = dongle_struct->queue[1];
		dongle_struct->queue_n--;
	}
}

static int	edf_logic(t_coder *coder_1, t_coder *coder_2,
		t_dongle *dongle_struct)
{
	struct timeval	tv;
	long long		cur_time;
	long long		time_left_01;
	long long		time_left_02;

	pthread_mutex_lock(&coder_1->coder_mutex);
	pthread_mutex_lock(&coder_2->coder_mutex);
	gettimeofday(&tv, NULL);
	cur_time = get_cur_time_ms(tv);
	time_left_01 = cur_time - coder_1->last_comp_time;
	time_left_02 = cur_time - coder_2->last_comp_time;
	if (time_left_01 > time_left_02)
	{
		dongle_struct->queue[0] = coder_1;
		pthread_mutex_unlock(&coder_1->coder_mutex);
		pthread_mutex_unlock(&coder_2->coder_mutex);
		return (coder_2->cod_num);
	}
	dongle_struct->queue[0] = coder_2;
	pthread_mutex_unlock(&coder_1->coder_mutex);
	pthread_mutex_unlock(&coder_2->coder_mutex);
	return (coder_1->cod_num);
}

void	edf_queue(t_dongle *dongle_struct)
{
	t_coder	*coder_1;
	t_coder	*coder_2;

	if (dongle_struct->queue_n == 2)
	{
		coder_1 = dongle_struct->queue[0];
		coder_2 = dongle_struct->queue[1];
		dongle_struct->next_coder = edf_logic(coder_1, coder_2, dongle_struct);
		dongle_struct->queue_n--;
	}
	else
	{
		dongle_struct->next_coder = dongle_struct->queue[0]->cod_num;
		dongle_struct->queue_n--;
	}
}
