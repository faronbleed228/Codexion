#include "codexion.h"

t_queue	*queue_init(t_simulation *sim_struct)
{
	t_queue	*queue_struct;

	queue_struct = malloc(sizeof(t_queue));
	if (!queue_struct)
		return (NULL);
	queue_struct->count = 0;
	queue_struct->head = 0;
	queue_struct->tail = 0;
	queue_struct->capacity = sim_struct->pars_struct->coders;
	queue_struct->queue = malloc(sizeof(t_coder *) * queue_struct->capacity);
	if (!queue_struct->head)
	{
		return (NULL);
	}
	return (queue_struct);
}
